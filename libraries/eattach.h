/*
 * @file   EAttach.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Attch/detach helper class.
 */

#ifndef EATTACH_H
#define EATTACH_H

#include "ecore/etypes.h"

class EAttached {
 public:
  virtual ~EAttached() {}
  virtual void event() = 0;
  virtual void event(void *) = 0;
};

class EAttach {
 public:
  EAttach() : _attached(0) {}
  void attach(EAttached *attached) { _attached = attached; }
  void detach() { _attached = 0; }

 protected:
  void notify() {
    if (_attached != 0) _attached->event();
  }
  void notify(void *args) {
    if (_attached != 0) _attached->event(args);
  }

 private:
  EAttached *_attached;
};

#endif  // EATTACH_H
