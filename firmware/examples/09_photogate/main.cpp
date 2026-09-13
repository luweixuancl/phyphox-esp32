/**
 * 09 — 光电门
 *
 * 对射红外 / 槽型光耦数字输出 → PIN_PHOTOGATE(GPIO3)。
 * 常见模块：未遮挡 HIGH，遮挡 LOW。适合测单摆周期、小车挡光时间。
 */

#include <MakerPhyphox.h>

volatile uint32_t isrEvents = 0;
volatile uint32_t isrPeriodUs = 0;
volatile uint32_t isrPulseUs = 0;
volatile uint32_t isrLastFallUs = 0;
volatile uint8_t isrState = 1;
volatile uint32_t isrLastEdgeUs = 0;

void IRAM_ATTR onGate() {
  const uint32_t now = micros();
  if (now - isrLastEdgeUs < 200) {
    return;
  }
  isrLastEdgeUs = now;
  const uint8_t state = digitalRead(PIN_PHOTOGATE);
  isrState = state;
  isrEvents++;
  if (state == LOW) {
    if (isrLastFallUs != 0) {
      isrPeriodUs = now - isrLastFallUs;
    }
    isrLastFallUs = now;
  } else if (isrLastFallUs != 0) {
    isrPulseUs = now - isrLastFallUs;
  }
}

static void buildExperiment() {
  PhyphoxBleExperiment experiment;
  experiment.setTitle("光电门");
  experiment.setCategory("创客活动");
  experiment.setDescription("GPIO3 光电门。周期为相邻两次挡光，挡光时间为遮挡脉宽。");

  PhyphoxBleExperiment::View view;
  view.setLabel("数据");

  PhyphoxBleExperiment::InfoField info;
  info.setInfo("挡住红外光束。period 为两次下降沿间隔。");

  PhyphoxBleExperiment::Value period;
  period.setLabel("周期");
  period.setUnit("ms");
  period.setPrecision(1);
  period.setChannel(2);

  PhyphoxBleExperiment::Value blocked;
  blocked.setLabel("挡光");
  blocked.setUnit("ms");
  blocked.setPrecision(1);
  blocked.setChannel(3);

  PhyphoxBleExperiment::Value count;
  count.setLabel("次数");
  count.setPrecision(0);
  count.setChannel(4);

  PhyphoxBleExperiment::Graph stateGraph;
  stateGraph.setLabel("门状态");
  stateGraph.setLabelX("时间");
  stateGraph.setLabelY("开通");
  stateGraph.setUnitX("s");
  stateGraph.setChannel(0, 1);
  stateGraph.setMinY(-0.1f, LAYOUT_FIXED);
  stateGraph.setMaxY(1.1f, LAYOUT_FIXED);

  PhyphoxBleExperiment::Graph periodGraph;
  periodGraph.setLabel("周期");
  periodGraph.setLabelX("时间");
  periodGraph.setLabelY("周期");
  periodGraph.setUnitX("s");
  periodGraph.setUnitY("ms");
  periodGraph.setChannel(0, 2);
  periodGraph.setStyle(STYLE_DOTS);

  view.addElement(info);
  view.addElement(period);
  view.addElement(blocked);
  view.addElement(count);
  view.addElement(stateGraph);
  view.addElement(periodGraph);
  experiment.addView(view);

  PhyphoxBleExperiment::ExportSet exports;
  exports.setLabel("导出");
  PhyphoxBleExperiment::ExportData dOpen;
  dOpen.setLabel("开通");
  dOpen.setDatachannel(1);
  PhyphoxBleExperiment::ExportData dPeriod;
  dPeriod.setLabel("周期");
  dPeriod.setDatachannel(2);
  PhyphoxBleExperiment::ExportData dBlock;
  dBlock.setLabel("挡光");
  dBlock.setDatachannel(3);
  PhyphoxBleExperiment::ExportData dCount;
  dCount.setLabel("次数");
  dCount.setDatachannel(4);
  exports.addElement(dOpen);
  exports.addElement(dPeriod);
  exports.addElement(dBlock);
  exports.addElement(dCount);
  experiment.addExportSet(exports);

  PhyphoxBLE::addExperiment(experiment);
}

void setup() {
  pinMode(PIN_PHOTOGATE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PHOTOGATE), onGate, CHANGE);

  MakerPhyphox::begin(DEVICE_NAME_PREFIX "-Gate");
  buildExperiment();
  Serial.printf("Photogate on GPIO%d\n", PIN_PHOTOGATE);
}

void loop() {
  noInterrupts();
  const uint8_t state = isrState;
  const uint32_t periodUs = isrPeriodUs;
  const uint32_t pulseUs = isrPulseUs;
  const uint32_t events = isrEvents;
  interrupts();

  float open = state ? 1.0f : 0.0f;
  float periodMs = periodUs / 1000.0f;
  float blockedMs = pulseUs / 1000.0f;
  float count = static_cast<float>(events);

  digitalWrite(PIN_STATUS_LED, state == LOW ? LOW : HIGH);
  PhyphoxBLE::write(open, periodMs, blockedMs, count);
  delay(20);
}
