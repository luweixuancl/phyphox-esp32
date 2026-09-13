#pragma once

#include <Arduino.h>
#include <phyphoxBle.h>
#include "board_pins.h"

/**
 * 青少年创客活动常用的 phyphox 实验构建辅助函数。
 * 目标：减少样板代码，让示例专注于「读传感器 → 发送数据」。
 */
namespace MakerPhyphox {

/** 启动 BLE，并闪烁板载 LED 提示已上电 */
void begin(const char *deviceName);

/** 可选：周期闪烁 LED，表示固件在运行 */
void heartbeatLed(uint32_t intervalMs = 1000);

/**
 * 创建「数值 + 随时间曲线」的单通道实验。
 * write 时使用：PhyphoxBLE::write(value);
 */
void addSimpleTimedPlot(const char *title,
                        const char *description,
                        const char *valueLabel,
                        const char *unit,
                        const char *graphLabel,
                        float minY = NAN,
                        float maxY = NAN);

/**
 * 创建双通道实验（如温湿度）。
 * write 时使用：PhyphoxBLE::write(ch1, ch2);
 */
void addDualTimedPlot(const char *title,
                      const char *description,
                      const char *label1,
                      const char *unit1,
                      const char *label2,
                      const char *unit2);

/**
 * 三通道实验（如 BME280：温度 / 湿度 / 气压）。
 * write 时使用：PhyphoxBLE::write(ch1, ch2, ch3);
 */
void addTripleTimedPlot(const char *title,
                        const char *description,
                        const char *label1,
                        const char *unit1,
                        const char *label2,
                        const char *unit2,
                        const char *label3,
                        const char *unit3);

}  // namespace MakerPhyphox
