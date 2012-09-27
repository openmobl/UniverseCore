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

#include "PalmOSConfig.h"
#include "Strings.h"

#include <FloatMgr.h>
#include "MemMgr.h"
#include "PalmUtils.h"
#include <unix_stdarg.h>

unsigned long stringLen(const char *str)
{
#ifdef PALMOS_MOD_API
    unsigned long   len = 0;
    
    for (len = 0; str[len] != '\0'; len++);
    
    return len;
#else
    return StrLen(str);
#endif
}

unsigned long stringLenProtected(const char *str)
{
    if (!str) {
        return 0;
    }
    
    return stringLen(str);
}

char *stringDup(const char *str)
{
    char            *tempString = NULL;
    unsigned long   strLen      = 0;

    if (!str)
        return NULL;

    strLen = stringLen(str);
    tempString = memMgrChunkNew(strLen + 1);
    if (!tempString)
        return NULL;

    stringCopy(tempString, str);

    return tempString;
}

char *stringNDup(const char *str, unsigned long n)
{
    char            *tempString = NULL;

    if (!str)
        return NULL;

    tempString = memMgrChunkNew(n + 1);
    if (!tempString)
        return NULL;

    stringNCopy(tempString, str, n);
    tempString[n] = '\0';

    return tempString;
}

char *stringCopy(char *dest, const char *src)
{
#ifdef PALMOS_MOD_API
    int i = 0;
    
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    
    return dest;
#else
    return StrCopy(dest, src);
#endif
}

char *stringNCopy(char *dest, const char *src, unsigned long n)
{
#ifdef PALMOS_MOD_API
    int i = 0;
    
    for (i = 0; i < n; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    
    return dest;
#else
    return StrNCopy(dest, src, n);
#endif
}

char *stringChr(const char *str, int tok)
{
    return StrChr(str, tok);
}

char *stringStr(const char *str, const char *sub)
{
    return StrStr(str, sub);
}

char *stringRChr(const char *str, int tok)
{
    char    *next = NULL;
    char    *last = NULL;
    
    next = stringChr(str, tok);
    last = next;
    
    while (next) {
        next = stringChr(last + 1, tok);
        if (next) {
            last = next;
        }
    }
    
    return last;
}

/* TODO: Make a true AToF, not ascii to double */
double stringAToF(const char *str)
{
#ifdef PALMOS_MOD_API
    int     i, start, length, punctPos, exponent, ePos;
    double  result, sign, fractPart;
    char    *ePtr;

    result  = fractPart = 0.0;
    length  = punctPos = stringLen(str);
    ePtr    = stringStr(str, "e");
    ePos    = ePtr - str;

    start = 0;
    sign = 1.0;
    if (str[0] == '-') {
        sign = -1.0;
        start = 1;
    }

    for (i = start; i < length; i++)				// parse the string from left to right converting the integer part
    {
        if (!(str[i] == '.' || str[i] == 'e')) {
            if (palmIsDigit(str[i])) {
                result = result * 10.0 + (str[i] - 0x30);
            } else {
	           return 0.0;
            }
        } else {
            punctPos = i;
            break;
        }
    }

    if (str[punctPos] == '.')				// parse the string from the end to the '.' converting the fractional part
    {
        if (ePtr) {
            start = ePos;
        } else {
            start = length;
        }
        
        for (i=start-1; i>punctPos; i--) {
            if (palmIsDigit(str[i])) {
                fractPart = fractPart / 10.0 + (str[i] - 0x30);
            } else {
                return 0.0;
            }
        }
        
        result += fractPart / 10.0;
    }

    exponent = 0;
    if (ePtr) {
        exponent = stringAToI(ePtr+1);			// adjust the result for the exponent
    }

    return result * mathPower(10, exponent) * sign;
#else
    FlpCompDouble   fcd;
    
    FlpBufferAToF(&fcd.fd, str);
    
    return fcd.d;
#endif
}

unsigned long stringAToI(const char *str)
{
    return StrAToI(str);
}

/* TODO: Actually implement the proper functionality. */
long stringToLong(const char *nptr, char **endptr, int base)
{
    return StrAToI(nptr);
}

/* TODO: Actually implement the proper functionality. */
unsigned long stringToULong(const char *nptr, char **endptr, int base)
{
    return (unsigned long)StrAToI(nptr);
}

int stringCompare(const char *str1, const char *str2)
{
    return StrCompare(str1, str2);
}

int stringNCompare(const char *str1, const char *str2, unsigned long n)
{
    return StrNCompare(str1, str2, n);
}

int stringCompareCaseless(const char *str1, const char *str2)
{
    return StrCaselessCompare(str1, str2);
}

int stringNCompareCaseless(const char *str1, const char *str2, unsigned long n)
{
    return StrNCaselessCompare(str1, str2, n);
}

int stringNPrintF(char *str, unsigned long size, const char *format, ...)
{
    va_list args;
    char    *text;
    short   ret = -1;
    
    if (size == 0) {
        return 0;
    }
    
    text = memMgrChunkNew(size);
    if (!text) {
        return -1;
    }
    
    va_start(args, format);
    
    ret = StrVPrintF(text, format, args);
    
    va_end(args);
    
    memMgrChunkMove(str, text, size);
    
    memMgrChunkFree(text);
    
    return ret;
}

char *stringCombine(char *string1, char *string2)
{
    unsigned long  length1  = 0;
    unsigned long  length2  = 0;
    char            *out    = NULL;
    
    if (!string1 || !string2) {
        return string1;
    }
    
    length1 = stringLen(string1);
    length2 = stringLen(string2);
    
    out = memMgrChunkNew(length1 + length2 + 1);
    if (!out) {
        return string1;
    }
    
    stringPrintF(out, "%s%s", string1, string2);
    
    out[length1 + length2] = '\0';
    
    memMgrChunkFree(string1);
    
    return out;
}

void stringMakeInt(char *text)
{
    int i = 0;
                    
    while ((text[i] != '\0') &&
           (((text[i] >= '0') && (text[i] <= '9')) ||
            (text[i] == '.') || (text[i] == 'e') ||
            (text[i] == ',') || (text[i] == '\\'))) {
                i++;
    }
                    
    text[i] = '\0';
}

boolean stringReverse(unsigned char *dst, unsigned char *src)
{ 
    unsigned char   a, b, c, d;
    
    dst += stringLen((const char *)src);
    
    while(*src) {
        a = *src++;
        b = *src;
        c = src[1];
        d = src[2];
        
        if ((a >= 0) && (a < 128)) {
            *(--dst) = a;    //one char
        } else if ((a & 0xE0 ) == 0xC0) {
            *(--dst) = b;
            *(--dst) = a;
            
            src++;
            
            if((b & 0xC0) != 0x80)
                return false;
        } else if ((a & 0xF0) == 0xE0) {
            *(--dst) = c;
            *(--dst) = b;
            *(--dst) = a;
            
            src += 2;
            
            if (((b & 0xC0) != 0x80) ||
                ((c & 0xC0) != 0x80))
                    return false;
        } else if ((a & 0xF8) == 0xF0) {
            *(--dst) = d;
            *(--dst) = c;
            *(--dst) = b;
            *(--dst) = a;
            
            src += 3;
            
            if(((b & 0xC0) != 0x80) ||
               ((c & 0xC0) != 0x80) ||
               ((d & 0xC0) != 0x80))
                    return false;
        } else {
            return false;
        }
    }
    
    return true;
}

boolean stringIsWhitespaceNoLineEnd(int c)
{
    return ((c == ' ') || (c == '\t'));
}

boolean stringIsUpper(int c)
{
    return ((c >= 'A') && (c <= 'Z'));
}

boolean stringIsLower(int c)
{
    return ((c >= 'a') && (c <= 'z'));
}

boolean stringIsAlpha(int c)
{
    return (stringIsLower(c) || stringIsUpper(c));
}

boolean stringIsNumeric(int c)
{
    return ((c >= '0') && (c <= '9'));
}

boolean stringChrInRangeInclusive(int chr, int lower, int upper)
{
    boolean     isTrue = false;
    
    if ((chr >= lower) && (chr < upper))
        isTrue = true;
        
    if ((chr > lower) && (chr <= upper))
        isTrue = true;
    
    return isTrue;
}

boolean stringChrInRangeExclusive(int chr, int lower, int upper)
{
    if ((chr > lower) && (chr < upper))
        return true;
    
    return false;
}
