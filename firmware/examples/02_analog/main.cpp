/**
 * 02 — 模拟传感器
 *
 * 将电位器 / 光敏电阻分压电路 / 土壤湿度等接到 PIN_ANALOG。
 * 手机端显示原始 ADC（0–4095）与换算百分比。
 */

#include <MakerPhyphox.h>

void setup() {
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(PIN_ANALOG, INPUT);

  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Analog");

  MakerPhyphox::addDualTimedPlot(
      "模拟传感器",
      "读取 GPIO 模拟量。可用于电位器、光敏、土壤湿度等。",
      "ADC",
      "",
      "百分比",
      "%");

  Serial.printf("Analog pin: GPIO%d\n", PIN_ANALOG);
}

void loop() {
  MakerPhyphox::heartbeatLed();

  float raw = (float)analogRead(PIN_ANALOG);
  float percent = (raw / 4095.0f) * 100.0f;

  PhyphoxBLE::write(raw, percent);
  Serial.printf("raw=%.0f  percent=%.1f%%\n", raw, percent);
  delay(80);
}
