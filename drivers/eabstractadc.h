/*
 * @file   eabstractadc.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  ADC abstract class
 */

#ifndef EABSTRACTADC_H
#define EABSTRACTADC_H

#include "ecore/emath.h"
#include "ecore/etypes.h"

class EAbstractAdc {
  E_DISABLE_COPY_MOVE(EAbstractAdc);

 public:
  EAbstractAdc(const float vref, const size_t maxValue)
      : Vref(vref), MaxValue(maxValue) {}
  const float Vref;
  const size_t MaxValue;
  virtual void init() const = 0;
  virtual size_t value() const = 0;
  virtual float voltage() const {
    return EMath::map<float>(value(), 0, MaxValue, 0, Vref);
  }
};

#endif  // EABSTRACTADC_H
