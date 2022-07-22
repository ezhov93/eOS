#include "EFilters.h"
#include <string.h>
#include "ESort.h"

using namespace EFilters;

// Moving Average class
MovingAverage::MovingAverage(int size) {
  _array = new float[_size = size];
  _value = 0;
}

MovingAverage::~MovingAverage() { delete[] _array; }

float MovingAverage::add(float value) {
  _value += value - _array[_counter];
  _array[_counter++] = value;
  if (_counter >= _size) _counter = 0;
  return MovingAverage::value();
}

// Exponent Moving Average class
ExpMovingAverage::ExpMovingAverage(float coef, int interval) {
  _coef = coef;
  _interval = interval;
  _value = 0.0f;
  _timer = 0;
}

float ExpMovingAverage::add(const float value) {
  if (((long)millis() - _timer >= _interval) || (_interval == 0)) {
    _timer = millis();
    _value += (value - _value) * _coef;
  }
  return _value;
}

// simple avarege filter class
Average::Average(int size, int window) {
  _size = size;
  _window = window;
  _counter = 0;
  _array = new float[size];
}

Average::~Average() { delete[] _array; }

float Average::add(float value) {
  _array[_counter++] = value;
  if (_counter >= _size) _counter = 0;
  return Average::value();
}

void Average::clear() { memset(_array, 0, sizeof(_array[0]) * _size); }

float Average::value() {
  float *begin;
  float *array = null;
  float tmp = 0.f;
  if (_size > _window) {
    array = new float[_size];
    memcpy(array, _array, sizeof(array[0]) * _size);
    ESort::selection<float>(_array, _size);
    begin = _array + ((_size - _window) >> 1);
  } else
    begin = _array;
  for (int i = 0; i < _window; ++i) tmp += begin[i];
  if (array != null) delete[] array;
  return tmp / _window;
}

// Median 3n class
float MedianThree::add(const float value) {
  _array[_counter] = value;
  if (++_counter > 2) _counter = 0;

  float &a = _array[0];
  float &b = _array[1];
  float &c = _array[2];

  if ((a <= b) && (a <= c))
    _value = (b <= c) ? b : c;
  else if ((b <= a) && (b <= c))
    _value = (a <= c) ? a : c;
  else
    _value = (a <= b) ? a : b;

  return _value;
}

// Linear class
void LinearFit::setData(float x[], float y[], int size) {
  double sumX, sumY, sumX2, sumXY;
  size /= sizeof(float);
  for (int i = 0; i < size; ++i) {
    sumX += x[i];
    sumY += y[i];
    sumX2 += x[i] * x[i];
    sumXY += y[i] * x[i];
  }
  _a = size * sumXY;
  _a = _a - sumX * sumY;
  _a = _a / (size * sumX2 - sumX * sumX);
  _b = (sumY - _a * sumX) / size;
  _delta = _a * size;
}
