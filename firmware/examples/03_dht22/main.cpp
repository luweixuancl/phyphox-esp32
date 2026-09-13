/**
 * 03 — DHT22 温湿度
 *
 * 接线：VCC→3V3, GND→GND, DATA→PIN_DHT（默认 GPIO10）
 * 若使用 DHT11，把 DHT_TYPE 改为 DHT11。
 */

#include <MakerPhyphox.h>
#include <DHT.h>

#ifndef DHT_TYPE
#define DHT_TYPE DHT22
#endif

DHT dht(PIN_DHT, DHT_TYPE);

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-DHT");
  dht.begin();

  MakerPhyphox::addDualTimedPlot(
      "温湿度 DHT",
      "DHT22/DHT11 温湿度实验。注意：DHT 读取较慢，约 2 秒一次。",
      "温度",
      "°C",
      "湿度",
      "%");

  Serial.printf("DHT on GPIO%d\n", PIN_DHT);
}

void loop() {
  MakerPhyphox::heartbeatLed(1500);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println(F("DHT read failed"));
    delay(2000);
    return;
  }

  PhyphoxBLE::write(t, h);
  Serial.printf("T=%.1fC  H=%.1f%%\n", t, h);
  delay(2000);
}
