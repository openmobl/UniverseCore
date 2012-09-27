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
 
#ifndef CorePrefs_h
#define CorePrefs_h

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned long corePrefsGetTimeout(CoreGlobalsPtr globals);
extern char *corePrefGetCacheStorageLoc(CoreGlobalsPtr globals);
extern char *corePrefGetCookieStorageLoc(CoreGlobalsPtr globals);
extern boolean corePrefGetCacheEnabled(CoreGlobalsPtr globals);
extern boolean corePrefGetCookiesEnabled(CoreGlobalsPtr globals);

extern void corePrefsSetTimeout(CoreGlobalsPtr globals, unsigned long timeout);
extern void corePrefSetCacheStorageLoc(CoreGlobalsPtr globals, char *location);
extern void corePrefSetCookieStorageLoc(CoreGlobalsPtr globals, char *location);
extern void corePrefSetCacheEnabled(CoreGlobalsPtr globals, boolean flag);
extern void corePrefSetCookiesEnabled(CoreGlobalsPtr globals, boolean flag);

#ifdef __cplusplus
}
#endif

#endif
