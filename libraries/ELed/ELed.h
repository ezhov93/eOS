/*
 * @file   ELed.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Led helper class.
 */

#ifndef ELED_H
#define ELED_H

#include "ecore/drivers/eabstractpin.h"

class ELed {
 private:
  const EAbstractPin &_pin;
  const bool _reverse;

 public:
  ELed(const EAbstractPin &pin,
       EAbstractPin::Pull pull = EAbstractPin::PullNone)
      : _pin(pin), _reverse(pull == EAbstractPin::PullUp ? false : true) {
    _pin.init(EAbstractPin::Output, pull);
    _pin.reset();
  }
  void enable(const bool enable = true) { _pin.write(enable ^ _reverse); }
  void disable(const bool disable = true) { enable(!disable); }
  void toggle() const { _pin.toggle(); }
  bool isEnable() const { return _pin.read() ^ _reverse; }
  bool isDisable() const { return !isEnable(); }
};

#endif  // ELED_H
