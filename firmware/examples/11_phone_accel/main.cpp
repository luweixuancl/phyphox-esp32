/**
 * 11 — 手机加速度计驱动板载 LED
 *
 * 不接外部传感器。phyphox 把手机加速度发给 ESP32-C3，摇动手机时 LED 闪得更快。
 */

#include <MakerPhyphox.h>

float lastMag = 1.0f;

void receivedPhoneData() {
  float x = 0, y = 0, z = 0;
  PhyphoxBLE::read(x, y, z);
  lastMag = sqrtf(x * x + y * y + z * z);
}

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Phone");
  PhyphoxBLE::configHandler = &receivedPhoneData;

  PhyphoxBleExperiment experiment;
  experiment.setTitle("手机到LED");
  experiment.setCategory("创客活动");
  experiment.setDescription("把手机加速度发给开发板。连接后摇动手机，LED 闪烁变快。");

  PhyphoxBleExperiment::View view;
  view.setLabel("说明");
  PhyphoxBleExperiment::InfoField info;
  info.setInfo("连接后摇动手机，板载 LED 跟随 |a|。");
  view.addElement(info);

  PhyphoxBleExperiment::Sensor acc;
  acc.setType(SENSOR_ACCELEROMETER);
  acc.setAverage(true);
  acc.setRate(50);
  acc.mapChannel("x", 1);
  acc.mapChannel("y", 2);
  acc.mapChannel("z", 3);

  experiment.addView(view);
  experiment.addSensor(acc);
  PhyphoxBLE::addExperiment(experiment);

  Serial.println(F("Shake the phone after connecting."));
}

void loop() {
  PhyphoxBLE::poll();
  const float extra = constrain(lastMag - 0.9f, 0.0f, 3.0f);
  const int onMs = 20 + static_cast<int>(extra * 40);
  int offMs = 400 - static_cast<int>(extra * 120);
  if (offMs < 40) {
    offMs = 40;
  }
  digitalWrite(PIN_STATUS_LED, LOW);
  delay(onMs);
  digitalWrite(PIN_STATUS_LED, HIGH);
  delay(offMs);
}
