/*
 * @file  emath.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief   Math header.
 */

#ifndef EMATH_H
#define EMATH_H

#include <stdlib.h>

#include "etypes.h"
#include "math.h"

namespace EMath {
static constexpr double Pi = 3.1415926535897932384626433832795;
static constexpr double HalfPi = 1.5707963267948966192313216916398;
static constexpr double TwoPi = 6.283185307179586476925286766559;
static constexpr double DegToRad = 0.017453292519943295769236907684886;
static constexpr double RadToDeg = 57.29577951306798154814105;
static constexpr double Euler = 2.718281828459045235360287471352;

template <typename T> T min(const T a, const T b) { return (a < b) ? a : b; }

template <typename T> T max(const T a, const T b) { return (a > b) ? a : b; }

template <typename T> T constrain(const T amt, const T low, const T high) {
  return (amt < low) ? low : ((amt > high) ? high : amt);
}

template <typename T> T round(const T x) {
  return static_cast<long>((x >= 0) ? (x + 0.5) : (x - 0.5));
}

template <typename T> T radians(const T deg) { return deg * DegToRad; }

template <typename T> T degrees(const T rad) { return rad * RadToDeg; }

template <typename T> T sq(const T x) { return x * x; }

template <typename T> bool inRange(const T amt, const T low, const T high) {
  return ((amt - high) * (amt - low)) <= 0;
}

template <typename T>
T map(const T value, const T fromStart, const T fromEnd, const T toStart,
      const T toEnd) {
  return (((value - fromStart) * (toEnd - toStart)) / (fromEnd - fromStart) +
          toStart);
}

static void randomSeed(unsigned int seed) {
  if (seed != 0)
    srand(seed);
}

static long random(long howbig) {
  if (howbig == 0)
    return 0;
  return rand() % howbig;
}

static long random(long howsmall, long howbig) {
  if (howsmall >= howbig)
    return howsmall;
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

static uint16_t makeWord(uint16_t w) { return w; }
static uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }
#define word(...) makeWord(__VA_ARGS__)

static constexpr int log2_8(const int x) {
  if (x >= 128)
    return 7;
  else if (x >= 64)
    return 6;
  else if (x >= 32)
    return 5;
  else if (x >= 16)
    return 4;
  else if (x >= 8)
    return 3;
  else if (x >= 4)
    return 2;
  else if (x >= 2)
    return 1;
  else
    return 0;
}

constexpr static int log2(const int x) {
  return (x > 256) ? (log2_8(x >> 8) + 8) : log2_8(x);
}
} // namespace EMath

#endif // EMATH_H
