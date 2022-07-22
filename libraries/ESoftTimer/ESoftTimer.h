/*
 * @file   ESoftTimer.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Soft timer class. Based on millis()/micros()
 */

#ifndef ESOFTTIMER_H
#define ESOFTTIMER_H

#include "ecore/ecore.h"

class ESoftTimer {
 public:
  enum Type { MS = 1000, US = MS * 1000 };
  explicit ESoftTimer(uint32 interval = 1000, Type type = MS);
  Type type() const { return _type; }
  void setType(Type type);
  int interval() const { return _interval; }
  bool isActive() const { return _isActive; }
  bool isReady();
  bool isSingleShot() const { return _singleShot; }
  void setSingleShot(bool singleShot = false) { _singleShot = true; }
  void setInterval(int interval) { _interval = interval; }
  void start(int interval);
  void start() { start(_interval); }
  void stop() { _isActive = false; }
  void reset();
  void update();
  int elapsed() const;
  int remaningTime() const { return _interval - elapsed(); }
  int timeSinceStart() const;
  void attachEvent(func_ptr handler) { _handler = handler; }
  void detachEvent() { _handler = null; }

 private:
  uint32 _timer;
  uint32 _timerSinceStart;
  uint32 _interval;
  Type _type;
  uint32 (*time)();
  func_ptr _handler;
  bool _isActive;
  bool _isReady;
  bool _singleShot;
};

#endif  // ESOFTTIMER_H
