/*
 * @file   EWidgets.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Widget helper file.
 */

#include "EWidgets.h"
#include "ecore.h"

using namespace EWidgets;

// Class EProgressBar
EProgressBar::EProgressBar(EConsole *console) : EWidget(console) {
  _maximum = 100;
  _value = 0;
}

size_t EProgressBar::print(const char *text) {
  size_t size = console().print('[');
  const int max = map(_value, 0, _maximum, 0, this->size);
  for (int i = 0; i < this->size; ++i)
    size = console().print((i <= max) ? '#' : '.');
  size = console().print(']');
  if (text != null) {
    size += console().print(' ');
    size += console().print(text);
  }
  return size;
}

void EProgressBar::showWait(EConsole *console, int sec, const char *text) {
  EProgressBar progress(console);
  progress.setMaximum(sec);
  progress.print(text);
  console->print('\r');
  for (int i = 0; i < progress.maximum(); ++i) {
    progress.setValue(i);
    progress.print(null);
    console->print('\r');
    delay(1000);
  }
}

// Class EStatus
EStatus::EStatus(EConsole *console) : EWidget(console) {}

size_t EStatus::print(const char *text) {
  size_t size = console().print('[');
  EConsole::Color color = console().color();

  if (_status) {
    size += console().setColor(EConsole::Green);
    size += console().print(_ok);
  }
  else {
    size += console().setColor(EConsole::Red);
    size += console().print(_failed);
  }
  size += console().setColor(color);
  size += console().print(']');
  if (text != null) {
    size += console().print(' ');
    size += console().print(text);
    size += console().print('\t');
  }
  return size;
}

void EStatus::showStatus(EConsole *console, bool status, const char *text) {
  EStatus widget(console);
  widget.set(status);
  widget.print(text);
}
