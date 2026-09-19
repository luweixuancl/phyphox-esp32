#include "phyphox_ble.h"
#include "phyphox_experiment.h"

#include <string.h>

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "phyphox_ble";

/* ---- UUIDs (phyphox Arduino / wiki) ---- */
static const ble_uuid128_t UUID_EXP_SVC = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x01, 0x00, 0xdf, 0xcd);
static const ble_uuid128_t UUID_EXP_CHR = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x02, 0x00, 0xdf, 0xcd);
static const ble_uuid128_t UUID_EXP_CTRL = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x03, 0x00, 0xdf, 0xcd);
static const ble_uuid128_t UUID_EVENT_CHR = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x04, 0x00, 0xdf, 0xcd);

static const ble_uuid128_t UUID_DATA_SVC = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x01, 0x10, 0xdf, 0xcd);
static const ble_uuid128_t UUID_DATA_CHR = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x02, 0x10, 0xdf, 0xcd);
static const ble_uuid128_t UUID_CFG_CHR = BLE_UUID128_INIT(
    0x4a, 0x51, 0x53, 0xba, 0x40, 0x5e, 0x43, 0x8b, 0x71, 0x46, 0xf7, 0x30, 0x03, 0x10, 0xdf, 0xcd);

#define EXP_XML_CAP 12000
#define CHUNK_SIZE 20

static char s_device_name[32];
static char s_exp_xml[EXP_XML_CAP];
static size_t s_exp_len;
static uint16_t s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
static uint16_t s_exp_val_handle;
static uint16_t s_data_val_handle;
static uint16_t s_ctrl_val_handle;
static uint16_t s_cfg_val_handle;
static uint16_t s_event_val_handle;
static bool s_data_subscribed;
static bool s_started;
static SemaphoreHandle_t s_notify_lock;
static TaskHandle_t s_transfer_task;

static void advertise(void);
static void start_experiment_transfer(void);

static int gap_event(struct ble_gap_event *event, void *arg);
static int gatt_access(uint16_t conn_handle, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt *ctxt, void *arg);

static const struct ble_gatt_svc_def s_gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &UUID_EXP_SVC.u,
        .characteristics =
            (struct ble_gatt_chr_def[]){
                {
                    .uuid = &UUID_EXP_CHR.u,
                    .access_cb = gatt_access,
                    .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                    .val_handle = &s_exp_val_handle,
                },
                {
                    .uuid = &UUID_EXP_CTRL.u,
                    .access_cb = gatt_access,
                    .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
                    .val_handle = &s_ctrl_val_handle,
                },
                {
                    .uuid = &UUID_EVENT_CHR.u,
                    .access_cb = gatt_access,
                    .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
                    .val_handle = &s_event_val_handle,
                },
                {0},
            },
    },
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &UUID_DATA_SVC.u,
        .characteristics =
            (struct ble_gatt_chr_def[]){
                {
                    .uuid = &UUID_DATA_CHR.u,
                    .access_cb = gatt_access,
                    .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                    .val_handle = &s_data_val_handle,
                },
                {
                    .uuid = &UUID_CFG_CHR.u,
                    .access_cb = gatt_access,
                    .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                    .val_handle = &s_cfg_val_handle,
                },
                {0},
            },
    },
    {0},
};

static int notify_raw(uint16_t val_handle, const uint8_t *data, size_t len)
{
    if (s_conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return BLE_HS_ENOTCONN;
    }
    struct os_mbuf *om = ble_hs_mbuf_from_flat(data, len);
    if (!om) {
        return BLE_HS_ENOMEM;
    }
    return ble_gatts_notify_custom(s_conn_handle, val_handle, om);
}

static void transfer_task(void *arg)
{
    (void)arg;
    ESP_LOGI(TAG, "transferring experiment (%u bytes)", (unsigned)s_exp_len);

    ble_gap_adv_stop();

    uint8_t header[CHUNK_SIZE] = {0};
    memcpy(header, "phyphox", 7);
    header[7] = (uint8_t)((s_exp_len >> 24) & 0xFF);
    header[8] = (uint8_t)((s_exp_len >> 16) & 0xFF);
    header[9] = (uint8_t)((s_exp_len >> 8) & 0xFF);
    header[10] = (uint8_t)(s_exp_len & 0xFF);

    uint32_t crc = phyphox_crc32((const uint8_t *)s_exp_xml, s_exp_len);
    header[11] = (uint8_t)((crc >> 24) & 0xFF);
    header[12] = (uint8_t)((crc >> 16) & 0xFF);
    header[13] = (uint8_t)((crc >> 8) & 0xFF);
    header[14] = (uint8_t)(crc & 0xFF);

    if (xSemaphoreTake(s_notify_lock, pdMS_TO_TICKS(2000)) == pdTRUE) {
        notify_raw(s_exp_val_handle, header, sizeof(header));
        xSemaphoreGive(s_notify_lock);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    size_t offset = 0;
    while (offset < s_exp_len) {
        size_t n = s_exp_len - offset;
        if (n > CHUNK_SIZE) {
            n = CHUNK_SIZE;
        }
        if (xSemaphoreTake(s_notify_lock, pdMS_TO_TICKS(2000)) == pdTRUE) {
            notify_raw(s_exp_val_handle, (const uint8_t *)s_exp_xml + offset, n);
            xSemaphoreGive(s_notify_lock);
        }
        offset += n;
        vTaskDelay(pdMS_TO_TICKS(12));
    }

    ESP_LOGI(TAG, "experiment transfer done");
    advertise();
    s_transfer_task = NULL;
    vTaskDelete(NULL);
}

static void start_experiment_transfer(void)
{
    if (s_exp_len == 0) {
        ESP_LOGW(TAG, "no experiment XML");
        return;
    }
    if (s_transfer_task) {
        ESP_LOGW(TAG, "transfer already running");
        return;
    }
    xTaskCreate(transfer_task, "phyphox_xfer", 4096, NULL, 5, &s_transfer_task);
}

static int gatt_access(uint16_t conn_handle, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)arg;
    switch (ctxt->op) {
    case BLE_GATT_ACCESS_OP_READ_CHR:
        return 0;
    case BLE_GATT_ACCESS_OP_WRITE_CHR: {
        uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
        uint8_t buf[32];
        if (len > sizeof(buf)) {
            len = sizeof(buf);
        }
        int rc = ble_hs_mbuf_to_flat(ctxt->om, buf, len, NULL);
        if (rc != 0) {
            return BLE_ATT_ERR_UNLIKELY;
        }
        if (attr_handle == s_ctrl_val_handle && len >= 1 && buf[0] == 1) {
            ESP_LOGI(TAG, "experiment control start (conn=%u)", conn_handle);
            start_experiment_transfer();
        }
        return 0;
    }
    default:
        return BLE_ATT_ERR_UNLIKELY;
    }
}

static void advertise(void)
{
    struct ble_hs_adv_fields fields = {0};
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.name = (uint8_t *)s_device_name;
    fields.name_len = strlen(s_device_name);
    fields.name_is_complete = 1;
    fields.uuids128 = (ble_uuid128_t *)&UUID_EXP_SVC;
    fields.num_uuids128 = 1;
    fields.uuids128_is_complete = 1;

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        /* Name + 128-bit UUID may exceed 31-byte adv payload; drop UUID from adv. */
        ESP_LOGW(TAG, "adv fields with UUID failed (%d), advertising name only", rc);
        memset(&fields, 0, sizeof(fields));
        fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
        fields.name = (uint8_t *)s_device_name;
        fields.name_len = strlen(s_device_name);
        fields.name_is_complete = 1;
        rc = ble_gap_adv_set_fields(&fields);
        if (rc != 0) {
            ESP_LOGE(TAG, "ble_gap_adv_set_fields rc=%d", rc);
            return;
        }

        struct ble_hs_adv_fields rsp = {0};
        rsp.uuids128 = (ble_uuid128_t *)&UUID_EXP_SVC;
        rsp.num_uuids128 = 1;
        rsp.uuids128_is_complete = 1;
        ble_gap_adv_rsp_set_fields(&rsp);
    }

    struct ble_gap_adv_params adv = {0};
    adv.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv.disc_mode = BLE_GAP_DISC_MODE_GEN;
    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv, gap_event, NULL);
    if (rc == BLE_HS_EALREADY) {
        return;
    }
    if (rc != 0) {
        /* Fallback to random address if public unavailable */
        rc = ble_gap_adv_start(BLE_OWN_ADDR_RANDOM, NULL, BLE_HS_FOREVER, &adv, gap_event, NULL);
        if (rc != 0 && rc != BLE_HS_EALREADY) {
            ESP_LOGE(TAG, "ble_gap_adv_start rc=%d", rc);
        }
    }
}

static int gap_event(struct ble_gap_event *event, void *arg)
{
    (void)arg;
    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0) {
            s_conn_handle = event->connect.conn_handle;
            ESP_LOGI(TAG, "connected handle=%u", s_conn_handle);
            struct ble_gap_upd_params params = {
                .itvl_min = 6,
                .itvl_max = 24,
                .latency = 0,
                .supervision_timeout = 50,
                .min_ce_len = 0,
                .max_ce_len = 0,
            };
            ble_gap_update_params(s_conn_handle, &params);
        } else {
            ESP_LOGW(TAG, "connect failed status=%d", event->connect.status);
            advertise();
        }
        return 0;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "disconnected reason=%d", event->disconnect.reason);
        s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
        s_data_subscribed = false;
        advertise();
        return 0;

    case BLE_GAP_EVENT_SUBSCRIBE:
        ESP_LOGI(TAG, "subscribe attr=%u cur_notify=%u", event->subscribe.attr_handle,
                 event->subscribe.cur_notify);
        if (event->subscribe.attr_handle == s_exp_val_handle && event->subscribe.cur_notify) {
            start_experiment_transfer();
        }
        if (event->subscribe.attr_handle == s_data_val_handle) {
            s_data_subscribed = event->subscribe.cur_notify != 0;
        }
        return 0;

    case BLE_GAP_EVENT_MTU:
        ESP_LOGI(TAG, "mtu updated to %u", event->mtu.value);
        return 0;

    case BLE_GAP_EVENT_ADV_COMPLETE:
        advertise();
        return 0;

    default:
        return 0;
    }
}

static void on_sync(void)
{
    int rc = ble_hs_util_ensure_addr(0);
    if (rc != 0) {
        ESP_LOGE(TAG, "ensure_addr rc=%d", rc);
    }
    advertise();
    ESP_LOGI(TAG, "advertising as \"%s\"", s_device_name);
}

static void on_reset(int reason)
{
    ESP_LOGW(TAG, "nimble reset reason=%d", reason);
}

static void host_task(void *param)
{
    (void)param;
    nimble_port_run();
    nimble_port_freertos_deinit();
}

esp_err_t phyphox_ble_start(const char *device_name, const phyphox_experiment_desc_t *experiment)
{
    ESP_RETURN_ON_FALSE(device_name && experiment, ESP_ERR_INVALID_ARG, TAG, "null arg");
    ESP_RETURN_ON_FALSE(!s_started, ESP_ERR_INVALID_STATE, TAG, "already started");

    strncpy(s_device_name, device_name, sizeof(s_device_name) - 1);
    s_device_name[sizeof(s_device_name) - 1] = '\0';

    s_exp_len = phyphox_experiment_build(s_exp_xml, sizeof(s_exp_xml), s_device_name, experiment);
    ESP_RETURN_ON_FALSE(s_exp_len > 0, ESP_ERR_NO_MEM, TAG, "XML build failed");

    s_notify_lock = xSemaphoreCreateMutex();
    ESP_RETURN_ON_FALSE(s_notify_lock, ESP_ERR_NO_MEM, TAG, "mutex");

    ESP_ERROR_CHECK(nimble_port_init());
    ble_hs_cfg.sync_cb = on_sync;
    ble_hs_cfg.reset_cb = on_reset;

    ble_svc_gap_init();
    ble_svc_gatt_init();
    ESP_ERROR_CHECK(ble_gatts_count_cfg(s_gatt_svcs));
    ESP_ERROR_CHECK(ble_gatts_add_svcs(s_gatt_svcs));
    ESP_ERROR_CHECK(ble_svc_gap_device_name_set(s_device_name));

    nimble_port_freertos_init(host_task);
    s_started = true;
    return ESP_OK;
}

bool phyphox_ble_is_subscribed(void)
{
    return s_data_subscribed;
}

esp_err_t phyphox_ble_write(const float *values, size_t count)
{
    if (!values || count == 0 || count > PHYPHX_MAX_CHANNELS) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t buf[PHYPHX_MAX_CHANNELS * 4];
    memcpy(buf, values, count * 4);

    if (xSemaphoreTake(s_notify_lock, pdMS_TO_TICKS(50)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    int rc = notify_raw(s_data_val_handle, buf, count * 4);
    xSemaphoreGive(s_notify_lock);
    return rc == 0 ? ESP_OK : ESP_FAIL;
}

esp_err_t phyphox_ble_write1(float v0)
{
    return phyphox_ble_write(&v0, 1);
}

esp_err_t phyphox_ble_write2(float v0, float v1)
{
    float v[2] = {v0, v1};
    return phyphox_ble_write(v, 2);
}

esp_err_t phyphox_ble_write3(float v0, float v1, float v2)
{
    float v[3] = {v0, v1, v2};
    return phyphox_ble_write(v, 3);
}

esp_err_t phyphox_ble_write4(float v0, float v1, float v2, float v3)
{
    float v[4] = {v0, v1, v2, v3};
    return phyphox_ble_write(v, 4);
}

esp_err_t phyphox_ble_write5(float v0, float v1, float v2, float v3, float v4)
{
    float v[5] = {v0, v1, v2, v3, v4};
    return phyphox_ble_write(v, 5);
}
