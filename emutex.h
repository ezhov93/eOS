/*
 * @file   emutex.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Mutex and mutex locker class.
 */

#ifndef EMUTEX_H
#define EMUTEX_H

#include "ethread.h"
#include "semphr.h"

class EMutex {
  E_DISABLE_COPY_MOVE(EMutex);

 public:
  EMutex(){
	_handle = xSemaphoreCreateMutex();
		configASSERT(_handle != nullptr);
	}
  ~EMutex() { vSemaphoreDelete(_handle); }

  void lock() { xSemaphoreTake(_handle, portMAX_DELAY); }
  void lock(const size_t ms) { xSemaphoreTake(_handle, ECore::msToTick(ms)); }
  void unlock() { xSemaphoreGive(_handle); }
  void lockFromIrq() { xSemaphoreTakeFromISR(_handle, NULL); }
  void unlockFromIrq() { xSemaphoreGiveFromISR(_handle, NULL); }

 private:
  SemaphoreHandle_t _handle;
};

class MutexLocker {
 public:
  MutexLocker(EMutex &mutex) : _mutex(mutex) { _mutex.lock(); }
  ~MutexLocker() { _mutex.unlock(); }

 private:
  EMutex &_mutex;
};

#endif  // EMUTEX_H
