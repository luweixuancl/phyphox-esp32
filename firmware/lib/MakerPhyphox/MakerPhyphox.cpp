#include "MakerPhyphox.h"

namespace MakerPhyphox {

static uint32_t s_lastBlinkMs = 0;
static bool s_ledOn = false;

void begin(const char *deviceName) {
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, HIGH);

  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println(F("=== ESP32-C3 Phyphox Maker ==="));
  Serial.print(F("BLE name: "));
  Serial.println(deviceName);

  PhyphoxBLE::start(deviceName);

  for (int i = 0; i < 2; i++) {
    digitalWrite(PIN_STATUS_LED, LOW);
    delay(80);
    digitalWrite(PIN_STATUS_LED, HIGH);
    delay(80);
  }
}

void heartbeatLed(uint32_t intervalMs) {
  uint32_t now = millis();
  if (now - s_lastBlinkMs < intervalMs) {
    return;
  }
  s_lastBlinkMs = now;
  s_ledOn = !s_ledOn;
  digitalWrite(PIN_STATUS_LED, s_ledOn ? LOW : HIGH);
}

void addSimpleTimedPlot(const char *title,
                        const char *description,
                        const char *valueLabel,
                        const char *unit,
                        const char *graphLabel,
                        float minY,
                        float maxY) {
  PhyphoxBleExperiment experiment;
  experiment.setTitle(title);
  experiment.setCategory("创客活动");
  experiment.setDescription(description);

  PhyphoxBleExperiment::View view;
  view.setLabel("数据");

  PhyphoxBleExperiment::Value value;
  value.setLabel(valueLabel);
  value.setUnit(unit);
  value.setPrecision(2);
  value.setChannel(1);

  PhyphoxBleExperiment::Graph graph;
  graph.setLabel(graphLabel);
  graph.setLabelX("时间");
  graph.setUnitX("s");
  graph.setLabelY(valueLabel);
  graph.setUnitY(unit);
  graph.setChannel(0, 1);
  if (!isnan(minY) && !isnan(maxY)) {
    graph.setMinY(minY, LAYOUT_FIXED);
    graph.setMaxY(maxY, LAYOUT_FIXED);
  }

  PhyphoxBleExperiment::ExportSet exportSet;
  exportSet.setLabel("导出");
  PhyphoxBleExperiment::ExportData exportData;
  exportData.setLabel(valueLabel);
  exportData.setDatachannel(1);
  exportSet.addElement(exportData);

  view.addElement(value);
  view.addElement(graph);
  experiment.addView(view);
  experiment.addExportSet(exportSet);
  PhyphoxBLE::addExperiment(experiment);
}

void addDualTimedPlot(const char *title,
                      const char *description,
                      const char *label1,
                      const char *unit1,
                      const char *label2,
                      const char *unit2) {
  PhyphoxBleExperiment experiment;
  experiment.setTitle(title);
  experiment.setCategory("创客活动");
  experiment.setDescription(description);

  PhyphoxBleExperiment::View view;
  view.setLabel("数据");

  PhyphoxBleExperiment::Value v1;
  v1.setLabel(label1);
  v1.setUnit(unit1);
  v1.setPrecision(2);
  v1.setChannel(1);

  PhyphoxBleExperiment::Value v2;
  v2.setLabel(label2);
  v2.setUnit(unit2);
  v2.setPrecision(2);
  v2.setChannel(2);

  PhyphoxBleExperiment::Graph g1;
  g1.setLabel(label1);
  g1.setLabelX("时间");
  g1.setUnitX("s");
  g1.setLabelY(label1);
  g1.setUnitY(unit1);
  g1.setChannel(0, 1);

  PhyphoxBleExperiment::Graph g2;
  g2.setLabel(label2);
  g2.setLabelX("时间");
  g2.setUnitX("s");
  g2.setLabelY(label2);
  g2.setUnitY(unit2);
  g2.setChannel(0, 2);

  PhyphoxBleExperiment::ExportSet exportSet;
  exportSet.setLabel("导出");
  PhyphoxBleExperiment::ExportData d1;
  d1.setLabel(label1);
  d1.setDatachannel(1);
  PhyphoxBleExperiment::ExportData d2;
  d2.setLabel(label2);
  d2.setDatachannel(2);
  exportSet.addElement(d1);
  exportSet.addElement(d2);

  view.addElement(v1);
  view.addElement(v2);
  view.addElement(g1);
  view.addElement(g2);
  experiment.addView(view);
  experiment.addExportSet(exportSet);
  PhyphoxBLE::addExperiment(experiment);
}

void addTripleTimedPlot(const char *title,
                        const char *description,
                        const char *label1,
                        const char *unit1,
                        const char *label2,
                        const char *unit2,
                        const char *label3,
                        const char *unit3) {
  PhyphoxBleExperiment experiment;
  experiment.setTitle(title);
  experiment.setCategory("创客活动");
  experiment.setDescription(description);

  PhyphoxBleExperiment::View view;
  view.setLabel("数据");

  PhyphoxBleExperiment::Value v1;
  v1.setLabel(label1);
  v1.setUnit(unit1);
  v1.setPrecision(2);
  v1.setChannel(1);

  PhyphoxBleExperiment::Value v2;
  v2.setLabel(label2);
  v2.setUnit(unit2);
  v2.setPrecision(2);
  v2.setChannel(2);

  PhyphoxBleExperiment::Value v3;
  v3.setLabel(label3);
  v3.setUnit(unit3);
  v3.setPrecision(2);
  v3.setChannel(3);

  PhyphoxBleExperiment::Graph g1;
  g1.setLabel(label1);
  g1.setLabelX("时间");
  g1.setUnitX("s");
  g1.setLabelY(label1);
  g1.setUnitY(unit1);
  g1.setChannel(0, 1);

  PhyphoxBleExperiment::Graph g2;
  g2.setLabel(label2);
  g2.setLabelX("时间");
  g2.setUnitX("s");
  g2.setLabelY(label2);
  g2.setUnitY(unit2);
  g2.setChannel(0, 2);

  PhyphoxBleExperiment::Graph g3;
  g3.setLabel(label3);
  g3.setLabelX("时间");
  g3.setUnitX("s");
  g3.setLabelY(label3);
  g3.setUnitY(unit3);
  g3.setChannel(0, 3);

  PhyphoxBleExperiment::ExportSet exportSet;
  exportSet.setLabel("导出");
  PhyphoxBleExperiment::ExportData d1;
  d1.setLabel(label1);
  d1.setDatachannel(1);
  PhyphoxBleExperiment::ExportData d2;
  d2.setLabel(label2);
  d2.setDatachannel(2);
  PhyphoxBleExperiment::ExportData d3;
  d3.setLabel(label3);
  d3.setDatachannel(3);
  exportSet.addElement(d1);
  exportSet.addElement(d2);
  exportSet.addElement(d3);

  view.addElement(v1);
  view.addElement(v2);
  view.addElement(v3);
  view.addElement(g1);
  view.addElement(g2);
  view.addElement(g3);
  experiment.addView(view);
  experiment.addExportSet(exportSet);
  PhyphoxBLE::addExperiment(experiment);
}

}  // namespace MakerPhyphox
