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

int palmIsDigit(int c)
{
    return (c >= '0' && c <= '9');
}

int mathPower(int base, int exp)
{
    int i       = 0;
    int result  = 0;
    
    for (i = 1; i < (exp + 1); i++) {
        result *= result;
    }
    
    return result;
}

/*Char *StrDup(Char *string)
{
    Char    *tempString = NULL;
    UInt16    stringLen;

    if (!string)
        return NULL;

    stringLen = StrLen(string);
    tempString = memMgrChunkNew(stringLen + 1);
    if (!tempString)
        return NULL;

    StrCopy(tempString, string);

    return tempString;
}*/

// Like StrCat
/*Char *StrCombine(Char *string1, Char *string2)
{
    UInt32  length1 = 0;
    UInt32  length2 = 0;
    Char    *out    = NULL;
    
    if (!string1 || !string2) {
        return string1;
    }
    
    length1 = StrLen(string1);
    length2 = StrLen(string2);
    
    out = memMgrChunkNew(length1 + length2 + 1);
    if (!out) {
        return string1;
    }
    
    StrPrintF(out, "%s%s", string1, string2);
    
    out[length1 + length2] = '\0';
    
    memMgrChunkFree(string1);
    
    return out;
}*/


