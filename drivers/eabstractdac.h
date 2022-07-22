/*
 * @file   eabstractdac.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  DAC abstract class
 */

#ifndef EABSTRACTDAC_H
#define EABSTRACTDAC_H

#include "eabstractadc.h"
#include "ecore/emath.h"
#include "ecore/etypes.h"

class EAbstractDac : public EAbstractAdc {
  E_DISABLE_COPY_MOVE(EAbstractDac);

 public:
  EAbstractDac(const float vref, const size_t maxValue)
      : EAbstractAdc(vref, maxValue) {}
  virtual void init() const = 0;
  virtual void setValue(const size_t val) const = 0;
  virtual size_t value() const = 0;
  virtual void setVoltage(const float val) const {
    setValue(EMath::map<float>(val, 0, Vref, 0, MaxValue));
  }
};

#endif  // EABSTRACTDAC_H
