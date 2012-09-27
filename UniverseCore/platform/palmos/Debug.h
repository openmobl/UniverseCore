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
 
#ifndef Debug_h
#define Debug_h

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void debugOutString(char *string);
extern void debugOutInt16(short integer);
extern void debugOutInt32(long integer);
extern void debugOutDouble(double number);
extern void debugOutFunctionStatus(const char *file, const char *func,
                               short line, long num, char *data);
                               
#define debugOutFunctionStatusShort(x,y) debugOutFunctionStatus(__FILE__, __FUNCTION__, __LINE__, \
                                                                x, y)

#ifdef __cplusplus
}
#endif

#if 0

#ifdef DEBUG
#define debugOutFunctionFormattedV0(x,y,z,a,b) debugOutFunctionStatus(a,x,b,z,(char *)y)
#else
#define debugOutFunctionFormattedV0
#endif

#endif

#define debugOutFunctionFormattedV0(x,y,z,a,b) debugOutFunctionStatus((char *)a,(char *)x,b,(int)z,(char *)y)


#endif
