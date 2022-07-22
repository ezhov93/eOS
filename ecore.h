/*
 * @file   ecore.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Main include file for the eCore.
 */

#ifndef ECORE_H
#define ECORE_H

#include "FreeRTOS.h"
#include "etypes.h"
#include "task.h"

template <typename T> T clearSwap(T &val) {
  T tmp(val);
  memset(&val, 0, sizeof(T));
  return tmp;
}

class ECore {
public:
  ECore() { updateCoreClock(); }
  enum class State { Suspend, NotStarted, Running };
  static constexpr size_t MaxDelay = portMAX_DELAY * portTICK_PERIOD_MS;
  inline static void exec() { vTaskStartScheduler(); }
  inline static unsigned int tickCount() { return xTaskGetTickCount(); };
  inline static State state() {
    return static_cast<State>(xTaskGetSchedulerState());
  }
  inline static int numberOfThreads() {
    return uxTaskGetNumberOfTasks();
  };
  inline static void threadList(char *str) { return vTaskList(str); }
  inline static constexpr size_t msToTick(int ms) {
    return ms / portTICK_PERIOD_MS;
  }
  inline static constexpr size_t tickToMs(const size_t tick) {
    return tick * (portTICK_PERIOD_MS);
  }
  inline static size_t millis() {
    return tickCount() * portTICK_PERIOD_MS;
  }
  static uint64_t micros();
  inline static void setCoreClock(float mhz) {
    setCoreClockHook(mhz);
    updateCoreClock();
  }
  inline static size_t coreClock() { return configCPU_CLOCK_HZ; }
  inline static void nop() { portNOP(); }

private:
  static void setCoreClockHook(float mhz);
  static size_t _cyclesPerMicrosec;
  static size_t _numberOfClesPerMicrosec;
  static void updateCoreClock() {
    constexpr size_t usPerSec = 1000000;
    _cyclesPerMicrosec = configCPU_CLOCK_HZ / usPerSec;
    _numberOfClesPerMicrosec = xPortSysTickArr + 1;
  }
};

#endif // ECORE_H
