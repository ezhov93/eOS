#ifndef ETIMER_H
#define ETIMER_H

#include "ecore.h"
#include "timers.h"

/* Do not use a block time if calling a timer API function
        from a timer callback function, as doing so could cause a
        deadlock! */

class ETimer {
public:
  ETimer(const char *name) {
    _handle =
        xTimerCreate(name, ECore::msToTick(1000), pdTRUE, this, timerFunction);
  }

  ~ETimer() { xTimerDelete(_handle, 100); }

  const char *name() { return pcTimerGetName(_handle); }
  void start(ETimer *ptr = nullptr) {
    xTimerStart(_handle, ptr != nullptr ? blockTime : 0);
  }
  void start(const int msec, ETimer *ptr = nullptr) {
    setInterval(msec, ptr);
    start(ptr);
  }
  void stop(ETimer *ptr = nullptr) {
    xTimerStop(_handle, ptr != nullptr ? blockTime : 0);
  }
  void reset(ETimer *ptr = nullptr) {
    xTimerReset(_handle, ptr != nullptr ? blockTime : 0);
  }
  void setInterval(const int msec, ETimer *ptr = nullptr) {
    xTimerChangePeriod(_handle, ECore::msToTick(msec),
                       ptr != nullptr ? blockTime : 0);
  }
  int interval() { return ECore::tickToMs(xTimerGetPeriod(_handle)); }
  bool isActive() { return xTimerIsTimerActive(_handle); }
  void setSingleShoot(bool enable) {
    vTimerSetReloadMode(_handle, enable ? pdFALSE : pdTRUE);
  }
  int remainingTime() const {
    return xTimerGetExpiryTime(_handle) - xTaskGetTickCount();
  }

protected:
  virtual void run() = 0;

private:
  TimerHandle_t _handle;
  static constexpr auto blockTime = ECore::MaxDelay;
  static void timerFunction(TimerHandle_t timer) {
    ETimer *obj = reinterpret_cast<ETimer *>(pvTimerGetTimerID(timer));
    obj->run();
  }
};

#endif // ETIMER_H
