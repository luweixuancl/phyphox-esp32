# 烧录指南

推荐 PlatformIO。手机安装 [phyphox](https://phyphox.org/download/)。也可下载预编译固件，见 [DOWNLOADS.md](DOWNLOADS.md)。

## PlatformIO

```bash
cd firmware
pio run -e 01_hello_phyphox          # 只编译
pio run -e 01_hello_phyphox -t upload
pio device monitor                   # 115200
```

其它实验改环境名即可，例如 `-e 09_photogate`、`-e 10_imu`。`platformio.ini` 已打开 USB CDC、DIO Flash、huge_app 分区。

## Arduino IDE 2

1. 附加开发板地址：`https://espressif.github.io/arduino-esp32/package_esp32_index.json`
2. 安装 **esp32** 支持，板型选 **ESP32C3 Dev Module**
3. USB CDC On Boot: **Enabled**；Flash Mode: **DIO**；Partition: **Huge APP**
4. 库管理器安装 **phyphox BLE**
5. 把 `firmware/lib/MakerPhyphox`、`firmware/lib/MPU6050Simple` 拷到 Arduino `libraries/`
6. 把 `firmware/include/board_pins.h` 放到草图目录，或把 `-I` 指到该路径
7. 打开对应 `firmware/examples/*/main.cpp` 作为草图（可改名为 `.ino`）

更省事的方式仍是 PlatformIO，或直接烧 [预编译 bin](DOWNLOADS.md)。

## 进入下载模式

上传卡在 `Connecting...` 时：按住 BOOT → 点 RST → 松开 BOOT → 再点上传。

## 手机连接

phyphox → **+** → **Bluetooth** → 选择 `Maker-C3-*`。Android 扫描 BLE 需要定位权限。同一时间一块板只服务一部手机。
