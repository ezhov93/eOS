/*
 * @file   taskmonitor.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Task monitor class for FreeRTOS.
 */

#ifndef TASKMONITOR_H
#define TASKMONITOR_H

#include "ecore/eprint.h"
#include "ecore/ethread.h"

class TaskMonitor : public EThread {
  static constexpr bool UseTask = true;

public:
  TaskMonitor(EPrint *print)
      : EThread("TaskMonitor", StackMedium), _print(print),
        _Delay(2000) {}
  virtual void run() final {
    for (;;) {
      static constexpr char label[] = "[DEBG] ";
      _print->print(label);
      _print->println("Task\t\tState\tPri-ty\tStack\t#");

      UBaseType_t taskCount = ECore::numberOfThreads();

      uint32_t totalRuntime;
      TaskStatus_t *taskStatus = new TaskStatus_t[taskCount];
      taskCount = uxTaskGetSystemState(taskStatus, taskCount, &totalRuntime);
      for (int task = 0; task < taskCount; task++) {
        const TaskStatus_t &status = taskStatus[task];
        _print->print(label);
        _print->print(status.pcTaskName);
        _print->print("\t");
        if (strlen(status.pcTaskName) < 9)
          _print->print('\t');
        _print->print(stateToChar(status.eCurrentState));
        _print->print('\t');
        _print->print(status.uxCurrentPriority);
        _print->print('\t');
        _print->print(status.usStackHighWaterMark);
        _print->print('\t');
        _print->print(status.xTaskNumber);
#if configGENERATE_RUN_TIME_STATS == 1
        _print->print('\t');
        _print->print(status.ulRunTimeCounter);
#endif
        _print->println();
      }

      _print->print(label);
      _print->print("Heap Free: ");
      _print->print(xPortGetFreeHeapSize());
      _print->print(" / ");
      _print->println(xPortGetMinimumEverFreeHeapSize());

#if configGENERATE_RUN_TIME_STATS == 1
      _print->print(label);
      _print->print("Total RunTime: ");
      _print->print(totalRuntime);
      _print->println(" ms");
#endif
      _print->print(label);
      _print->print("System Uptime: ");
      _print->print(ECore::tickToMs(xTaskGetTickCount()));
      _print->println(" ms");
      delete[] taskStatus;
      sleep(_Delay);
    }
  }

private:
  EPrint *_print;
  const size_t _Delay;
  static char stateToChar(const eTaskState state) {
    char c;
    switch (state) {
    case eRunning:
      c = '*';
      break;
    case eReady:
      c = 'R';
      break;
    case eBlocked:
      c = 'B';
      break;
    case eSuspended:
      c = 'S';
      break;
    case eDeleted:
      c = 'D';
      break;
    case eInvalid:
      c = 'I';
      break;
    }
    return c;
  }
};

#if configGENERATE_RUN_TIME_STATS == 1
extern "C" {
void vConfigureTimerForRunTimeStats() {
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->LOAD = ~(size_t)(0);
  SysTick->VAL = 0;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

unsigned long vGetTimerForRunTimeStats() {
  enum TickToTime { MS = 1000, US = MS * 1000 };
  static size_t cnt = 0;
  cnt += ((float)SysTick->VAL / (float)configCPU_CLOCK_HZ) * MS;
  SysTick->VAL = 0;
  return cnt;
}

// void SysTick_Handler(void) {}
}

#endif

#endif // TASKMONITOR_H
