# ESP-IDF 5 × phyphox（ESP32-C3 / NimBLE）

本目录是 **原生 ESP-IDF 5** 实现，使用 NimBLE 外设协议栈，与仓库中 Arduino / PlatformIO 示例（`firmware/`）并行存在。

## 环境

- ESP-IDF **v5.3.x**（已在本分支验证 `v5.3.2`）
- 目标芯片：`esp32c3`

```bash
. $IDF_PATH/export.sh   # 或 . ~/esp/esp-idf/export.sh
cd idf
idf.py set-target esp32c3
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

## 选择示例应用

```bash
idf.py menuconfig
# → Phyphox Maker App → Demo / sensor application
```

| 选项 | BLE 名（默认前缀） | 说明 |
|------|-------------------|------|
| Hello | `Maker-C3-Hello` | 无传感器入门 |
| Analog | `Maker-C3-Analog` | GPIO2 ADC |
| Ultrasonic | `Maker-C3-Sonic` | HC-SR04 TRIG=6 ECHO=7 |

也可在 `sdkconfig.defaults` 旁放置 `sdkconfig.defaults.hello` 等，或直接改 `sdkconfig`：

```
CONFIG_PHYPHX_APP_HELLO=y
```

## 组件说明

`components/phyphox_ble/`：

- 广播 / 实现 phyphox 实验服务 `cddf0001-…` 与数据服务 `cddf1001-…`
- 连接后按协议下发实验 XML（header + CRC32 + 分片 notify）
- `phyphox_ble_writeN()` 以 float32 LE 推送最多 5 通道数据

## 手机连接

1. 安装 [phyphox](https://phyphox.org/download/)
2. **+** → **Bluetooth** → 选择 `Maker-C3-*`
3. 实验界面自动加载

接线与活动教案仍见仓库根目录 `docs/`。
