/*
 * @file   ebit.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Bits utility functions.
 */

#ifndef EBIT_H
#define EBIT_H

#include "etypes.h"

namespace EBit {
constexpr static size_t bit(const size_t pos) { return 1UL << pos; }

template <typename T>
uint8_t lowByte(const T w) {
  return w & 0xFF;
}
template <typename T>
uint8_t highByte(const T w) {
  return (w >> 8 * (sizeof(T) - 1)) & 0xFF;
}
template <typename T>
bool read(const T value, const size_t pos) {
  return (value >> pos) & 0x01;
}
template <typename T>
void set(T &value, const size_t pos) {
  value |= bit(pos);
}
template <typename T>
void clear(T &value, const size_t pos) {
  value &= ~bit(pos);
}

template <typename T>
void write(T &value, const size_t pos, const bool bitvalue) {
  return bitvalue ? set(value, pos) : clear(value, pos);
}
}  // namespace EBit

#endif  // EBIT_H
