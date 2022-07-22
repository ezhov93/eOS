/*
 * @file  estring.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  EString library
 */

#ifndef ESTRING_H
#define ESTRING_H
#ifdef __cplusplus

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "etypes.h"

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

class __FlashEStringHelper;
#define F(string_literal) \
  (reinterpret_cast<const __FlashEStringHelper *>(PSTR(string_literal)))

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class EStringSumHelper;

// The string class
class EString {
  // use a function pointer to allow for "if (s)" without the
  // complications of an operator bool(). for more information, see:
  // http://www.artima.com/cppsource/safebool.html
  typedef void (EString::*EStringIfHelperType)() const;
  void EStringIfHelper() const {}

 public:
  // constructors
  // creates a copy of the initial value.
  // if the initial value is null or invalid, or if memory allocation
  // fails, the string will be marked as invalid (i.e. "if (s)" will
  // be false).
  EString(const char *cstr = "");
  EString(const EString &str);
  EString(const __FlashEStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  EString(EString &&rval);
  EString(EStringSumHelper &&rval);
#endif
  explicit EString(char c);
  explicit EString(unsigned char, unsigned char base = 10);
  explicit EString(int, unsigned char base = 10);
  explicit EString(unsigned int, unsigned char base = 10);
  explicit EString(long, unsigned char base = 10);
  explicit EString(unsigned long, unsigned char base = 10);
  explicit EString(float, unsigned char decimalPlaces = 2);
  explicit EString(double, unsigned char decimalPlaces = 2);
  ~EString(void);

  // memory management
  // return true on success, false on failure (in which case, the string
  // is left unchanged).  reserve(0), if successful, will validate an
  // invalid string (i.e., "if (s)" will be true afterwards)
  unsigned char reserve(unsigned int size);
  inline unsigned int length(void) const { return len; }

  // creates a copy of the assigned value.  if the value is null or
  // invalid, or if the memory allocation fails, the string will be
  // marked as invalid ("if (s)" will be false).
  EString &operator=(const EString &rhs);
  EString &operator=(const char *cstr);
  EString &operator=(const __FlashEStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  EString &operator=(EString &&rval);
  EString &operator=(EStringSumHelper &&rval);
#endif

  // concatenate (works w/ built-in types)

  // returns true on success, false on failure (in which case, the string
  // is left unchanged).  if the argument is null or invalid, the
  // concatenation is considered unsucessful.
  unsigned char concat(const EString &str);
  unsigned char concat(const char *cstr);
  unsigned char concat(char c);
  unsigned char concat(unsigned char c);
  unsigned char concat(int num);
  unsigned char concat(unsigned int num);
  unsigned char concat(long num);
  unsigned char concat(unsigned long num);
  unsigned char concat(float num);
  unsigned char concat(double num);
  unsigned char concat(const __FlashEStringHelper *str);

  // if there's not enough memory for the concatenated value, the string
  // will be left unchanged (but this isn't signalled in any way)
  EString &operator+=(const EString &rhs) {
    concat(rhs);
    return (*this);
  }
  EString &operator+=(const char *cstr) {
    concat(cstr);
    return (*this);
  }
  EString &operator+=(char c) {
    concat(c);
    return (*this);
  }
  EString &operator+=(unsigned char num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(int num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(unsigned int num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(long num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(unsigned long num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(float num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(double num) {
    concat(num);
    return (*this);
  }
  EString &operator+=(const __FlashEStringHelper *str) {
    concat(str);
    return (*this);
  }

  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     const EString &rhs);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     const char *cstr);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs, char c);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     unsigned char num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs, int num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     unsigned int num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs, long num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     unsigned long num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs, float num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs, double num);
  friend EStringSumHelper &operator+(const EStringSumHelper &lhs,
                                     const __FlashEStringHelper *rhs);

  // comparison (only works w/ EStrings and "strings")
  operator EStringIfHelperType() const {
    return buffer ? &EString::EStringIfHelper : 0;
  }
  int compareTo(const EString &s) const;
  unsigned char equals(const EString &s) const;
  unsigned char equals(const char *cstr) const;
  unsigned char operator==(const EString &rhs) const { return equals(rhs); }
  unsigned char operator==(const char *cstr) const { return equals(cstr); }
  unsigned char operator!=(const EString &rhs) const { return !equals(rhs); }
  unsigned char operator!=(const char *cstr) const { return !equals(cstr); }
  unsigned char operator<(const EString &rhs) const;
  unsigned char operator>(const EString &rhs) const;
  unsigned char operator<=(const EString &rhs) const;
  unsigned char operator>=(const EString &rhs) const;
  unsigned char equalsIgnoreCase(const EString &s) const;
  unsigned char startsWith(const EString &prefix) const;
  unsigned char startsWith(const EString &prefix, unsigned int offset) const;
  unsigned char endsWith(const EString &suffix) const;

  // character acccess
  char charAt(unsigned int index) const;
  void setCharAt(unsigned int index, char c);
  char operator[](unsigned int index) const;
  char &operator[](unsigned int index);
  void getBytes(unsigned char *buf, unsigned int bufsize,
                unsigned int index = 0) const;
  void toCharArray(char *buf, unsigned int bufsize,
                   unsigned int index = 0) const {
    getBytes((unsigned char *)buf, bufsize, index);
  }
  const char *c_str() const { return buffer; }
  char *begin() { return buffer; }
  char *end() { return buffer + length(); }
  const char *begin() const { return c_str(); }
  const char *end() const { return c_str() + length(); }

  // search
  int indexOf(char ch) const;
  int indexOf(char ch, unsigned int fromIndex) const;
  int indexOf(const EString &str) const;
  int indexOf(const EString &str, unsigned int fromIndex) const;
  int lastIndexOf(char ch) const;
  int lastIndexOf(char ch, unsigned int fromIndex) const;
  int lastIndexOf(const EString &str) const;
  int lastIndexOf(const EString &str, unsigned int fromIndex) const;
  EString substring(unsigned int beginIndex) const {
    return substring(beginIndex, len);
  };
  EString substring(unsigned int beginIndex, unsigned int endIndex) const;

  // modification
  void replace(char find, char replace);
  void replace(const EString &find, const EString &replace);
  void remove(unsigned int index);
  void remove(unsigned int index, unsigned int count);
  void toLowerCase(void);
  void toUpperCase(void);
  void trim(void);

  // parsing/conversion
  long toInt(void) const;
  float toFloat(void) const;

 protected:
  char *buffer;           // the actual char array
  unsigned int capacity;  // the array length minus one (for the '\0')
  unsigned int len;       // the EString length (not counting the '\0')
 protected:
  void init(void);
  void invalidate(void);
  unsigned char changeBuffer(unsigned int maxStrLen);
  unsigned char concat(const char *cstr, unsigned int length);

  // copy and move
  EString &copy(const char *cstr, unsigned int length);
  EString &copy(const __FlashEStringHelper *pstr, unsigned int length);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  void move(EString &rhs);
#endif
};

class EStringSumHelper : public EString {
 public:
  EStringSumHelper(const EString &s) : EString(s) {}
  EStringSumHelper(const char *p) : EString(p) {}
  EStringSumHelper(char c) : EString(c) {}
  EStringSumHelper(unsigned char num) : EString(num) {}
  EStringSumHelper(int num) : EString(num) {}
  EStringSumHelper(unsigned int num) : EString(num) {}
  EStringSumHelper(long num) : EString(num) {}
  EStringSumHelper(unsigned long num) : EString(num) {}
  EStringSumHelper(float num) : EString(num) {}
  EStringSumHelper(double num) : EString(num) {}
};

#endif  // __cplusplus
#endif  // STRING_H
