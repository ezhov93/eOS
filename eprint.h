/*
 * @file  eprint.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Base class that provides print() and println().
 */

#ifndef EPRINT_H
#define EPRINT_H

#include "eprintable.h"
#include "estring.h"
#include <stdint.h>

class EPrint {
 public:
  enum { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };

  virtual size_t write(uint8_t ch) = 0;
  virtual size_t write(const char *str);
  virtual size_t write(const void *buf, uint32_t len);

  size_t print(const EString &);
  size_t print(char);
  size_t print(const char[]);
  size_t print(uint8_t, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(long long, int = DEC);
  size_t print(unsigned long long, int = DEC);
  size_t print(double, int = 2);
  size_t print(const __FlashEStringHelper *);
  size_t print(const EPrintable &);
  size_t println(void);
  size_t println(const EString &s);
  size_t println(char);
  size_t println(const char[]);
  size_t println(uint8_t, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(long long, int = DEC);
  size_t println(unsigned long long, int = DEC);
  size_t println(double, int = 2);
  size_t println(const __FlashEStringHelper *);
  size_t println(const EPrintable &);

  EPrint() : write_error(0) {}

  int getWriteError() { return write_error; }
  void clearWriteError() { setWriteError(0); }

 protected:
  void setWriteError(int err = 1) { write_error = err; }

 private:
  int write_error;
  size_t printNumber(unsigned long long, uint8_t);
  size_t printFloat(double, uint8_t);
};

#endif  // EPRINT_H
