/**
 * 08 — I2C 扫描
 *
 * 串口列出 SDA=GPIO4、SCL=GPIO5 上的设备地址。
 * 常见：MPU6050=0x68，BME280=0x76/0x77，BH1750=0x23/0x5C。
 */

#include <Arduino.h>
#include <Wire.h>
#include "board_pins.h"

void setup() {
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, HIGH);
  Serial.begin(115200);
  delay(300);
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  Wire.setClock(100000);
  Serial.printf("I2C scan  SDA=GPIO%d  SCL=GPIO%d\n", PIN_I2C_SDA, PIN_I2C_SCL);
}

void loop() {
  uint8_t found = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  device at 0x%02X\n", address);
      found++;
      digitalWrite(PIN_STATUS_LED, LOW);
      delay(40);
      digitalWrite(PIN_STATUS_LED, HIGH);
    }
  }
  if (found == 0) {
    Serial.println(F("  no devices. Check 3V3/GND/SDA/SCL."));
  } else {
    Serial.printf("found %u device(s)\n", found);
  }
  Serial.println(F("----"));
  delay(2000);
}
