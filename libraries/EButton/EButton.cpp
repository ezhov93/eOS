/*
 * @file   EButton.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Button helper file. Based on millis()/micros()
 */

#include "EButton.h"

#include "ecore/ethread.h"

/*
EButton::EButton(EAbstractPin &pin, Type type) : _pin(pin) {
  _lastState = false;
  _release = _press = false;
  setType(type);
  _debounce = 60;
  _clicks = 0;
  _state = false;
}

void EButton::update() {
  bool state = EButton::state();
  if (state != _lastState) EThread::sleep(_debounce);
  if (state != _state) { 
    _state = state;
    if (_state)
      _press = true;
    else
      _release = true;
    if (_event != null) _event();
  }
  _lastState = state;
}

bool EButton::isChecked() {
  update();
  bool press = _press;
  _press = false;
  return press;
}
*/


