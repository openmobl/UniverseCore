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
 
#ifndef Cache_h
#define Cache_h

#include "config.h"
#include "Loader.h"

typedef struct CacheObject {
    FILE            *cacheFile;
    unsigned long   offset;
    unsigned long   size;
} CacheObject;

typedef struct CacheObject *CacheObjectPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern boolean cacheObjectIsCacheable(LoaderPtr loader);

extern int cacheSaveToCache(LoaderPtr loader);
extern int cacheLoadFromCache(LoaderPtr loader);

// These are internal functions. The "emulate" HTTP network traffic.
extern int cacheSendRequest(LoaderPtr loader);
extern int cacheReceiveResponse(LoaderPtr loader);

extern CacheObjectPtr loaderGetCache(LoaderPtr loader);
extern int loaderSetCache(CacheObjectPtr cache, LoaderPtr loader);

extern void cacheDestroyCache(LoaderPtr loader);
extern int cacheFlushCache(CoreGlobalsPtr globals);

#ifdef __cplusplus
}
#endif

#endif
