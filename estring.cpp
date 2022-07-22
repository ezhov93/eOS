/*
 * @file  estring.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  EString library
 */

#include "estring.h"

#include "itoa.h"
#include "stdio.h"

#define PGM_P const char *
#define strcpy_P(dest, src) strcpy((dest), (src))
#define strlen_P(a) strlen((a))

char *dtostrf(double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

EString::EString(const char *cstr) {
  init();
  if (cstr) copy(cstr, strlen(cstr));
}

EString::EString(const EString &value) {
  init();
  *this = value;
}

EString::EString(const __FlashEStringHelper *pstr) {
  init();
  *this = pstr;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
EString::EString(EString &&rval) {
  init();
  move(rval);
}
EString::EString(EStringSumHelper &&rval) {
  init();
  move(rval);
}
#endif

EString::EString(char c) {
  init();
  char buf[2];
  buf[0] = c;
  buf[1] = 0;
  *this = buf;
}

EString::EString(unsigned char value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned char)];
  utoa(value, buf, base);
  *this = buf;
}

EString::EString(int value, unsigned char base) {
  init();
  char buf[2 + 8 * sizeof(int)];
  itoa(value, buf, base);
  *this = buf;
}

EString::EString(unsigned int value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned int)];
  utoa(value, buf, base);
  *this = buf;
}

EString::EString(long value, unsigned char base) {
  init();
  char buf[2 + 8 * sizeof(long)];
  ltoa(value, buf, base);
  *this = buf;
}

EString::EString(unsigned long value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned long)];
  ultoa(value, buf, base);
  *this = buf;
}

EString::EString(float value, unsigned char decimalPlaces) {
  init();
  char buf[33];
  *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

EString::EString(double value, unsigned char decimalPlaces) {
  init();
  char buf[33];
  *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

EString::~EString() { free(buffer); }

/*********************************************/
/*  Memory Management                        */
/*********************************************/

inline void EString::init(void) {
  buffer = NULL;
  capacity = 0;
  len = 0;
}

void EString::invalidate(void) {
  if (buffer) free(buffer);
  buffer = NULL;
  capacity = len = 0;
}

unsigned char EString::reserve(unsigned int size) {
  if (buffer && capacity >= size) return 1;
  if (changeBuffer(size)) {
    if (len == 0) buffer[0] = 0;
    return 1;
  }
  return 0;
}

unsigned char EString::changeBuffer(unsigned int maxStrLen) {
  char *newbuffer = (char *)realloc(buffer, maxStrLen + 1);
  if (newbuffer) {
    buffer = newbuffer;
    capacity = maxStrLen;
    return 1;
  }
  return 0;
}

/*********************************************/
/*  Copy and Move                            */
/*********************************************/

EString &EString::copy(const char *cstr, unsigned int length) {
  if (!reserve(length)) {
    invalidate();
    return *this;
  }
  len = length;
  strcpy(buffer, cstr);
  return *this;
}

EString &EString::copy(const __FlashEStringHelper *pstr, unsigned int length) {
  if (!reserve(length)) {
    invalidate();
    return *this;
  }
  len = length;
  strcpy_P(buffer, (PGM_P)pstr);
  return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
void EString::move(EString &rhs) {
  if (buffer) {
    if (rhs && capacity >= rhs.len) {
      strcpy(buffer, rhs.buffer);
      len = rhs.len;
      rhs.len = 0;
      return;
    } else {
      free(buffer);
    }
  }
  buffer = rhs.buffer;
  capacity = rhs.capacity;
  len = rhs.len;
  rhs.buffer = NULL;
  rhs.capacity = 0;
  rhs.len = 0;
}
#endif

EString &EString::operator=(const EString &rhs) {
  if (this == &rhs) return *this;

  if (rhs.buffer)
    copy(rhs.buffer, rhs.len);
  else
    invalidate();

  return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
EString &EString::operator=(EString &&rval) {
  if (this != &rval) move(rval);
  return *this;
}

EString &EString::operator=(EStringSumHelper &&rval) {
  if (this != &rval) move(rval);
  return *this;
}
#endif

EString &EString::operator=(const char *cstr) {
  if (cstr)
    copy(cstr, strlen(cstr));
  else
    invalidate();

  return *this;
}

EString &EString::operator=(const __FlashEStringHelper *pstr) {
  if (pstr)
    copy(pstr, strlen_P((PGM_P)pstr));
  else
    invalidate();

  return *this;
}

/*********************************************/
/*  concat                                   */
/*********************************************/

unsigned char EString::concat(const EString &s) {
  return concat(s.buffer, s.len);
}

unsigned char EString::concat(const char *cstr, unsigned int length) {
  unsigned int newlen = len + length;
  if (!cstr) return 0;
  if (length == 0) return 1;
  if (!reserve(newlen)) return 0;
  strcpy(buffer + len, cstr);
  len = newlen;
  return 1;
}

unsigned char EString::concat(const char *cstr) {
  if (!cstr) return 0;
  return concat(cstr, strlen(cstr));
}

unsigned char EString::concat(char c) {
  char buf[2];
  buf[0] = c;
  buf[1] = 0;
  return concat(buf, 1);
}

unsigned char EString::concat(unsigned char num) {
  char buf[1 + 3 * sizeof(unsigned char)];
  itoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

unsigned char EString::concat(int num) {
  char buf[2 + 3 * sizeof(int)];
  itoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

unsigned char EString::concat(unsigned int num) {
  char buf[1 + 3 * sizeof(unsigned int)];
  utoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

unsigned char EString::concat(long num) {
  char buf[2 + 3 * sizeof(long)];
  ltoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

unsigned char EString::concat(unsigned long num) {
  char buf[1 + 3 * sizeof(unsigned long)];
  ultoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

unsigned char EString::concat(float num) {
  char buf[20];
  char *string = dtostrf(num, 4, 2, buf);
  return concat(string, strlen(string));
}

unsigned char EString::concat(double num) {
  char buf[20];
  char *string = dtostrf(num, 4, 2, buf);
  return concat(string, strlen(string));
}

unsigned char EString::concat(const __FlashEStringHelper *str) {
  if (!str) return 0;
  int length = strlen_P((const char *)str);
  if (length == 0) return 1;
  unsigned int newlen = len + length;
  if (!reserve(newlen)) return 0;
  strcpy_P(buffer + len, (const char *)str);
  len = newlen;
  return 1;
}

/*********************************************/
/*  Concatenate                              */
/*********************************************/

EStringSumHelper &operator+(const EStringSumHelper &lhs, const EString &rhs) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(rhs.buffer, rhs.len)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, const char *cstr) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!cstr || !a.concat(cstr, strlen(cstr))) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, char c) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(c)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, unsigned char num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, int num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, unsigned int num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, long num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, unsigned long num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, float num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs, double num) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(num)) a.invalidate();
  return a;
}

EStringSumHelper &operator+(const EStringSumHelper &lhs,
                            const __FlashEStringHelper *rhs) {
  EStringSumHelper &a = const_cast<EStringSumHelper &>(lhs);
  if (!a.concat(rhs)) a.invalidate();
  return a;
}

/*********************************************/
/*  Comparison                               */
/*********************************************/

int EString::compareTo(const EString &s) const {
  if (!buffer || !s.buffer) {
    if (s.buffer && s.len > 0) return 0 - *(unsigned char *)s.buffer;
    if (buffer && len > 0) return *(unsigned char *)buffer;
    return 0;
  }
  return strcmp(buffer, s.buffer);
}

unsigned char EString::equals(const EString &s2) const {
  return (len == s2.len && compareTo(s2) == 0);
}

unsigned char EString::equals(const char *cstr) const {
  if (len == 0) return (cstr == NULL || *cstr == 0);
  if (cstr == NULL) return buffer[0] == 0;
  return strcmp(buffer, cstr) == 0;
}

unsigned char EString::operator<(const EString &rhs) const {
  return compareTo(rhs) < 0;
}

unsigned char EString::operator>(const EString &rhs) const {
  return compareTo(rhs) > 0;
}

unsigned char EString::operator<=(const EString &rhs) const {
  return compareTo(rhs) <= 0;
}

unsigned char EString::operator>=(const EString &rhs) const {
  return compareTo(rhs) >= 0;
}

unsigned char EString::equalsIgnoreCase(const EString &s2) const {
  if (this == &s2) return 1;
  if (len != s2.len) return 0;
  if (len == 0) return 1;
  const char *p1 = buffer;
  const char *p2 = s2.buffer;
  while (*p1) {
    if (tolower(*p1++) != tolower(*p2++)) return 0;
  }
  return 1;
}

unsigned char EString::startsWith(const EString &s2) const {
  if (len < s2.len) return 0;
  return startsWith(s2, 0);
}

unsigned char EString::startsWith(const EString &s2,
                                  unsigned int offset) const {
  if (offset > len - s2.len || !buffer || !s2.buffer) return 0;
  return strncmp(&buffer[offset], s2.buffer, s2.len) == 0;
}

unsigned char EString::endsWith(const EString &s2) const {
  if (len < s2.len || !buffer || !s2.buffer) return 0;
  return strcmp(&buffer[len - s2.len], s2.buffer) == 0;
}

/*********************************************/
/*  Character Access                         */
/*********************************************/

char EString::charAt(unsigned int loc) const { return operator[](loc); }

void EString::setCharAt(unsigned int loc, char c) {
  if (loc < len) buffer[loc] = c;
}

char &EString::operator[](unsigned int index) {
  static char dummy_writable_char;
  if (index >= len || !buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return buffer[index];
}

char EString::operator[](unsigned int index) const {
  if (index >= len || !buffer) return 0;
  return buffer[index];
}

void EString::getBytes(unsigned char *buf, unsigned int bufsize,
                       unsigned int index) const {
  if (!bufsize || !buf) return;
  if (index >= len) {
    buf[0] = 0;
    return;
  }
  unsigned int n = bufsize - 1;
  if (n > len - index) n = len - index;
  strncpy((char *)buf, buffer + index, n);
  buf[n] = 0;
}

/*********************************************/
/*  Search                                   */
/*********************************************/

int EString::indexOf(char c) const { return indexOf(c, 0); }

int EString::indexOf(char ch, unsigned int fromIndex) const {
  if (fromIndex >= len) return -1;
  const char *temp = strchr(buffer + fromIndex, ch);
  if (temp == NULL) return -1;
  return temp - buffer;
}

int EString::indexOf(const EString &s2) const { return indexOf(s2, 0); }

int EString::indexOf(const EString &s2, unsigned int fromIndex) const {
  if (fromIndex >= len) return -1;
  const char *found = strstr(buffer + fromIndex, s2.buffer);
  if (found == NULL) return -1;
  return found - buffer;
}

int EString::lastIndexOf(char theChar) const {
  return lastIndexOf(theChar, len - 1);
}

int EString::lastIndexOf(char ch, unsigned int fromIndex) const {
  if (fromIndex >= len) return -1;
  char tempchar = buffer[fromIndex + 1];
  buffer[fromIndex + 1] = '\0';
  char *temp = strrchr(buffer, ch);
  buffer[fromIndex + 1] = tempchar;
  if (temp == NULL) return -1;
  return temp - buffer;
}

int EString::lastIndexOf(const EString &s2) const {
  return lastIndexOf(s2, len - s2.len);
}

int EString::lastIndexOf(const EString &s2, unsigned int fromIndex) const {
  if (s2.len == 0 || len == 0 || s2.len > len) return -1;
  if (fromIndex >= len) fromIndex = len - 1;
  int found = -1;
  for (char *p = buffer; p <= buffer + fromIndex; p++) {
    p = strstr(p, s2.buffer);
    if (!p) break;
    if ((unsigned int)(p - buffer) <= fromIndex) found = p - buffer;
  }
  return found;
}

EString EString::substring(unsigned int left, unsigned int right) const {
  if (left > right) {
    unsigned int temp = right;
    right = left;
    left = temp;
  }
  EString out;
  if (left >= len) return out;
  if (right > len) right = len;
  char temp = buffer[right];  // save the replaced character
  buffer[right] = '\0';
  out = buffer + left;   // pointer arithmetic
  buffer[right] = temp;  // restore character
  return out;
}

/*********************************************/
/*  Modification                             */
/*********************************************/

void EString::replace(char find, char replace) {
  if (!buffer) return;
  for (char *p = buffer; *p; p++) {
    if (*p == find) *p = replace;
  }
}

void EString::replace(const EString &find, const EString &replace) {
  if (len == 0 || find.len == 0) return;
  int diff = replace.len - find.len;
  char *readFrom = buffer;
  char *foundAt;
  if (diff == 0) {
    while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      memcpy(foundAt, replace.buffer, replace.len);
      readFrom = foundAt + replace.len;
    }
  } else if (diff < 0) {
    char *writeTo = buffer;
    while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      unsigned int n = foundAt - readFrom;
      memcpy(writeTo, readFrom, n);
      writeTo += n;
      memcpy(writeTo, replace.buffer, replace.len);
      writeTo += replace.len;
      readFrom = foundAt + find.len;
      len += diff;
    }
    strcpy(writeTo, readFrom);
  } else {
    unsigned int size = len;  // compute size needed for result
    while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      readFrom = foundAt + find.len;
      size += diff;
    }
    if (size == len) return;
    if (size > capacity && !changeBuffer(size)) return;  // XXX: tell user!
    int index = len - 1;
    while (index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
      readFrom = buffer + index + find.len;
      memmove(readFrom + diff, readFrom, len - (readFrom - buffer));
      len += diff;
      buffer[len] = 0;
      memcpy(buffer + index, replace.buffer, replace.len);
      index--;
    }
  }
}

void EString::remove(unsigned int index) {
  // Pass the biggest integer as the count. The remove method
  // below will take care of truncating it at the end of the
  // string.
  remove(index, (unsigned int)-1);
}

void EString::remove(unsigned int index, unsigned int count) {
  if (index >= len) {
    return;
  }
  if (count <= 0) {
    return;
  }
  if (count > len - index) {
    count = len - index;
  }
  char *writeTo = buffer + index;
  len = len - count;
  strncpy(writeTo, buffer + index + count, len - index);
  buffer[len] = 0;
}

void EString::toLowerCase(void) {
  if (!buffer) return;
  for (char *p = buffer; *p; p++) {
    *p = tolower(*p);
  }
}

void EString::toUpperCase(void) {
  if (!buffer) return;
  for (char *p = buffer; *p; p++) {
    *p = toupper(*p);
  }
}

void EString::trim(void) {
  if (!buffer || len == 0) return;
  char *begin = buffer;
  while (isspace(*begin)) begin++;
  char *end = buffer + len - 1;
  while (isspace(*end) && end >= begin) end--;
  len = end + 1 - begin;
  if (begin > buffer) memcpy(buffer, begin, len);
  buffer[len] = 0;
}

/*********************************************/
/*  Parsing / Conversion                     */
/*********************************************/

long EString::toInt(void) const {
  if (buffer) return atol(buffer);
  return 0;
}

float EString::toFloat(void) const {
  if (buffer) return float(atof(buffer));
  return 0;
}
