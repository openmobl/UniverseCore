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
 
#ifndef PlatformBytes_h
#define PlatformBytes_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Default locations
#define CacheStorageLoc     "~/Library/Caches/UniverseCore/"
#define CookieStorageLoc    "~/Library/Cookies/UniverseCore/"

#define SAFETY_BUFFER_MAX   67108864ul // 64MB maximum buffer size

#define false           0
#define true            1

#define errClassStart   0

#define netErrSocketNotOpen     0x1208
#define netErrTimeout           0x1211
#define memErrNotEnoughSpace    0x0103
#define errNone                 0

#define ONE_KILOBYTE            1024

/* Compile time device configuration. */

#define NET_BLOCK_TIMEOUT		1 			// Used for looped network calls
#define NET_CONNECT_TIMEOUT		10			// Used for network connection calls, currently unused
#define MAX_URL_LEN 			(4 * ONE_KILOBYTE)
#define MAX_REDIRECT_COUNT		8			// This defines how many times the browser can be redirected in succession


// INTL_UNICODE_SUPPORT
// INTL_WESTERNEURO_SUPPORT
// Toggle these for Character Encoding support (platform/international/International.c)
#define INTL_NEXTSTEPLATIN_SUPPORT
#define INTL_ARABIC_SUPPORT
#define INTL_ROMAN_SUPPORT
#define INTL_CHINESE_SUPPORT
#define INTL_CYRILLIC_SUPPORT
#define INTL_HEBREW_SUPPORT
#define INTL_JAPANESE_SUPPORT
#define INTL_KOREAN_SUPPORT
#define INTL_THAI_SUPPORT
#define INTL_TURKISH_SUPPORT
#define INTL_VIETNAMESE_SUPPORT

#endif
