/*
 * @file   eabstractpin.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Pin abstract class
 * @copyright Copyright 2020 Ezhov Mihail. All rights reserved.
 */

#ifndef EABSTRACTPIN_H
#define EABSTRACTPIN_H

#include "ecore/etypes.h"

class EAbstractPin {
 public:
  enum Mode { Input, Output };
  enum Pull { PullUp, PullDown, PullNone };

  constexpr EAbstractPin(const size_t portAddr, const size_t number)
      : _portAddr(portAddr), _number(number) {}

  virtual void init(const Mode, const Pull = PullNone) const = 0;
  virtual void set() const = 0;
  virtual void reset() const = 0;
  virtual bool read() const = 0;
  void toggle() const { read() ? reset() : set(); }
  void write(const bool state) const { (state ? set() : reset()); }
  size_t portAddress() const { return _portAddr; }
  size_t number() const { return _number; }

 private:
  const size_t _portAddr;
  const size_t _number;
};

#endif  // EABSTRACTPIN_H
