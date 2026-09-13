#pragma once

#include <Arduino.h>
#include <Wire.h>

class MPU6050Simple {
 public:
  bool begin(uint8_t address = 0x68) {
    address_ = address;
    if (!wake(address_)) {
      address_ = (address == 0x68) ? 0x69 : 0x68;
      if (!wake(address_)) {
        return false;
      }
    }
    delay(50);
    return true;
  }

  bool read(float &ax, float &ay, float &az, float &gx, float &gy, float &gz) {
    uint8_t raw[14];
    if (!readRegs(0x3B, raw, sizeof(raw))) {
      return false;
    }
    ax = toInt16(raw[0], raw[1]) / 16384.0f;
    ay = toInt16(raw[2], raw[3]) / 16384.0f;
    az = toInt16(raw[4], raw[5]) / 16384.0f;
    gx = toInt16(raw[8], raw[9]) / 131.0f;
    gy = toInt16(raw[10], raw[11]) / 131.0f;
    gz = toInt16(raw[12], raw[13]) / 131.0f;
    return true;
  }

  uint8_t address() const { return address_; }

 private:
  uint8_t address_ = 0x68;

  static int16_t toInt16(uint8_t hi, uint8_t lo) {
    return static_cast<int16_t>((static_cast<uint16_t>(hi) << 8) | lo);
  }

  bool wake(uint8_t address) {
    Wire.beginTransmission(address);
    Wire.write(0x6B);
    Wire.write(0x00);
    if (Wire.endTransmission() != 0) {
      return false;
    }
    Wire.beginTransmission(address);
    Wire.write(0x1C);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(address);
    Wire.write(0x1B);
    Wire.write(0x00);
    return Wire.endTransmission() == 0;
  }

  bool readRegs(uint8_t reg, uint8_t *buf, size_t len) {
    Wire.beginTransmission(address_);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
      return false;
    }
    const size_t got = Wire.requestFrom(address_, static_cast<uint8_t>(len));
    if (got != len) {
      return false;
    }
    for (size_t i = 0; i < len; ++i) {
      buf[i] = Wire.read();
    }
    return true;
  }
};
