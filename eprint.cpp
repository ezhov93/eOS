/*
 * @file  eprint.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Base class that provides print() and println().
 */

#include "eprint.h"

#include "eprintable.h"
#include "etypes.h"
#include "limits.h"
#include "math.h"

#ifndef LLONG_MAX
#define LLONG_MAX 9223372036854775807LL
#endif

size_t EPrint::write(const char *str) {
  if (str == NULL) return 0;
  return write((const uint8_t *)str, strlen(str));
}

size_t EPrint::write(const void *buffer, uint32_t size) {
  size_t n = 0;
  uint8_t *ch = (uint8_t *)buffer;
  while (size--) {
    write(*ch++);
    n++;
  }
  return n;
}

size_t EPrint::print(uint8_t b, int base) { return print((uint64_t)b, base); }

size_t EPrint::print(const EString &s) { return write(s.c_str(), s.length()); }

size_t EPrint::print(char c) { return write(c); }

size_t EPrint::print(const char str[]) { return write(str); }

size_t EPrint::print(int n, int base) { return print((long long)n, base); }

size_t EPrint::print(unsigned int n, int base) {
  return print((unsigned long long)n, base);
}

size_t EPrint::print(long n, int base) { return print((long long)n, base); }

size_t EPrint::print(unsigned long n, int base) {
  return print((unsigned long long)n, base);
}

size_t EPrint::print(long long n, int base) {
  if (n < 0) {
    print('-');
    n = -n;
  }
  return printNumber(n, base);
}

size_t EPrint::print(unsigned long long n, int base) {
  return printNumber(n, base);
}

size_t EPrint::print(double n, int digits) { return printFloat(n, digits); }

size_t EPrint::print(const __FlashEStringHelper *ifsh) {
  return print(reinterpret_cast<const char *>(ifsh));
}

size_t EPrint::print(const EPrintable &x) { return x.printTo(*this); }

size_t EPrint::println(void) {
  size_t n = print('\r');
  n += print('\n');
  return n;
}

size_t EPrint::println(const EString &s) {
  size_t n = print(s);
  n += println();
  return n;
}

size_t EPrint::println(char c) {
  size_t n = print(c);
  n += println();
  return n;
}

size_t EPrint::println(const char c[]) {
  size_t n = print(c);
  n += println();
  return n;
}

size_t EPrint::println(uint8_t b, int base) {
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t EPrint::println(int n, int base) {
  size_t s = print(n, base);
  s += println();
  return s;
}

size_t EPrint::println(unsigned int n, int base) {
  size_t s = print(n, base);
  s += println();
  return s;
}

size_t EPrint::println(long n, int base) {
  size_t s = print((long long)n, base);
  s += println();
  return s;
}

size_t EPrint::println(unsigned long n, int base) {
  size_t s = print((unsigned long long)n, base);
  s += println();
  return s;
}

size_t EPrint::println(long long n, int base) {
  size_t s = print(n, base);
  s += println();
  return s;
}

size_t EPrint::println(unsigned long long n, int base) {
  size_t s = print(n, base);
  s += println();
  return s;
}

size_t EPrint::println(double n, int digits) {
  size_t s = print(n, digits);
  s += println();
  return s;
}

size_t EPrint::println(const __FlashEStringHelper *ifsh) {
  size_t n = print(ifsh);
  n += println();
  return n;
}

size_t EPrint::println(const EPrintable &x) {
  size_t n = print(x);
  n += println();
  return n;
}

/*
 * Private methods
 */

size_t EPrint::printNumber(unsigned long long n, uint8_t base) {
  unsigned char buf[CHAR_BIT * sizeof(long long)];
  unsigned long i = 0;
  size_t s = 0;
  if (n == 0) {
    print('0');
    return 1;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--) {
    s += print(
        (char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
  }
  return s;
}

/* According to snprintf(),
 *
 * nextafter((double)numeric_limits<long long>::max(), 0.0) ~= 9.22337e+18
 *
 * This slightly smaller value was picked semi-arbitrarily. */
#define LARGE_DOUBLE_TRESHOLD (9.1e18)

/* THIS FUNCTION SHOULDN'T BE USED IF YOU NEED ACCURATE RESULTS. */
size_t EPrint::printFloat(double number, uint8_t digits) {
  size_t s = 0;
  // Hackish fail-fast behavior for large-magnitude doubles
  if (abs(number) >= LARGE_DOUBLE_TRESHOLD) {
    if (number < 0.0) {
      s = print('-');
    }
    s += print("<large double>");
    return s;
  }

  // Handle negative numbers
  if (number < 0.0) {
    s += print('-');
    number = -number;
  }

  // Simplistic rounding strategy so that e.g. print(1.999, 2)
  // prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; i++) {
    rounding /= 10.0;
  }
  number += rounding;

  // Extract the integer part of the number and print it
  long long int_part = (long long)number;
  double remainder = number - int_part;
  s += print(int_part);

  // EPrint the decimal point, but only if there are digits beyond
  if (digits > 0) {
    s += print(".");
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int to_print = (int)remainder;
    s += print(to_print);
    remainder -= to_print;
  }
  return s;
}
