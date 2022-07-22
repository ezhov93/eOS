/*
 * @file   eelapsedtimer.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Elapsed timer class
 */

#ifndef EELAPSEDTIMER_H
#define EELAPSEDTIMER_H

#include "eCore/ecore.h"

class EElapsedTimer {
public:
  EElapsedTimer() = default;
  ~EElapsedTimer() = default;
  int64_t elapsed() const { return  ECore::micros() - _micros; }
  uint64_t restart() {
    uint64_t ret = _micros;
    start();
    return _micros;
  }
  void start() { _micros = ECore::micros(); }

private:
  uint64_t _micros = 0;
};


#endif // EELAPSEDTIMER_H