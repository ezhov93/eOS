/*
 * @file   expmovingaverage.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Exponential moving filter.
 * @copyright Copyright 2020 Ezhov Mihail. All rights reserved.
 */

#ifndef EXPMOVINGAVERAGE_H
#define EXPMOVINGAVERAGE_H

#include "ecore/ethread.h"

template <typename T> class ExpMovingAverage {
public:
  ExpMovingAverage(const float coef = 0.f, const int interval = 0)
      : _coef(coef), _value(0), _interval(interval) {}
  void setCoef(const float coef) { _coef = coef; }
  void setInterval(const int interval) { _interval = interval; }
  float add(const T value) {
    if (_interval != 0)
      EThread::sleep(_interval);
    _value += (value - _value) * _coef;
    return _value;
  }
  T value() const { return _value; }
  void reset() { _value = 0; }
  void reset(const T init) { _value = init; }

private:
  float _coef;
  T _value;
  int _interval;
};

#endif // EXPMOVINGAVERAGE_H
