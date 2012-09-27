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

#include "LinuxConfig.h"
#include "Strings.h"

#include "MemMgr.h"

unsigned long stringLen(const char *str)
{
    return strlen(str);
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
    return (str)?strdup(str):NULL;
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
    return strcpy(dest, src);
}

char *stringNCopy(char *dest, const char *src, unsigned long n)
{
    return strncpy(dest, src, n);
}

char *stringChr(const char *str, int tok)
{
    return strchr(str, tok);
}

char *stringStr(const char *str, const char *sub)
{
    return strstr(str, sub);
}

char *stringRChr(const char *str, int tok)
{
    return strrchr(str, tok);
}

double stringAToF(const char *str)
{
    return atof(str);
}

unsigned long stringAToI(const char *str)
{
    return atoi(str);
}

long stringToLong(const char *nptr, char **endptr, int base)
{
    return strtol(nptr, endptr, base);
}

unsigned long stringToULong(const char *nptr, char **endptr, int base)
{
    return strtoul(nptr, endptr, base);
}

int stringCompare(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

int stringNCompare(const char *str1, const char *str2, unsigned long n)
{
    return strncmp(str1, str2, n);
}

int stringCompareCaseless(const char *str1, const char *str2)
{
    return strcasecmp(str1, str2);
}

int stringNCompareCaseless(const char *str1, const char *str2, unsigned long n)
{
    return strncasecmp(str1, str2, n);
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

boolean stringReverse(unsigned char *dst, unsigned char *src)
{ 
    unsigned char   a, b, c, d;
    
    dst += stringLen((char *)src);
    
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
