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
#include "CorePrefs.h"

void corePrefsSetTimeout(CoreGlobalsPtr globals, unsigned long timeout)
{
    if (!globals)
        return;

    globals->prefs.loader.timeout = timeout;
}

unsigned long corePrefsGetTimeout(CoreGlobalsPtr globals)
{
    if (!globals)
        return 0;
    
    return globals->prefs.loader.timeout;
}

// TODO: Copy data into a fixed buffer
void corePrefSetCacheStorageLoc(CoreGlobalsPtr globals, char *location)
{
    if (!globals)
        return;

    globals->prefs.loader.cacheLocation = location;
}

char *corePrefGetCacheStorageLoc(CoreGlobalsPtr globals)
{
    if (!globals || !globals->prefs.loader.cacheLocation)
        return CacheStorageLoc;
    
    return globals->prefs.loader.cacheLocation;
}

void corePrefSetCookieStorageLoc(CoreGlobalsPtr globals, char *location)
{
    if (!globals)
        return;

    globals->prefs.loader.cookiesLocation = location;
}

char *corePrefGetCookieStorageLoc(CoreGlobalsPtr globals)
{
    if (!globals || !globals->prefs.loader.cookiesLocation)
        return CookieStorageLoc;
    
    return globals->prefs.loader.cookiesLocation;
}

void corePrefSetCacheEnabled(CoreGlobalsPtr globals, boolean flag)
{
    if (!globals)
        return;

    globals->prefs.loader.cacheEnabled = flag;
}

boolean corePrefGetCacheEnabled(CoreGlobalsPtr globals)
{
    if (!globals)
        return false;
    
    return globals->prefs.loader.cacheEnabled;
}

void corePrefSetCookiesEnabled(CoreGlobalsPtr globals, boolean flag)
{
    if (!globals)
        return;

    globals->prefs.loader.cookiesEnabled = flag;
}

boolean corePrefGetCookiesEnabled(CoreGlobalsPtr globals)
{
    if (!globals)
        return false;
    
    return globals->prefs.loader.cookiesEnabled;
}
