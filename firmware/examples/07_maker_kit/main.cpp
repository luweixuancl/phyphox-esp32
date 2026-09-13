/**
 * 07 — 创客套件综合固件
 *
 * 通过 platformio.ini 的 -DMAKER_ENABLE_xxx 开关编译需要的传感器。
 * 默认：模拟量 + DHT + 超声波（最多占用 4 个 BLE 通道，符合 phyphox 限制）。
 *
 * 通道约定（按启用情况从 1 起依次占用）：
 *   1 模拟百分比(%)  2 温度(°C)  3 湿度(%)  4 距离(cm)  5 照度(lx) / 气压(hPa)
 */

#include <MakerPhyphox.h>
#include <Wire.h>

#ifndef MAKER_ENABLE_ANALOG
#define MAKER_ENABLE_ANALOG 1
#endif
#ifndef MAKER_ENABLE_DHT
#define MAKER_ENABLE_DHT 1
#endif
#ifndef MAKER_ENABLE_BME280
#define MAKER_ENABLE_BME280 0
#endif
#ifndef MAKER_ENABLE_ULTRASONIC
#define MAKER_ENABLE_ULTRASONIC 1
#endif
#ifndef MAKER_ENABLE_BH1750
#define MAKER_ENABLE_BH1750 0
#endif

#if MAKER_ENABLE_DHT
#include <DHT.h>
#ifndef DHT_TYPE
#define DHT_TYPE DHT22
#endif
DHT dht(PIN_DHT, DHT_TYPE);
#endif

#if MAKER_ENABLE_BME280
#include <Adafruit_BME280.h>
#ifndef BME_ADDR
#define BME_ADDR 0x76
#endif
Adafruit_BME280 bme;
#endif

#if MAKER_ENABLE_BH1750
#include <BH1750.h>
BH1750 lightMeter;
bool lightOk = false;
#endif

#if MAKER_ENABLE_ULTRASONIC
static float readDistanceCm() {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);
  unsigned long duration = pulseIn(PIN_US_ECHO, HIGH, 30000UL);
  if (duration == 0) {
    return NAN;
  }
  return constrain(duration * 0.0343f / 2.0f, 0.0f, 400.0f);
}
#endif

static void buildExperiment() {
  PhyphoxBleExperiment experiment;
  experiment.setTitle("创客套件");
  experiment.setCategory("创客活动");
  experiment.setDescription(
      "ESP32-C3 多传感器套件。在 phyphox 中查看实时数值与曲线，可导出数据。");

  PhyphoxBleExperiment::View view;
  view.setLabel("综合");

  int channel = 1;

#if MAKER_ENABLE_ANALOG
  PhyphoxBleExperiment::Value vAnalog;
  vAnalog.setLabel("模拟量");
  vAnalog.setUnit("%");
  vAnalog.setPrecision(1);
  vAnalog.setChannel(channel);

  PhyphoxBleExperiment::Graph gAnalog;
  gAnalog.setLabel("模拟量");
  gAnalog.setLabelX("时间");
  gAnalog.setUnitX("s");
  gAnalog.setLabelY("模拟量");
  gAnalog.setUnitY("%");
  gAnalog.setChannel(0, channel);
  view.addElement(vAnalog);
  view.addElement(gAnalog);
  channel++;
#endif

#if MAKER_ENABLE_DHT || MAKER_ENABLE_BME280
  PhyphoxBleExperiment::Value vTemp;
  vTemp.setLabel("温度");
  vTemp.setUnit("°C");
  vTemp.setPrecision(1);
  vTemp.setChannel(channel);
  PhyphoxBleExperiment::Graph gTemp;
  gTemp.setLabel("温度");
  gTemp.setLabelX("时间");
  gTemp.setUnitX("s");
  gTemp.setLabelY("温度");
  gTemp.setUnitY("°C");
  gTemp.setChannel(0, channel);
  view.addElement(vTemp);
  view.addElement(gTemp);
  channel++;

  PhyphoxBleExperiment::Value vHum;
  vHum.setLabel("湿度");
  vHum.setUnit("%");
  vHum.setPrecision(1);
  vHum.setChannel(channel);
  PhyphoxBleExperiment::Graph gHum;
  gHum.setLabel("湿度");
  gHum.setLabelX("时间");
  gHum.setUnitX("s");
  gHum.setLabelY("湿度");
  gHum.setUnitY("%");
  gHum.setChannel(0, channel);
  view.addElement(vHum);
  view.addElement(gHum);
  channel++;
#endif

#if MAKER_ENABLE_BME280
  PhyphoxBleExperiment::Value vPress;
  vPress.setLabel("气压");
  vPress.setUnit("hPa");
  vPress.setPrecision(1);
  vPress.setChannel(channel);
  PhyphoxBleExperiment::Graph gPress;
  gPress.setLabel("气压");
  gPress.setLabelX("时间");
  gPress.setUnitX("s");
  gPress.setLabelY("气压");
  gPress.setUnitY("hPa");
  gPress.setChannel(0, channel);
  view.addElement(vPress);
  view.addElement(gPress);
  channel++;
#endif

#if MAKER_ENABLE_ULTRASONIC
  PhyphoxBleExperiment::Value vDist;
  vDist.setLabel("距离");
  vDist.setUnit("cm");
  vDist.setPrecision(1);
  vDist.setChannel(channel);
  PhyphoxBleExperiment::Graph gDist;
  gDist.setLabel("距离");
  gDist.setLabelX("时间");
  gDist.setUnitX("s");
  gDist.setLabelY("距离");
  gDist.setUnitY("cm");
  gDist.setMinY(0, LAYOUT_FIXED);
  gDist.setMaxY(200, LAYOUT_FIXED);
  gDist.setChannel(0, channel);
  view.addElement(vDist);
  view.addElement(gDist);
  channel++;
#endif

#if MAKER_ENABLE_BH1750
  PhyphoxBleExperiment::Value vLux;
  vLux.setLabel("照度");
  vLux.setUnit("lx");
  vLux.setPrecision(1);
  vLux.setChannel(channel);
  PhyphoxBleExperiment::Graph gLux;
  gLux.setLabel("照度");
  gLux.setLabelX("时间");
  gLux.setUnitX("s");
  gLux.setLabelY("照度");
  gLux.setUnitY("lx");
  gLux.setChannel(0, channel);
  view.addElement(vLux);
  view.addElement(gLux);
  channel++;
#endif

  if (channel > 6) {
    Serial.println(F("WARNING: more than 5 sensor channels configured"));
  }

  experiment.addView(view);
  PhyphoxBLE::addExperiment(experiment);
}

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Kit");

#if MAKER_ENABLE_ANALOG
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(PIN_ANALOG, INPUT);
#endif

#if MAKER_ENABLE_DHT
  dht.begin();
#endif

#if MAKER_ENABLE_BME280 || MAKER_ENABLE_BH1750
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
#endif

#if MAKER_ENABLE_BME280
  if (!bme.begin(BME_ADDR, &Wire)) {
    Serial.println(F("BME280 missing"));
  }
#endif

#if MAKER_ENABLE_BH1750
  lightOk = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
  if (!lightOk) {
    lightOk = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire);
  }
  if (!lightOk) {
    Serial.println(F("BH1750 missing"));
  }
#endif

#if MAKER_ENABLE_ULTRASONIC
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  digitalWrite(PIN_US_TRIG, LOW);
#endif

  buildExperiment();
  Serial.println(F("Maker kit ready"));
}

void loop() {
  MakerPhyphox::heartbeatLed(700);

  float values[5] = {0, 0, 0, 0, 0};
  int n = 0;

#if MAKER_ENABLE_ANALOG
  values[n++] = (analogRead(PIN_ANALOG) / 4095.0f) * 100.0f;
#endif

#if MAKER_ENABLE_DHT
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t)) t = 0;
    if (isnan(h)) h = 0;
    values[n++] = t;
    values[n++] = h;
  }
#elif MAKER_ENABLE_BME280
  values[n++] = bme.readTemperature();
  values[n++] = bme.readHumidity();
  values[n++] = bme.readPressure() / 100.0f;
#endif

#if MAKER_ENABLE_ULTRASONIC
  {
    float d = readDistanceCm();
    values[n++] = isnan(d) ? -1.0f : d;
  }
#endif

#if MAKER_ENABLE_BH1750
  values[n++] = lightOk ? lightMeter.readLightLevel() : -1.0f;
#endif

  // phyphox write 最多 5 个 float
  switch (n) {
    case 1:
      PhyphoxBLE::write(values[0]);
      break;
    case 2:
      PhyphoxBLE::write(values[0], values[1]);
      break;
    case 3:
      PhyphoxBLE::write(values[0], values[1], values[2]);
      break;
    case 4:
      PhyphoxBLE::write(values[0], values[1], values[2], values[3]);
      break;
    default:
      PhyphoxBLE::write(values[0], values[1], values[2], values[3], values[4]);
      break;
  }

  Serial.print(F("ch:"));
  for (int i = 0; i < n; i++) {
    Serial.printf(" %.2f", values[i]);
  }
  Serial.println();

#if MAKER_ENABLE_DHT && !MAKER_ENABLE_BME280
  delay(2000);  // DHT 需要较慢采样
#else
  delay(120);
#endif
}
