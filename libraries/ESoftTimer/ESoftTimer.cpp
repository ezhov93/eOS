/*
 * @file   ESoftTimer.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Soft timer source file. Based on millis()/micros()
 */

#include "ESoftTimer.h"

ESoftTimer::ESoftTimer(uint32 interval, Type type) {
  setType(type);
  _interval = interval;
  _timer = time();
  _isActive = false;
  _isReady = false;
  _singleShot = false;
  _handler = null;
}

void ESoftTimer::setType(Type type) {
  _type = type;
  if (_type == MS)
    time = millis;
  else
    time = micros;
}

bool ESoftTimer::isReady() {
  update();
  bool tmp = _isReady;
  _isReady = false;
  return tmp;
}

void ESoftTimer::start(int interval) {
  _interval = interval;
  reset();
  _isActive = true;
}

void ESoftTimer::reset() {
  _timer = time();
  _isReady = false;
  _timerSinceStart = _timer;
}

int ESoftTimer::elapsed() const {
  int tmp = isActive() ? time() - _timer : 0;
  return tmp < 0 ? ~tmp : tmp;
}

int ESoftTimer::timeSinceStart() const {
  int tmp = isActive() ? time() - _timerSinceStart : 0;
  return tmp < 0 ? ~tmp : tmp;
}

void ESoftTimer::update() {
  if (!_isActive)
    return;
  else {
    uint32 currentTime = time();
    if (currentTime - _timer >= _interval) {
      _isReady = true;
      _timer = currentTime;
      if (_singleShot) _isActive = false;
      if (_handler) _handler();
    }
  }
}
