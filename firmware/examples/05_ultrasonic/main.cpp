/**
 * 05 — HC-SR04 超声波测距
 *
 * TRIG→PIN_US_TRIG(6), ECHO→PIN_US_ECHO(7)
 * 模块 VCC 若为 5V，ECHO 建议经分压到 3.3V 再接 ESP32-C3。
 */

#include <MakerPhyphox.h>

static float readDistanceCm() {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  // 超时约 30ms ≈ 5m
  unsigned long duration = pulseIn(PIN_US_ECHO, HIGH, 30000UL);
  if (duration == 0) {
    return NAN;
  }
  return duration * 0.0343f / 2.0f;
}

void setup() {
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  digitalWrite(PIN_US_TRIG, LOW);

  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Sonic");

  MakerPhyphox::addSimpleTimedPlot(
      "超声波测距",
      "HC-SR04 距离随时间变化。用手靠近/远离传感器观察曲线。",
      "距离",
      "cm",
      "距离曲线",
      0.0f,
      200.0f);

  Serial.printf("TRIG=GPIO%d ECHO=GPIO%d\n", PIN_US_TRIG, PIN_US_ECHO);
}

void loop() {
  MakerPhyphox::heartbeatLed(600);

  float cm = readDistanceCm();
  if (isnan(cm)) {
    Serial.println(F("no echo"));
    delay(100);
    return;
  }

  // 简单限幅，避免野值冲坏坐标轴观感
  cm = constrain(cm, 0.0f, 400.0f);
  PhyphoxBLE::write(cm);
  Serial.printf("distance=%.1f cm\n", cm);
  delay(80);
}
