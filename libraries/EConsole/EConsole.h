/*
 * @file   EConsole.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Console input/output monitor header file.
 */

#ifndef ECONSOLE_H
#define ECONSOLE_H

#include "ecore/libraries/esofttimer/ESoftTimer.h"
#include "ecore/estream.h"

class EConsole : private EStream {
 public:
  enum Key : char { Key_Normal };
  enum Style : char {
    Normal,
    Bold,
    Underlined = 4,
    Flashing = 5,
    Inverted = 7,
    Invisible = 8,
    NoStyle
  };

  enum Color : char {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    BlueLight,
    White,
    NoColor
  };

  class Cursor {
   public:
    Cursor(EStream *stream) { _stream = stream; }
    void home();
    void setPos(int x, int y);
    void pos(int *x, int *y);
    void hide(bool);
    void visible(bool visible) { hide(false); }

   private:
    EStream *_stream;
  };

  EConsole(EStream *stream);
  using EStream::print;
  using EStream::println;
  Cursor &cursor() { return _cursor; }
  EString scan();
  EString *silentScan();
  void scan(const char *);
  void cls() { EStream::print('\f'); }
  void clln();
  size_t setStyle(const Style style);
  Style style() const { return _style; }
  size_t setColor(const Color color);
  Color color() const { return _color; }
  size_t setBackground(const Color color);
  Color background() const { return _background; }
  EStream *parent() { return _stream; }

 private:
  ESoftTimer _timer;
  EString _ret;
  EString _cmd;
  int _cnt;
  bool _control;
  bool _start;
  EStream *_stream;
  Cursor _cursor;
  Style _style;
  Color _background;
  Color _color;
  virtual int available(void) { return _stream->available(); }
  virtual int peek(void) { return _stream->peek(); }
  virtual void flush(void) { _stream->flush(); }
  using EStream::write;
  virtual size_t write(uint8 ch) {
    _stream->write(ch);
    _stream->flush();
    return 1;
  }
  virtual int read(void) { return _stream->read(); }
  bool isDefaultStyle() {
    return _style == NoStyle && _color == NoColor && _background == NoColor;
  }
  size_t setAttribute(int);
};

#endif  // ECONSOLE_H
