/**
 * 10 — MPU6050 加速度
 *
 * VCC→3V3, GND→GND, SDA→GPIO4, SCL→GPIO5
 * 加速度单位 g。静止约 1g，自由落体时 |a| 接近 0。
 */

#include <MakerPhyphox.h>
#include <MPU6050Simple.h>
#include <Wire.h>

MPU6050Simple imu;
bool imuOk = false;

static void buildExperiment() {
  PhyphoxBleExperiment experiment;
  experiment.setTitle("加速度计");
  experiment.setCategory("创客活动");
  experiment.setDescription("MPU6050 三轴加速度与合加速度，单位 g。");

  PhyphoxBleExperiment::View view;
  view.setLabel("数据");

  PhyphoxBleExperiment::Value ax;
  ax.setLabel("ax");
  ax.setUnit("g");
  ax.setPrecision(3);
  ax.setChannel(1);
  PhyphoxBleExperiment::Value ay;
  ay.setLabel("ay");
  ay.setUnit("g");
  ay.setPrecision(3);
  ay.setChannel(2);
  PhyphoxBleExperiment::Value az;
  az.setLabel("az");
  az.setUnit("g");
  az.setPrecision(3);
  az.setChannel(3);
  PhyphoxBleExperiment::Value amag;
  amag.setLabel("|a|");
  amag.setUnit("g");
  amag.setPrecision(3);
  amag.setChannel(4);

  PhyphoxBleExperiment::Graph magGraph;
  magGraph.setLabel("合加速度");
  magGraph.setLabelX("时间");
  magGraph.setLabelY("|a|");
  magGraph.setUnitX("s");
  magGraph.setUnitY("g");
  magGraph.setChannel(0, 4);

  PhyphoxBleExperiment::Graph xyz;
  xyz.setLabel("三轴");
  xyz.setLabelX("时间");
  xyz.setLabelY("a");
  xyz.setUnitX("s");
  xyz.setUnitY("g");
  xyz.setChannel(0, 1);
  PhyphoxBleExperiment::Graph::Subgraph ySub;
  ySub.setChannel(0, 2);
  ySub.setColor("2bfb4c");
  PhyphoxBleExperiment::Graph::Subgraph zSub;
  zSub.setChannel(0, 3);
  zSub.setColor("ff7e22");
  xyz.addSubgraph(ySub);
  xyz.addSubgraph(zSub);

  view.addElement(ax);
  view.addElement(ay);
  view.addElement(az);
  view.addElement(amag);
  view.addElement(magGraph);
  view.addElement(xyz);
  experiment.addView(view);

  PhyphoxBleExperiment::ExportSet exports;
  exports.setLabel("导出");
  PhyphoxBleExperiment::ExportData dx;
  dx.setLabel("ax");
  dx.setDatachannel(1);
  PhyphoxBleExperiment::ExportData dy;
  dy.setLabel("ay");
  dy.setDatachannel(2);
  PhyphoxBleExperiment::ExportData dz;
  dz.setLabel("az");
  dz.setDatachannel(3);
  PhyphoxBleExperiment::ExportData dm;
  dm.setLabel("amag");
  dm.setDatachannel(4);
  exports.addElement(dx);
  exports.addElement(dy);
  exports.addElement(dz);
  exports.addElement(dm);
  experiment.addExportSet(exports);

  PhyphoxBLE::addExperiment(experiment);
}

void setup() {
  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-IMU");
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  imuOk = imu.begin();
  buildExperiment();
  if (imuOk) {
    Serial.printf("MPU6050 at 0x%02X\n", imu.address());
  } else {
    Serial.println(F("MPU6050 not found. Run 08_i2c_scan."));
  }
}

void loop() {
  MakerPhyphox::heartbeatLed(700);
  float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
  if (imuOk && imu.read(ax, ay, az, gx, gy, gz)) {
    float mag = sqrtf(ax * ax + ay * ay + az * az);
    PhyphoxBLE::write(ax, ay, az, mag);
  }
  delay(20);
}
