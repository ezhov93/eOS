/*
 * @file   esemaphore.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Semaphore tamplate class.
 */

#ifndef ESEMAPHORE_H
#define ESEMAPHORE_H

#include "FreeRTOS.h"
#include "ethread.h"
#include "semphr.h"


template <size_t Size = 0>
class ESemaphore {
  E_DISABLE_COPY_MOVE(ESemaphore)

 public: 
  ESemaphore(size_t count = 0) {
    _handle = (Size == 0) ? xSemaphoreCreateBinary()
                          : xSemaphoreCreateCounting(Size, count);
		configASSERT(_handle != nullptr);
  }
  ~ESemaphore() { vSemaphoreDelete(_handle); }
  bool wait(size_t ms = ECore::MaxDelay) {
    return xSemaphoreTake(_handle, ECore::msToTick(ms) ) == pdPASS ;
  }
  void emmit() { xSemaphoreGive(_handle); }
  void emmitFromIrq() {
    BaseType_t taskWoken = pdFALSE;
    xSemaphoreGiveFromISR(_handle, &taskWoken);
    if (taskWoken) portYIELD_FROM_ISR(taskWoken);
  }


 private:
  SemaphoreHandle_t _handle;
};

#endif  // ESEMAPHORE_H
