/*
 * @file   eobject.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Base object interface.
 */

#ifndef EOBJECT_H
#define EOBJECT_H

#include "etypes.h"

class EObject {
 public:
  virtual void init() = 0;

 protected:
  virtual void update() {}
};

#endif  // EOBJECT_H
