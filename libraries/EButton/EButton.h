/*
 * @file   EButton.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Button helper class. Based on eCore
 */

#ifndef EBUTTON_H
#define EBUTTON_H

#include "ecore/drivers/eabstractpin.h"
// #include "ecore/ethread.h"

// class EButtonNotifiable {
//  public:
//  private:
// };

// class EButtonNotify {
//  public:
//   void attach(EButtonNotifiable &);
//   void dettach(EButtonNotifiable &);

//  protected:
//   void pressed();
//   void released();

//   private:
//   EButtonNotifiable *_notifiable;
// }

class EButton {
public:
  EButton(EAbstractPin &pin,
          const EAbstractPin::Pull pull = EAbstractPin::PullNone)
      : _pin(pin), _pull(pull) {
    _pin.init(EAbstractPin::Input, _pull);
  }

  bool isChecked()  {
	 return _pin.read() ^ ((_pull == EAbstractPin::PullUp) ? false : true);
	}

  auto isPressed() {
		update();
  auto tmp = _pressed;
	_pressed = false;	
	return tmp;
	}

  auto isReleased() {
		update();
	  auto tmp = _released;
	_released = false;	
	return tmp;}

private:
  void update() {
  bool state = isChecked();
  if (state != _state) {
		_state = state;
    if (_state == true) {
			_pressed = true;
    } else {
			_released = true;
  }
}
	}

   EAbstractPin &_pin;
  const EAbstractPin::Pull _pull;
  bool _state =false;
  bool _pressed = false;
  bool _released = false;
};

#endif // EBUTTON_H
