# 硬件接线说明

默认针对 **ESP32-C3** 常见开发板（DevKitM-1、C3 SuperMini 等）。改引脚请编辑 `firmware/include/board_pins.h`。

## 默认引脚

| 功能 | GPIO | 备注 |
|------|------|------|
| 状态 LED | 8 | 多数板载 LED；低电平点亮 |
| 模拟输入 | 2 | ADC1，12-bit |
| DHT 数据 | 10 | 单总线 |
| I2C SDA | 4 | BME280 / BH1750 / MPU6050 |
| I2C SCL | 5 | |
| 超声波 TRIG | 6 | |
| 超声波 ECHO | 7 | 注意电平（见下） |
| 光电门 | 3 | 数字输入，内部上拉 |
| BOOT 键 | 9 | 勿外接负载 |

供电优先使用 **3.3V**。只有明确标注可接 5V 的模块再接 5V，并处理信号电平。

## 各模块接线

### 模拟传感器（`02_analog`）

**电位器**

- 两端：3V3 / GND
- 中心抽头：GPIO2

**光敏电阻（分压）**

- 3V3 → 光敏 → GPIO2 → 10kΩ → GND  
  （或对调光敏与电阻，只是明暗方向相反）

**电容式土壤湿度**

- VCC→3V3，GND→GND，AO→GPIO2

### DHT22 / DHT11（`03_dht22`）

| DHT | ESP32-C3 |
|-----|----------|
| VCC | 3V3 |
| GND | GND |
| DATA | GPIO10 |

建议 DATA 上拉 4.7kΩ～10kΩ 到 3V3（部分模块板载已有）。

### BME280（`04_bme280`）

| BME280 | ESP32-C3 |
|--------|----------|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO4 |
| SCL | GPIO5 |

I2C 地址常见 `0x76` / `0x77`。不对时在示例里改 `BME_ADDR`。

### HC-SR04（`05_ultrasonic`）

| HC-SR04 | ESP32-C3 |
|---------|----------|
| VCC | 5V（模块通常要 5V） |
| GND | GND |
| TRIG | GPIO6 |
| ECHO | **分压后** → GPIO7 |

ECHO 为 5V 脉冲，ESP32-C3  tolerates 3.3V I/O。推荐分压：

```
ECHO -- 2kΩ --+-- GPIO7
              |
             3kΩ
              |
             GND
```

也可用成品「HC-SR04 转 3.3V」模块。

### BH1750（`06_bh1750`）

| BH1750 | ESP32-C3 |
|--------|----------|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO4 |
| SCL | GPIO5 |
| ADDR | GND → 地址 0x23；接 3V3 → 0x5C |

### I2C 扫描（`08_i2c_scan`）

不接 phyphox。SDA→GPIO4，SCL→GPIO5，打开串口 115200 查看地址。

### 光电门（`09_photogate`）

对射红外 / 槽型光耦 / KY-032 一类数字输出：

| 模块 | ESP32-C3 |
|------|----------|
| VCC | 3V3 |
| GND | GND |
| DO | GPIO3 |

通常未遮挡为 HIGH、遮挡为 LOW。逻辑相反时曲线仍可用，只是「开通」含义对调。

### MPU6050（`10_imu`）

| MPU6050 | ESP32-C3 |
|---------|----------|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO4 |
| SCL | GPIO5 |

地址常见 `0x68`（AD0 接 3V3 则为 `0x69`）。找不到时先烧 `08_i2c_scan`。

### 手机到 LED（`11_phone_accel`）

无需外接传感器。连接 `Maker-C3-Phone` 后摇动手机，板载 LED 闪烁加快。

## 创客套件同时接线（`07_maker_kit`）

默认可同时接：

- 模拟量 → GPIO2  
- DHT → GPIO10  
- 超声波 → GPIO6 / GPIO7  

I2C 设备与 DHT/超声波不共用数据线时，可按 `platformio.ini` 里的 `MAKER_ENABLE_*` 打开 BME280 / BH1750。注意 phyphox 单次最多传 **5** 个数值通道。

## 排错清单

1. 手机搜不到：确认已烧录成功、手机蓝牙已开、距离近；复位板子再扫  
2. 有设备无曲线：是否点进实验并开始测量；看串口是否在刷数值  
3. I2C 找不到：烧录 `08_i2c_scan`；交叉确认 SDA/SCL、地址、3V3 供电  
4. 超声波一直无回波：查 ECHO 电平与 TRIG/ECHO 是否接反  
5. DHT 读失败：线别太长、加上下拉/上拉、供电稳定，等待约 2s 采样周期  
6. 光电门无周期：确认 GPIO3 与 GND/3V3，用手反复挡光看次数是否增加  
