/*
 * @file   eprintable.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Interface class that allows printing of complex types
 */

#ifndef EPRINTABLE_H
#define EPRINTABLE_H

#include <stdlib.h>

class EPrint;

class EPrintable {
 public:
  virtual size_t printTo(EPrint& p) const = 0;
};

#endif  // EPRINTABLE_H
