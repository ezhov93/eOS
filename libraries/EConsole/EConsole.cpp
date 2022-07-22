/*
 * @file   EConsole.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Console input/output helper source file.
 */

#include "EConsole.h"
#include "echar.h"

#define CSI "\x1b["
#define RESET_STYLE CSI "0m"

void EConsole::Cursor::home() { _stream->print(CSI "H"); }

void EConsole::Cursor::setPos(int y, int x) {
  _stream->print(CSI);
  _stream->print(y);
  _stream->print(';');
  _stream->print(x);
  _stream->print('H');
}

void EConsole::Cursor::pos(int *y, int *x) {
  _stream->print(CSI);
  _stream->print("6n");
  _stream->setTimeout(10);
  EString str = _stream->readString();
  if (str.startsWith(CSI) >= 0) {
    EString strY, strX;
    strY = strX = str;
    int begin = strY.indexOf("[");
    int end = strY.indexOf(";");
    strY.remove(end, str.length() - 1);
    strY.remove(0, begin + 1);
    *y = strY.toInt();
    begin = strX.indexOf(";");
    end = strX.indexOf("R");
    strX.remove(end, str.length() - 1);
    strX.remove(0, begin + 1);
    *x = strX.toInt();
  }
}

void EConsole::Cursor::hide(bool hide) {
  _stream->print(CSI);
  _stream->print(hide ? "?25l" : "?25h");
}

EConsole::EConsole(EStream *stream) : _cursor(Cursor(stream)) {
  _stream = stream;
  _style = NoStyle;
  _background = NoColor;
  _color = NoColor;
  _timer.setInterval(10);
}

EString *EConsole::silentScan() {
  if (!_start) {
    print('>');
    _start = true;
    _ret = "";
    _cmd = "";
    _cnt = 0;
  } else if (_timer.isReady()) {
    if (_cmd == (CSI "D")) {
      if (_cnt > 0) --_cnt;
    } else if (_cmd == (CSI "C")) {
      if (_cnt < _ret.length()) _cnt++;
    }
    _timer.stop();
    print(_cmd);
  }

  if (_stream->available() > 0) {
    char ch = (char)_stream->read();
    if (!_control) {
      if (isControl(ch)) {
        if (ch == '\r') {
          println();
          _start = false;
          return &_ret;
        } else if (ch == 127) {  // backspace
          _ret.remove(--_cnt, 1);
          int length = _ret.length();
          if (length != _cnt) {
            ++length;
            while (length--) print(' ');
            print("\r>");
            print(_ret);
            int shift = _ret.length() - _cnt;
            while (shift--) print(CSI "D");
          } else
            print(ch);
        } else {
          _control = true;
          _cmd += ch;
          _timer.start();
        }
      } else if (isPrintable(ch)) {
        if (_cnt == _ret.length())
          _ret += ch;
        else
          _ret.setCharAt(_cnt, ch);
        ++_cnt;
        print(ch);
      }
    } else
      _cmd += ch;
  }
  return null;
}

EString EConsole::scan() {
  EString *str = null;
  while (str == null) str = silentScan();
  return *str;
}

void EConsole::scan(const char *str) {
  int c = read();
  char *ptr = const_cast<char*>(str);
  while (true) {
    if (c >= 0) {
      char ch = (char)c;
      if (ch == '\r' || ch == '\n') break;
      if (isPrintable(ch)) {
        *ptr++ = ch;
        print(ch);
      }
    }
    c = read();
  }
  println();
}

void EConsole::clln() {
  print(CSI "2K");
  print('\r');
}

size_t EConsole::setStyle(const Style style) {
  _style = style;
  return setAttribute(style);
}
size_t EConsole::setColor(const Color color) {
  _color = color;
  return setAttribute(color + 30);
}

size_t EConsole::setBackground(const Color color) {
  _background = color;
  return setAttribute(color + 40);
}

size_t EConsole::setAttribute(int x) {
  size_t size = print(CSI);
  size += print(x, DEC);
  size += print('m');
  return size;
}
