#ifndef EFILTERS_H
#define EFILTERS_H

// RunnigAverage - компактная альтернатива фильтра экспоненциальное бегущее
// среднее
// EAverage - отсоритровананое среднее из выборки
// MedianThree - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
// inear - линейная аппроксимация методом наименьших квадратов для двух
// массивов

#include <ecore/ecore.h>

namespace EFilters {
class MovingAverage {
 public:
  explicit MovingAverage(int size = 8);
  ~MovingAverage();
  float add(const float);
  float value() const { return _value / _size; }

 private:
  float *_array;
  float _value;
  int _size;
  int _counter;
};

class ExpMovingAverage {
 public:
  ExpMovingAverage(const float coef = 0.f, int interval = 0);
  void setCoef(const float coef) { _coef = coef; }
  void setInterval(const int interval) { _interval = interval; }
  float add(const float);
  float value() const { return _value; }
  void reset() { _value = 0; }
  void reset(const float init) { _value = init; }

 private:
  float _coef;
  float _value;
  size_t _timer;
  int _interval;
};

class Average {
 public:
  explicit Average(int size = 3, int window = 1);
  ~Average();
  float add(const float);
  void clear();
  float value();

 private:
  int _size;
  int _window;
  float *_array;
  int _counter;
};

class MedianThree {
 public:
  explicit MedianThree() { _counter = _value = 0; }
  float add(const float);
  float value() const { return _value; }

 private:
  float _array[3];
  float _value;
  int _counter;
};

class LinearFit {
 public:
  void setData(const float[], const float[], const int size);
  float a() { return _a; }
  float b() { return _b; }
  float delta() { return _delta; }

 private:
  float _a, _b, _delta;
};
}  // namespace EFilters

#endif  // EFILTERS_H
