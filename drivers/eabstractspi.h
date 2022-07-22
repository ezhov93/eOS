/*
 * @file   eabstrtactspi.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Abstract SPI port implementation.
 * @copyright Copyright 2020 Ezhov Mihail. All rights reserved.
 */

#ifndef EABSTRACTSPI_H
#define EABSTRACTSPI_H

#include "eCore/etypes.h"
#include "eabstractpin.h"

class EAbstractSpi {
  E_DISABLE_COPY_MOVE(EAbstractSpi);

 public:
  enum Mode { Mode0, Mode1, Mode2, Mode3 };
  enum DataBits { DataBits8, DataBits16 };

  explicit EAbstractSpi(EAbstractPin *chipSelect = null)
      : _speed(200000),
        _mode(Mode0),
        _dataBits(DataBits16),
        _chipSelect(chipSelect) {}
  ~EAbstractSpi() {}
  void open() { open(_speed); }
  virtual void open(uint32_t speed) = 0;
  virtual void close() = 0;
  virtual bool isOpen() = 0;
  void setSpeed(const uint32_t speed) { _speed = speed; }
  int speed() const { return _speed; }
  void setDataBits(DataBits dataBits) { _dataBits = dataBits; }
  DataBits dataBits() const { return _dataBits; }
  void setMode(const Mode mode) { _mode = mode; }
  Mode mode() const { return _mode; }
  void setChipSelect(const EAbstractPin *chipSelect) {
    if (chipSelect != null) _chipSelect = chipSelect;
  }
  const EAbstractPin *chipSelect() const { return _chipSelect; }
  void chipEnable() {
    if (_chipSelect != null) _chipSelect->reset();
  }
  void chipDisable() {
    if (_chipSelect != null) _chipSelect->set();
  }

  virtual size_t transfer(const size_t) = 0;

 protected:
  size_t _speed;
  Mode _mode;
  DataBits _dataBits;
  const EAbstractPin *_chipSelect;
};

#endif  // EABSTRACTSPI_H
