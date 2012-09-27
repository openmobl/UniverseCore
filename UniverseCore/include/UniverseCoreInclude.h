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
#ifndef UniverseCoreInclude_h
#define UniverseCoreInclude_h

#include "PlatformBytes.h"

// Remove
//#define CONFIG_HTTP_OWN_URL // The HTTP code uses an internal URL, eventually the loader will "own" the URL

#ifndef DOMString
typedef char *DOMString;
#endif

#ifndef DOMTimeStamp
typedef unsigned long long DOMTimeStamp;
#endif

#ifndef boolean
typedef int boolean;
#endif

// The application will set this stuff through the API and this will get passed down through the layers
// TODO: Check for invalid values!!!!!!
typedef struct CoreGlobalsType {
    struct CorePrefsType {
        struct LoaderPref {
            char                *cacheLocation;
            char                *cookiesLocation;
            unsigned long       timeout;
            unsigned long       cacheSize;
            int                 cookiePref; // CookieNotifyEnum
            
            boolean             cookiesEnabled;
            boolean             cacheEnabled;
        } loader;
    } prefs;
    
} CoreGlobalsType;

typedef CoreGlobalsType *CoreGlobalsPtr;


#include "Errors.h"
#include "File.h"
#include "StdUtils.h"
#include "Strings.h"


#endif
