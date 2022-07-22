/*
 * @file   ecore.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Main include file for the eCore.
 */

#include "ecore.h"

size_t ECore::_cyclesPerMicrosec = 0;
size_t ECore::_numberOfClesPerMicrosec = 0;

uint64_t ECore::micros() {
  uint64_t ms;
  uint32_t cnt;
  do {
    ms = ECore::millis();
    cnt = xPortSysTickValue;
    ECore::nop();
    ECore::nop();
		ECore::nop();
  } while (ms != ECore::millis());
  const size_t us = (_numberOfClesPerMicrosec - cnt) / _cyclesPerMicrosec;
  uint64_t ret = ms * 1000;
  ret += us;
  return ret;
}
