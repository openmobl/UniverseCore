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

#include "config.h"
#include "Debug.h"

#include <DebugMgr.h>

#define kBufferSize     512

void debugOutString(char *string)
{
    DbgMessage(string);
}

void debugOutInt16(short integer)
{
    char    buffer[kBufferSize];
    
    StrPrintF(buffer, "%d", integer);
    
    DbgMessage(buffer);
}

void debugOutInt32(long integer)
{
    char    buffer[kBufferSize];
    
    StrPrintF(buffer, "%ld", integer);
    
    DbgMessage(buffer);
}

void debugOutDouble(double number)
{
    char    buffer[kBufferSize];
    
    StrPrintF(buffer, "%f", number);
    
    DbgMessage(buffer);
}

/**
 * Write out this format:
 *      Debug.c:53, debugOutFunctionStatus; 0 Clear
 */
void debugOutFunctionStatus(const char *file, const char *func,
                               short line, long num, char *data)
{
    debugOutString((char *)file);
    debugOutString(":");
    debugOutInt16(line);
    debugOutString(", ");
    debugOutString((char *)func);
    debugOutString("; ");
    
    debugOutInt32(num);
    debugOutString(" ");
    debugOutString(data);
    
    debugOutString("\r\n");
}
