#include "phyphox_experiment.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

static const char *TAG = "phyphox_xml";

static int appendf(char *out, size_t cap, size_t *used, const char *fmt, ...)
{
    if (*used >= cap) {
        return -1;
    }
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(out + *used, cap - *used, fmt, ap);
    va_end(ap);
    if (n < 0 || (size_t)n >= cap - *used) {
        return -1;
    }
    *used += (size_t)n;
    return 0;
}

size_t phyphox_experiment_build(char *out, size_t out_cap,
                                const char *device_name,
                                const phyphox_experiment_desc_t *desc)
{
    if (!out || !device_name || !desc || desc->channel_count < 1 ||
        desc->channel_count > PHYPHX_MAX_CHANNELS) {
        return 0;
    }

    size_t used = 0;
    const char *title = desc->title ? desc->title : "ESP-IDF Experiment";
    const char *category = desc->category ? desc->category : "创客活动";
    const char *description =
        desc->description ? desc->description : "ESP32-C3 phyphox sensor (ESP-IDF 5)";
    const char *view_label = desc->view_label ? desc->view_label : "数据";

    if (appendf(out, out_cap, &used,
                "<phyphox version=\"1.15\">\n"
                "<title>%s</title>\n"
                "<category>%s</category>\n"
                "<description>%s</description>\n"
                "<data-containers>\n"
                "\t<container size=\"0\" static=\"false\">CH0</container>\n"
                "\t<container size=\"0\" static=\"false\">CB1</container>\n"
                "\t<container size=\"0\" static=\"false\">CB2</container>\n"
                "\t<container size=\"0\" static=\"false\">CB3</container>\n"
                "\t<container size=\"0\" static=\"false\">CB4</container>\n"
                "\t<container size=\"0\" static=\"false\">CB5</container>\n",
                title, category, description) != 0) {
        goto oom;
    }

    for (int i = 1; i <= desc->channel_count; i++) {
        if (appendf(out, out_cap, &used,
                    "\t<container size=\"0\" static=\"false\">CH%d</container>\n", i) != 0) {
            goto oom;
        }
    }
    if (appendf(out, out_cap, &used, "</data-containers>\n<input>\n") != 0) {
        goto oom;
    }

    if (appendf(out, out_cap, &used,
                "\t<bluetooth name=\"%s\" id=\"phyphoxBLE\" mode=\"notification\" "
                "subscribeOnStart=\"false\">\n",
                device_name) != 0) {
        goto oom;
    }

    for (int i = 1; i <= desc->channel_count; i++) {
        if (appendf(out, out_cap, &used,
                    "\t\t<output char=\"cddf1002-30f7-4671-8b43-5e40ba53514a\" "
                    "conversion=\"float32LittleEndian\" offset=\"%d\">CH%d</output>\n",
                    (i - 1) * 4, i) != 0) {
            goto oom;
        }
    }

    if (appendf(out, out_cap, &used,
                "\t\t<output char=\"cddf1002-30f7-4671-8b43-5e40ba53514a\" "
                "extra=\"time\">CH0</output>\n"
                "\t</bluetooth>\n</input>\n"
                "<output>\n"
                "\t<bluetooth id=\"phyphoxBLE\" name=\"%s\">\n"
                "\t\t<input char=\"cddf1003-30f7-4671-8b43-5e40ba53514a\" "
                "conversion=\"float32LittleEndian\">CB1</input>\n"
                "\t\t<input char=\"cddf1003-30f7-4671-8b43-5e40ba53514a\" "
                "conversion=\"float32LittleEndian\" offset=\"4\">CB2</input>\n"
                "\t\t<input char=\"cddf1003-30f7-4671-8b43-5e40ba53514a\" "
                "conversion=\"float32LittleEndian\" offset=\"8\">CB3</input>\n"
                "\t\t<input char=\"cddf1003-30f7-4671-8b43-5e40ba53514a\" "
                "conversion=\"float32LittleEndian\" offset=\"12\">CB4</input>\n"
                "\t\t<input char=\"cddf1003-30f7-4671-8b43-5e40ba53514a\" "
                "conversion=\"float32LittleEndian\" offset=\"16\">CB5</input>\n"
                "\t</bluetooth>\n</output>\n"
                "<analysis sleep=\"0\" onUserInput=\"false\"></analysis>\n"
                "<views>\n\t<view label=\"%s\">\n",
                device_name, view_label) != 0) {
        goto oom;
    }

    for (int i = 0; i < desc->channel_count; i++) {
        const char *vlabel =
            desc->channels[i].value_label ? desc->channels[i].value_label : "值";
        const char *unit = desc->channels[i].unit ? desc->channels[i].unit : "";
        const char *glabel =
            desc->channels[i].graph_label ? desc->channels[i].graph_label : vlabel;

        if (appendf(out, out_cap, &used,
                    "\t\t<value label=\"%s\" precision=\"2\" unit=\"%s\" factor=\"1\">\n"
                    "\t\t\t<input>CH%d</input>\n"
                    "\t\t</value>\n",
                    vlabel, unit, i + 1) != 0) {
            goto oom;
        }

        if (!isnan(desc->channels[i].min_y) && !isnan(desc->channels[i].max_y)) {
            if (appendf(out, out_cap, &used,
                        "\t\t<graph label=\"%s\" labelX=\"时间\" labelY=\"%s\" "
                        "unitX=\"s\" unitY=\"%s\" "
                        "scaleMinY=\"fixed\" minY=\"%g\" scaleMaxY=\"fixed\" maxY=\"%g\">\n"
                        "\t\t\t<input axis=\"x\">CH0</input>\n"
                        "\t\t\t<input axis=\"y\">CH%d</input>\n"
                        "\t\t</graph>\n",
                        glabel, vlabel, unit, (double)desc->channels[i].min_y,
                        (double)desc->channels[i].max_y, i + 1) != 0) {
                goto oom;
            }
        } else {
            if (appendf(out, out_cap, &used,
                        "\t\t<graph label=\"%s\" labelX=\"时间\" labelY=\"%s\" "
                        "unitX=\"s\" unitY=\"%s\">\n"
                        "\t\t\t<input axis=\"x\">CH0</input>\n"
                        "\t\t\t<input axis=\"y\">CH%d</input>\n"
                        "\t\t</graph>\n",
                        glabel, vlabel, unit, i + 1) != 0) {
                goto oom;
            }
        }
    }

    if (appendf(out, out_cap, &used, "\t</view>\n</views>\n<export>\n\t<set name=\"导出\">\n") !=
        0) {
        goto oom;
    }

    for (int i = 0; i < desc->channel_count; i++) {
        const char *vlabel =
            desc->channels[i].value_label ? desc->channels[i].value_label : "值";
        if (appendf(out, out_cap, &used, "\t\t<data name=\"%s\">CH%d</data>\n", vlabel,
                    i + 1) != 0) {
            goto oom;
        }
    }

    if (appendf(out, out_cap, &used, "\t</set>\n</export>\n</phyphox>") != 0) {
        goto oom;
    }

    ESP_LOGI(TAG, "experiment XML size=%u", (unsigned)used);
    return used;

oom:
    ESP_LOGE(TAG, "experiment XML buffer too small (cap=%u used=%u)", (unsigned)out_cap,
             (unsigned)used);
    return 0;
}
