/*
 * @file   average.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Arithmetical mean filter.
 * @copyright Copyright 2020 Ezhov Mihail. All rights reserved.
 */

#ifndef AVERAGE_H
#define AVERAGE_H

#include <string.h>
#include "eCore/libraries/esort.h"

template <typename T, int Size, int Window> class Average {
public:
  Average() : _counter(0) {}
  T add(const T value) {
    _array[_counter++] = value;
    if (_counter >= Size)
      _counter = 0;
    return Average::value();
  }
  void clear() { memset(_array, 0, sizeof(_array)); }
  float value() {
    T *begin;
    T tmp = 0.f;
    if (Size > Window) {
      T array[Size];
      memcpy(array, _array, sizeof(_array));
      ESort::selection<T>(_array, Size);
      begin = _array + ((Size - Window) >> 1);
    } else
      begin = _array;
    for (int i = 0; i < Window; ++i)
      tmp += begin[i];
    return tmp / Window;
  }

private:
  T _array[Size];
  int _counter;
};

#endif // AVERAGE_H
