/*
 * @file   echar.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  Character utility functions.
 */

#ifndef ECHAR_H
#define ECHAR_H

#include <ctype.h>

namespace EChar {
bool isAlphaNumeric(int c) { return (isalnum(c) == 0 ? false : true); }
bool isAlpha(int c) { return (isalpha(c) == 0 ? false : true); }
bool isAscii(int c) {
  /*  return ( isascii(c) == 0 ? false : true); */
  return ((c & ~0x7f) != 0 ? false : true);
}
bool isWhitespace(int c) { return (isblank(c) == 0 ? false : true); }
bool isControl(int c) { return (iscntrl(c) == 0 ? false : true); }
bool isDigit(int c) { return (isdigit(c) == 0 ? false : true); }
bool isGraph(int c) { return (isgraph(c) == 0 ? false : true); }
bool isLowerCase(int c) { return (islower(c) == 0 ? false : true); }
bool isPrintable(int c) { return (isprint(c) == 0 ? false : true); }
bool isPunct(int c) { return (ispunct(c) == 0 ? false : true); }
bool isSpace(int c) { return (isspace(c) == 0 ? false : true); }
bool isUpperCase(int c) { return (isupper(c) == 0 ? false : true); }
bool isHexadecimalDigit(int c) { return (isxdigit(c) == 0 ? false : true); }
int toAscii(int c) {
  /*  return toascii (c); */
  return (c & 0x7f);
}
int toLowerCase(int c) { return tolower(c); }
int toUpperCase(int c) { return toupper(c); }
}  // namespace EChar

#endif  // ECHAR_H
