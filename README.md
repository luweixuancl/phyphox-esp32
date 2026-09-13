# ESP32-C3 × phyphox 创客传感器

面向青少年创客 / STEM 活动的 **ESP32-C3 + phyphox** 开源套件：板端采集传感器数据，手机端用 [phyphox](https://phyphox.org/) 实时看数、画曲线、导出实验数据。

## 为什么用 phyphox？

- 手机里已有成熟的实验界面（数值、图表、导出），学生不用另写 App
- 通过 BLE 自动下发实验配置，扫到设备即可用
- 适合课堂：老师少讲软件，多讲测量与探究

## 你需要准备

| 物品 | 说明 |
|------|------|
| ESP32-C3 开发板 | DevKitM-1 / 常见 C3 SuperMini 等 |
| 安卓或 iOS 手机 | 安装 [phyphox](https://phyphox.org/download/) |
| USB 线 | 能传输数据的线 |
| 传感器（按课程选） | 电位器、DHT22、BME280、HC-SR04、BH1750 等 |

默认引脚见 [`firmware/include/board_pins.h`](firmware/include/board_pins.h)，接线说明见 [docs/hardware.md](docs/hardware.md)。

## 固件下载

无需自行编译时可直接下载已构建固件（含国内镜像）：

→ **[固件下载地址（点击下载）](docs/DOWNLOADS.md)**

## 快速开始

### 1. 编译与烧录（PlatformIO）

```bash
cd firmware
pio run -e 01_hello_phyphox -t upload
pio device monitor
```

无需外接传感器：烧录 `01_hello_phyphox` 后即可在手机上看演示曲线。

### 2. 手机连接

1. 打开 **phyphox** → 右上角 **+** → **Bluetooth**
2. 选择名称类似 `Maker-C3-Hello` 的设备
3. 实验界面会自动加载，开始记录

### 3. 示例一览

| 环境名 | 内容 | 传感器 |
|--------|------|--------|
| `01_hello_phyphox` | 入门演示 | 无 |
| `02_analog` | 模拟量 / 百分比 | 电位器、光敏、土壤湿度等 |
| `03_dht22` | 温湿度 | DHT22 / DHT11 |
| `04_bme280` | 温湿度气压 | BME280 |
| `05_ultrasonic` | 测距 | HC-SR04 |
| `06_bh1750` | 光照 | BH1750 |
| `07_maker_kit` | 多传感器综合 | 可编译开关 |

示例：

```bash
pio run -e 05_ultrasonic -t upload
```

## 仓库结构

```
firmware/                 PlatformIO 工程
  include/board_pins.h    统一引脚
  lib/MakerPhyphox/       phyphox 实验辅助库
  examples/               各传感器示例
docs/hardware.md          接线与电平注意
docs/activities.md        活动教案建议
```

## 活动建议

完整课时流程与探究问题见 [docs/activities.md](docs/activities.md)。推荐路径：

1. **第 1 课**：Hello Phyphox（连接与读图）
2. **第 2 课**：模拟传感器（变量控制、校准）
3. **第 3 课**：测距或温湿度（提出问题 → 测数据 → 解释）

## 开发说明

- 依赖官方库 [phyphox BLE](https://github.com/phyphox/phyphox-arduino)（PlatformIO：`staacks/phyphox BLE` ≥ 1.2.4，含 ESP32-C3 单核修复）
- 板型默认 `esp32-c3-devkitm-1`；其他 C3 板可在 `platformio.ini` 修改 `board`
- USB 串口已打开 CDC（`ARDUINO_USB_CDC_ON_BOOT`）；若监视器无输出，换线或按复位

## License

MIT（见 [LICENSE](LICENSE)）。phyphox 相关库遵循其自身许可证（LGPL）。
