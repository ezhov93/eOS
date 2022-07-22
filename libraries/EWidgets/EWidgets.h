/*
 * @file   EWidgets.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Widget helper class.
 */

#ifndef EWIDGETS_H
#define EWIDGETS_H

#include "EConsole.h"
#include "estring.h"

namespace EWidgets {
static const char *tab = "         ";
class EWidget {
 private:
  EConsole *_console;

 protected:
  const int size = 6;
  EWidget(EConsole *console) { _console = console; }
  EConsole &console() { return *_console; }
  virtual size_t print(const char *) = 0;
  size_t print() { return print(null); }
};

class EProgressBar : public EWidget {
 private:
  int _value;
  int _maximum;

 public:
  explicit EProgressBar(EConsole *);
  int value() const { return _value; }
  void setValue(const int value) { _value = value; };
  int maximum() const { return _maximum; }
  void setMaximum(const int max) { _maximum = max; }
  virtual size_t print(const char *) final;
  static void showWait(EConsole *, int sec, const char *text = null);
};
class EStatus : public EWidget {
 private:
  const char *_ok = "  OK  ";
  const char *_failed = "FAILED";
  bool _status;

 public:
  explicit EStatus(EConsole *);
  bool get() const { return _status; }
  void set(const bool status) { _status = status; }
  virtual size_t print(const char *) final;
  static void showStatus(EConsole *, bool, const char *text = null);
};

}  // namespace EWidgets

#endif  // EWIDGETS_H
