# 固件下载（v0.1.0）

板型：**ESP32-C3**（4MB Flash）  
推荐下载 **合并固件（merged）**：烧录地址 `0x0`，含 bootloader + 分区表 + 应用。

> 若 GitHub 直链较慢，请用同表中的 **镜像站** 链接（可直接点击下载）。

发布标签：[`v0.1.0`](https://github.com/luweixuancl/phyphox-esp32/releases/tag/v0.1.0)（若尚未创建 Release 页面，文件仍可通过下方 raw / 镜像链接下载）

新增示例（光电门 / IMU / I2C 扫描 / 手机加速度计）的预编译文件为 **v0.1.1**，合入 `main` 后可用下方链接。旧的 01–07 仍用 v0.1.0。

---

## 新增固件（v0.1.1）

| 固件 | 用途 | GitHub | 镜像1 (ghproxy) | 镜像2 (jsDelivr) |
|------|------|--------|-----------------|------------------|
| 08 I2C scan | 串口扫地址 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/08_i2c_scan-merged-v0.1.1.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/08_i2c_scan-merged-v0.1.1.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@main/firmware/dist/08_i2c_scan-merged-v0.1.1.bin) |
| 09 Photogate | 光电门周期 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/09_photogate-merged-v0.1.1.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/09_photogate-merged-v0.1.1.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@main/firmware/dist/09_photogate-merged-v0.1.1.bin) |
| 10 IMU | MPU6050 加速度 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/10_imu-merged-v0.1.1.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/10_imu-merged-v0.1.1.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@main/firmware/dist/10_imu-merged-v0.1.1.bin) |
| 11 Phone | 手机加速度计→LED | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/11_phone_accel-merged-v0.1.1.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/11_phone_accel-merged-v0.1.1.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@main/firmware/dist/11_phone_accel-merged-v0.1.1.bin) |
| 新增 merged 打包 | ZIP | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/phyphox-esp32-c3-firmware-extra-v0.1.1.zip) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/main/firmware/dist/phyphox-esp32-c3-firmware-extra-v0.1.1.zip) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@main/firmware/dist/phyphox-esp32-c3-firmware-extra-v0.1.1.zip) |

---

## 一键打包（全部合并固件）

| 说明 | GitHub | 镜像1 (ghproxy) | 镜像2 (jsDelivr) |
|------|--------|-----------------|------------------|
| 全部 merged 固件 ZIP | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/phyphox-esp32-c3-firmware-v0.1.0.zip) | [镜像下载](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/phyphox-esp32-c3-firmware-v0.1.0.zip) | [CDN 下载](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/phyphox-esp32-c3-firmware-v0.1.0.zip) |

备用镜像（gh-proxy）：  
[点此下载 ZIP](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/phyphox-esp32-c3-firmware-v0.1.0.zip)

---

## 合并固件（推荐，直接烧录到 0x0）

| 固件 | 用途 | GitHub | 镜像1 (ghproxy) | 镜像2 (jsDelivr) |
|------|------|--------|-----------------|------------------|
| 01 Hello | 入门演示，无需传感器 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/01_hello_phyphox-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/01_hello_phyphox-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/01_hello_phyphox-merged-v0.1.0.bin) |
| 02 Analog | 电位器 / 光敏 / 土壤湿度 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/02_analog-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/02_analog-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/02_analog-merged-v0.1.0.bin) |
| 03 DHT22 | 温湿度 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/03_dht22-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/03_dht22-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/03_dht22-merged-v0.1.0.bin) |
| 04 BME280 | 温湿度气压 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/04_bme280-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/04_bme280-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/04_bme280-merged-v0.1.0.bin) |
| 05 Ultrasonic | HC-SR04 测距 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/05_ultrasonic-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/05_ultrasonic-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/05_ultrasonic-merged-v0.1.0.bin) |
| 06 BH1750 | 光照 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/06_bh1750-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/06_bh1750-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/06_bh1750-merged-v0.1.0.bin) |
| 07 Maker Kit | 多传感器综合 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/07_maker_kit-merged-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/07_maker_kit-merged-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/07_maker_kit-merged-v0.1.0.bin) |

### 合并固件备用镜像（gh-proxy）

- [01 Hello](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/01_hello_phyphox-merged-v0.1.0.bin)
- [02 Analog](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/02_analog-merged-v0.1.0.bin)
- [03 DHT22](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/03_dht22-merged-v0.1.0.bin)
- [04 BME280](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/04_bme280-merged-v0.1.0.bin)
- [05 Ultrasonic](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/05_ultrasonic-merged-v0.1.0.bin)
- [06 BH1750](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/06_bh1750-merged-v0.1.0.bin)
- [07 Maker Kit](https://gh-proxy.com/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/07_maker_kit-merged-v0.1.0.bin)

---

## 仅应用固件（app，烧录到 0x10000）

| 固件 | GitHub | 镜像 (ghproxy) | CDN (jsDelivr) |
|------|--------|----------------|----------------|
| 01 Hello | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/01_hello_phyphox-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/01_hello_phyphox-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/01_hello_phyphox-app-v0.1.0.bin) |
| 02 Analog | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/02_analog-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/02_analog-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/02_analog-app-v0.1.0.bin) |
| 03 DHT22 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/03_dht22-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/03_dht22-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/03_dht22-app-v0.1.0.bin) |
| 04 BME280 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/04_bme280-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/04_bme280-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/04_bme280-app-v0.1.0.bin) |
| 05 Ultrasonic | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/05_ultrasonic-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/05_ultrasonic-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/05_ultrasonic-app-v0.1.0.bin) |
| 06 BH1750 | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/06_bh1750-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/06_bh1750-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/06_bh1750-app-v0.1.0.bin) |
| 07 Maker Kit | [下载](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/07_maker_kit-app-v0.1.0.bin) | [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/07_maker_kit-app-v0.1.0.bin) | [CDN](https://cdn.jsdelivr.net/gh/luweixuancl/phyphox-esp32@v0.1.0/firmware/dist/07_maker_kit-app-v0.1.0.bin) |

---

## 校验与烧录

- 校验文件：[SHA256SUMS.txt](https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/SHA256SUMS.txt) · [镜像](https://ghproxy.net/https://github.com/luweixuancl/phyphox-esp32/raw/v0.1.0/firmware/dist/SHA256SUMS.txt)

**合并固件烧录示例：**

```bash
esptool.py --chip esp32c3 -p /dev/ttyACM0 -b 460800 \
  write_flash 0x0 01_hello_phyphox-merged-v0.1.0.bin
```

Windows 将端口换成 `COMx`。也可用 [Flash Download Tools](https://www.espressif.com/zh-hans/support/download/other-tools)，地址填 `0x0`。

烧录后手机打开 **phyphox** → **+** → **Bluetooth**，连接 `Maker-C3-*`。接线见 [hardware.md](hardware.md)。
