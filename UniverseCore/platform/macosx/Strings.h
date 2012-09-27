/********************************************************************************
 * UniverseCore Web Core                                                        *
 * Copyright (c) 2007 OpenMobl Systems                                          *
 * Copyright (c) 2006-2007 Donald C. Kirker                                     *
 *                                                                              *
 * http://www.openmobl.com/                                                     *
 * dev-support@openmobl.com                                                     *
 *                                                                              *
 * This library is free software; you can redistribute it and/or                *
 * modify it under the terms of the GNU Lesser General Public                   *
 * License as published by the Free Software Foundation; either                 *
 * version 2.1 of the License, or (at your option) any later version.           *
 *                                                                              *
 * This library is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            *
 * Lesser General Public License for more details.                              *
 *                                                                              *
 * You should have received a copy of the GNU Lesser General Public             *
 * License along with this library; if not, write to the Free Software          *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA*
 *                                                                              *
 ********************************************************************************/
 
#ifndef Strings_h
#define Strings_h

#include "MacOSXConfig.h"

extern unsigned long stringLen(const char *str);
extern unsigned long stringLenProtected(const char *str);
extern char *stringDup(const char *str);
extern char *stringNDup(const char *str, unsigned long n);
extern char *stringCopy(char *dest, const char *src);
extern char *stringNCopy(char *dest, const char *src, unsigned long n);
extern char *stringChr(const char *str, int tok);
extern char *stringStr(const char *str, const char *sub);
extern char *stringRChr(const char *str, int tok);
extern double stringAToF(const char *str);
extern unsigned long stringAToI(const char *str);
extern long stringToLong(const char *nptr, char **endptr, int base);
extern unsigned long stringToULong(const char *nptr, char **endptr, int base);
extern int stringCompare(const char *str1, const char *str2);
extern int stringNCompare(const char *str1, const char *str2, unsigned long n);
extern int stringCompareCaseless(const char *str1, const char *str2);
extern int stringNCompareCaseless(const char *str1, const char *str2, unsigned long n);
extern char *stringCombine(char *string1, char *string2);
extern boolean stringReverse(unsigned char *dst, unsigned char *src);

//extern int stringNPrintF(char *str, unsigned long size, const char *format, ...);
#define stringPrintF    sprintf
#define stringNPrintF   snprintf

extern boolean stringIsWhitespaceNoLineEnd(int c);
extern boolean stringIsUpper(int c);
extern boolean stringIsLower(int c);
extern boolean stringIsAlpha(int c);
extern boolean stringIsNumeric(int c);

extern boolean stringChrInRangeInclusive(int chr, int lower, int upper);
extern boolean stringChrInRangeExclusive(int chr, int lower, int upper);

#endif
