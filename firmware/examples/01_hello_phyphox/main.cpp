/**
 * 01 — 入门：Hello Phyphox
 *
 * 无需外接传感器。ESP32-C3 生成演示数据，通过 BLE 推送到手机 phyphox。
 * 适合活动第一课时：熟悉烧录、开蓝牙、扫码连接。
 */

#include <MakerPhyphox.h>

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Hello");

  MakerPhyphox::addSimpleTimedPlot(
      "创客入门",
      "ESP32-C3 演示数据。用手靠近板子或摇晃时数值会变化（基于片上温度与运行时间）。",
      "演示值",
      "",
      "演示曲线",
      0.0f,
      100.0f);

  Serial.println(F("Open phyphox -> + -> Bluetooth -> Maker-C3-Hello"));
}

void loop() {
  MakerPhyphox::heartbeatLed(800);

  // 用片上温度 + 慢速正弦，做出「活」的曲线，方便现场演示
  float chipTemp = temperatureRead();  // ESP32 Arduino: °C
  float wave = 50.0f + 35.0f * sinf(millis() / 1500.0f);
  float value = constrain(0.4f * chipTemp + 0.6f * wave, 0.0f, 100.0f);

  PhyphoxBLE::write(value);
  Serial.printf("value=%.2f  chip=%.1fC\n", value, chipTemp);
  delay(100);
}
