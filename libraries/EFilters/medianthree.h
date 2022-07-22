#ifndef EMEDIANTREE_H
#define EMEDIANTREE_H

// MedianThree - быстрый медианный фильтр 3-го порядка (отсекает выбросы)

#include <array>

template <typename T> class MedianThree {
public:
  MedianThree() = default;
	
  T add(const T value) {
    _array[_counter] = value;
    if (++_counter > 2)
      _counter = 0;

    T &a = _array[0];
    T &b = _array[1];
    T &c = _array[2];

    if ((a <= b) && (a <= c))
      _value = (b <= c) ? b : c;
    else if ((b <= a) && (b <= c))
      _value = (a <= c) ? a : c;
    else
      _value = (a <= b) ? a : b;
		return _value;
  }
  T value() const { return _value; }

private:
  std::array<T, 3> _array{0,0,0};
  T _value = 0;
  int _counter = 0;
};

#endif // EMEDIANTREE_H
