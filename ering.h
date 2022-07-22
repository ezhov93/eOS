/*
 * @file   ering.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Simple circular buffer template class.
 */

#ifndef ERING_H
#define ERING_H

#include "etypes.h"

template <typename T, const int Size>
class ERing {
 public:
  ERing() { _tail = _head = 0; }
  ~ERing() = default;

  bool isFull() const {
    return (_tail + 1 == _head) || ((_tail == size()) && (_head == 0));
  }

  bool isEmpty() const { return _head == _tail; }

  void push(const T data, bool *ok = null) { push(data, 1, ok); }

  void push(const T *data, int size, bool *ok = null) {
    bool status = true;
    for (int cnt = 0; cnt < size; ++cnt) {
      _data[_tail++] = data[cnt];
      if (_tail >= this->size()) _tail = 0;
      if (isFull()) status = false;
    }
    if (ok) *ok = status;
  }

  T pop(bool *ok = null) {
    T data;
    pop(data, 1, ok);
    return data;
  }

  void pop(T *data, int size, bool *ok = null) {
    for (int cnt = 0; cnt < size; ++cnt) {
      if (isEmpty()) {
        if (ok) *ok = false;
        return;
      }
      data[cnt] = _data[_head++];
      if (_head >= this->size()) _head = 0;
    }
    if (ok) *ok = true;
    return;
  }

  T peek(bool *ok = null) {
    T data;
    peek(data, 1, ok);
    return data;
  }

  void peek(T *data, int size, bool *ok = null) {
    const int head = _head;
    bool status = pop(data, size);
    if (ok) *ok = status;
    _head = head;
  }

  void clear() { _head = _tail; }

  constexpr int size() const { return Size; }

  int count() const {
    int cnt = _tail - _head;
    if (cnt < 0)
      return size() - _head + _tail - 1;
    else
      return cnt;
  }
  int available() const { return count(); }

  void *data_ptr() const { return (void *)_data; }

 private:
  __IO T _data[Size];
  __IO int _tail;
  __IO int _head;
};

#endif  // ERINGBUFFER_H
