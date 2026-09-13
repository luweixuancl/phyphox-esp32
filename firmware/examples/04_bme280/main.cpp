/**
 * 04 — BME280 环境传感器（温度 / 湿度 / 气压）
 *
 * I2C：SDA=PIN_I2C_SDA(4), SCL=PIN_I2C_SCL(5)
 * 地址默认 0x76；若模块是 0x77，修改 BME_ADDR。
 */

#include <MakerPhyphox.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#ifndef BME_ADDR
#define BME_ADDR 0x76
#endif

Adafruit_BME280 bme;

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-BME280");

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  if (!bme.begin(BME_ADDR, &Wire)) {
    Serial.println(F("BME280 not found, check wiring / address"));
    while (true) {
      digitalWrite(PIN_STATUS_LED, LOW);
      delay(120);
      digitalWrite(PIN_STATUS_LED, HIGH);
      delay(120);
    }
  }

  MakerPhyphox::addTripleTimedPlot(
      "环境 BME280",
      "BME280：温度、相对湿度、气压。适合气象小实验与海拔估算。",
      "温度",
      "°C",
      "湿度",
      "%",
      "气压",
      "hPa");

  Serial.printf("I2C SDA=%d SCL=%d addr=0x%02X\n", PIN_I2C_SDA, PIN_I2C_SCL,
                BME_ADDR);
}

void loop() {
  MakerPhyphox::heartbeatLed();

  float t = bme.readTemperature();
  float h = bme.readHumidity();
  float p = bme.readPressure() / 100.0f;  // Pa → hPa

  PhyphoxBLE::write(t, h, p);
  Serial.printf("T=%.1fC  H=%.1f%%  P=%.1fhPa\n", t, h, p);
  delay(200);
}
