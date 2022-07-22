/*
 * @file   eabstrtactserial.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Abstract serial port implementation.
 */

#ifndef EABSTRACTSERIAL_H
#define EABSTRACTSERIAL_H

#include "ecore/estream.h"

class EAbstractSerial : public EStream {
  E_DISABLE_COPY_MOVE(EAbstractSerial);

 public:
  enum DataBits { Data5 = 0x00, Data6 = 0x20, Data7 = 0x40, Data8 = 0x60 };
  enum Parity {
    NoParity = 0x00,
    EvenParity = 0x06,
    OddParity = 0x02,
    MarkParity = 0x82,
    SpaceParity = 0x86
  };
  enum StopBits { OneStop = 0x00, TwoStop = 0x08 };

  EAbstractSerial() {
    _baudRate = 9600;
    _dataBits = Data8;
    _stopBits = OneStop;
    _parity = NoParity;
  }
  ~EAbstractSerial() {}
  void open() { open(_baudRate); }
  virtual void open(uint32_t baudrate) = 0;
  virtual void close() = 0;
  virtual bool isOpen() = 0;
  void setBaudRate(uint32_t baudrate) { _baudRate = baudrate; }
  int baudrate() const { return _baudRate; }
  void setDataBits(DataBits dataBits) { _dataBits = dataBits; }
  void setParity(Parity parity) { _parity = parity; }
  void setStopBits(StopBits stopBits) { _stopBits = stopBits; }
  DataBits dataBits() const { return _dataBits; }
  Parity parity() const { return _parity; }
  StopBits stopBits() const { return _stopBits; }
  virtual int available(void) = 0;
  virtual int peek(void) = 0;
  virtual int read(void) = 0;
  virtual int availableForWrite(void) = 0;
  virtual void flush(void) = 0;
  virtual size_t write(uint8_t ch) = 0;
  virtual size_t write(const void *buf, uint32_t len) = 0;
  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
  using EPrint::write;

 protected:
  uint32_t _baudRate;
  DataBits _dataBits;
  Parity _parity;
  StopBits _stopBits;

  virtual int timedRead() = 0;
  virtual int timedPeak() = 0;
};

#endif  // EABSTRACTSERIAL_H
