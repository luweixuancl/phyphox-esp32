#pragma once

/**
 * ESP32-C3 创客板默认引脚（兼容 DevKitM-1 / 常见 C3 SuperMini）
 *
 * 如使用其他板型，只需改本文件，无需改各示例逻辑。
 * 注意：GPIO8 在多数 C3 开发板上是板载 LED，尽量不要占用做 I2C。
 *       GPIO9 常为 BOOT 按键。
 */

// 板载状态 LED（低电平点亮的板子请在代码中注意极性）
#ifndef PIN_STATUS_LED
#define PIN_STATUS_LED 8
#endif

// 模拟传感器输入（电位器 / 光敏电阻 / 土壤湿度等）
#ifndef PIN_ANALOG
#define PIN_ANALOG 2
#endif

// DHT11 / DHT22 单总线
#ifndef PIN_DHT
#define PIN_DHT 10
#endif

// I2C（BME280 / BH1750 等）
#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 4
#endif
#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 5
#endif

// HC-SR04 超声波
#ifndef PIN_US_TRIG
#define PIN_US_TRIG 6
#endif
#ifndef PIN_US_ECHO
#define PIN_US_ECHO 7
#endif

// BLE 设备名前缀（手机端扫描时显示）
#ifndef DEVICE_NAME_PREFIX
#define DEVICE_NAME_PREFIX "Maker-C3"
#endif
