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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config.h>

#include <Loader.h>
#include <WebData.h>
#include <URL.h>

#define LOADERTEST_VER  1.0

#define LOADERTEST_HTTP_MAJ     1
#define LOADERTEST_HTTP_MIN     0

#define errAssert(x,y,z)    if (x) {                \
                                fprintf(stderr, y); \
                                goto z;             \
                            }

#define LOADER_USER_AGENT   "LoaderTester/1.0 UniverseCore/0.5"
#define CACHE_STORE         "~/Library/Caches/UniverseCore/LoaderTester/"

int testerNotifyCallbackPtr(LoaderCallbackPtr data, void *loader)
{
    switch (data->state) {
        case loaderReceiveHeaderState:
            printf("Receiving Header...\n");
            break;
        case loaderHeaderDoneState:
            printf("Headers Done\n");
            break;
        case loaderReceiveDataState:
            printf("Receiving Data...\n");
            break;
        case loaderDataDoneState:
            printf("Data Done\n");
            break;
        default:
            break;
    }
    
    return 0;
}

int testerStateChangeCallbackPtr(LoaderStateEnum newState, void *loader)
{
    switch (newState) {
        case loaderStateRequesting:
            printf("Loader: Requesting...\n");
            break;
        case loaderStateReceiving:
            printf("Loader: Receiving...\n");
            break;
        case loaderStateDone:
            printf("Loader: Done\n");
            break;
        case loaderStateError:
            printf("Loader: ERROR!!  %ld\n", loaderGetError((LoaderPtr)loader));
            break;
        default:
            break;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    int             error       = 0;
    char            *url        = NULL;
    URLPtr          lastURL     = NULL;
    LoaderPtr       loader      = NULL;
    CoreGlobalsType globals;
    WebDataType     webData;
    
    memMgrChunkSet(&webData, 0, sizeof(WebDataType));
    memMgrChunkSet(&globals, 0, sizeof(CoreGlobalsType));
    
    globals.prefs.loader.timeout       = 600;
    globals.prefs.loader.cacheEnabled  = true;
    globals.prefs.loader.cacheLocation = CACHE_STORE;
    
    printf("UniverseCore Loader Tester v%.3f\n", LOADERTEST_VER);
    
    url = malloc(2046);
    if (!url)
        goto cleanup;
    
    printf("Please enter a URL to test: ");
    fscanf(stdin, "%s", url);
    
request:
    printf("Requesting %s\n", url);

    loader = loaderNewLoaderFromURL(url, loaderHTTP, testerNotifyCallbackPtr,
                                    testerStateChangeCallbackPtr, &webData, HTTP_GET, 
                                    &globals, lastURL, &error);
    if (!loader) {
        printf("Error: %d\n", error);
        
        goto cleanup;
    }
    
    loaderSetHTTPVersion(LOADERTEST_HTTP_MAJ, LOADERTEST_HTTP_MIN, loader);
    loaderSetUserAgent(LOADER_USER_AGENT, loader);
    
    loaderStartHeaders(loader);
    loaderFinishHeaders(loader);
    
    loaderStartLoader(loader); // Starts the engine
    while (!loaderIsDone(loader) && !loaderHasError(loader)) {
        loaderStateLoop(loader);
    }
    
    if (loaderGetRedirectURL(loader)) {
        int err = 0;
        
        if (lastURL)
            urlDestroyURL(lastURL);
        lastURL = urlParseURL(url, &err);
        if (err)
            goto cleanup;
        free(url);
        
        url = stringDup((char *)loaderGetRedirectURL(loader));
        
        loaderDestroyLoader(0, &loader);
        
        memMgrChunkSet(&webData, 0, sizeof(WebDataType));
        
        goto request;
    }
    
    loaderDestroyLoader(0, &loader);
    
cleanup:
    if (loader)
        free(loader);
    if (lastURL)
        urlDestroyURL(lastURL);

    return 0;
}
