/**
 * 06 — BH1750 光照强度
 *
 * I2C：SDA=4, SCL=5；地址通常 0x23（ADDR 接 GND）或 0x5C（ADDR 接 VCC）。
 */

#include <MakerPhyphox.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Light");

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire)) {
    Serial.println(F("BH1750 not found at 0x23, trying 0x5C..."));
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire)) {
      Serial.println(F("BH1750 not found"));
      while (true) {
        digitalWrite(PIN_STATUS_LED, LOW);
        delay(120);
        digitalWrite(PIN_STATUS_LED, HIGH);
        delay(120);
      }
    }
  }

  MakerPhyphox::addSimpleTimedPlot(
      "光照 BH1750",
      "测量环境照度（lux）。用手遮挡或用手电筒照射观察变化。",
      "照度",
      "lx",
      "照度曲线");

  Serial.printf("I2C SDA=%d SCL=%d\n", PIN_I2C_SDA, PIN_I2C_SCL);
}

void loop() {
  MakerPhyphox::heartbeatLed();

  float lux = lightMeter.readLightLevel();
  if (lux < 0) {
    Serial.println(F("BH1750 read error"));
    delay(200);
    return;
  }

  PhyphoxBLE::write(lux);
  Serial.printf("lux=%.1f\n", lux);
  delay(150);
}
