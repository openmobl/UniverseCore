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
#include "Cache.h"

#include "CorePrefs.h"
#include "File.h"
#include "HTTP.h"
#include "Loader.h"
#include "PlatformTime.h"
//#include "WAP.h"


// This is the HTTP 1.1 heaer that we save to the cache
#define CACHE_HTTP_200  "HTTP/1.1 200 OK"

static char *cacheGetCacheControlHeader(LoaderPtr loader);
static char *cacheGetExpiresHeader(LoaderPtr loader);
static char *cacheGetPragmaHeader(LoaderPtr loader);
static void cacheAddToBuf(char **buf, unsigned long *bufSize, char *str, unsigned long len);
static char *cacheCreateEncodedPath(char *urlPath);
static char *cacheCreateEncodedURL(LoaderPtr loader, int *err);
static unsigned long cacheFileReadExpiresHeader(FILE *cacheFile, int *err);
static boolean cacheIsExpired(char *expires);
static boolean cacheControlIsCacheable(char *cacheControl);
static unsigned char *cacheHTTPGetOffset(HTTPRequestPtr request);
static unsigned long cacheHTTPGetSizeLeft(HTTPRequestPtr request);
static void cacheHTTPUpdateSize(unsigned long size, HTTPRequestPtr request);


boolean cacheObjectIsCacheable(LoaderPtr loader)
{
    LoaderProtocolEnum  proto           = loaderNONE;
    WebDataPtr          webData         = NULL;
    char                *cacheControl   = NULL;
    char                *pragma         = NULL;
    char                *expires        = NULL;
    
    if (!loader)
        return false;
    
    proto           = loaderGetProtocol(loader);
    webData         = loaderGetWebData(loader);
    cacheControl    = cacheGetCacheControlHeader(loader);
    pragma          = cacheGetPragmaHeader(loader);
    expires         = cacheGetExpiresHeader(loader);
    
    // Did we even download anything??
    if (!webData) {
        debugOutFunctionStatusShort(0, "No webData");
        return false;
    }
    
    if (!corePrefGetCacheEnabled(loader->globals)) {
        debugOutFunctionStatusShort(0, "Prefs says no caching");
        return false;
    }
    
    // Is it HTTP or WAP?
    if ((proto != loaderHTTP) && (proto != loaderWAP)) {
        debugOutFunctionStatusShort(0, "Not HTTP based (HTTP or WAP)");
        return false;
    }
    
    // Did we have to authenticate? TODO: Is this a necessary check?
    
    
    // Is it HTTP OK?
    if ((loaderGetResponseCode(loader) != 200) && (loaderGetResponseCode(loader) != 203)) {
        debugOutFunctionStatusShort(0, "Not 200 or 203");
        return false;
    }
    
    // Is it POST or GET?
    if (loaderGetRequestType(loader) != HTTP_GET) {
        debugOutFunctionStatusShort(0, "Not GET");
        return false;
    }
    
    // Is is a page or an embeded image?
    if (!webDataIsPageMIME(webData) && !(webDataIsImageMIME(webData) && webDataIsEmbeded(webData)) &&
        !(webDataIsCssMIME(webData) && webDataIsEmbeded(webData)) &&
        !(webDataIsJavaScriptMIME(webData) && webDataIsEmbeded(webData))) {
        debugOutFunctionStatusShort(0, "Not a page or embeded image or embedded css");
        debugOutFunctionStatusShort(0, webDataGetContentType(webData));
        return false;
    }
    
    // Does this have a no-cache header?
    if (cacheControl && !cacheControlIsCacheable(cacheControl)) {
        debugOutFunctionStatusShort(0, "Cache-Control says do not cache");
        return false;
    }
    
    // Is there a Pragma: no-cache?
    if (pragma && !stringCompareCaseless(pragma, "no-cache")) {
        debugOutFunctionStatusShort(0, "Pragma: no-cache");
        return false;
    }
    
    // Is the Expires header from the past?
    if (cacheIsExpired(expires)) {
        debugOutFunctionStatusShort(0, "Expires: Previously");
        return false;
    }
    
    // TODO: Markup says no-cache
    
    // Then we can cache
    
    return true;
}

static char *cacheGetCacheControlHeader(LoaderPtr loader)
{
    return loaderFindHeader("Cache-Control", loader);
}

static char *cacheGetExpiresHeader(LoaderPtr loader)
{
    return loaderFindHeader("Expires", loader);
}

static char *cacheGetPragmaHeader(LoaderPtr loader)
{
    return loaderFindHeader("Pragma", loader);
}

static boolean cacheIsExpired(char *expires)
{
    long    seconds = 0;
    
    if (!expires)
        return false;
    
    seconds = timeStringToSeconds(expires);
    
    return (seconds <= timeSinceEpoch())?true:false;
}

static boolean cacheControlIsCacheable(char *cacheControl)
{
    char    *noCache    = NULL;
    char    *noStore    = NULL;
    char    *maxAge     = NULL;
    
    if (!cacheControl) {
        debugOutFunctionStatusShort(0, "cacheControl = NULL");
        return true; // Do we want to do this?
    }
    debugOutFunctionStatusShort(0, "Cache-Control: ");
    debugOutFunctionStatusShort(0, cacheControl);
    
    noCache = stringStr(cacheControl, "no-cache");
    noStore = stringStr(cacheControl, "no-store");
    maxAge  = stringStr(cacheControl, "max-age");
    
    if (noStore) {
        debugOutFunctionStatusShort(0, "no-store");
        return false;
    }
    
    if (noCache) {
        // Right now we just check for a plain Cache-Control: no-cache
        // (we assume that no-cache="..." is not for the page), TODO: Fix
        if (noCache[8] != '=') {
            debugOutFunctionStatusShort(0, "no-cache");
            return false;
        }
    }
    
    if (maxAge) {
        char    number[20];
        int     i           = 0;
        char    *maxAgeVal  = maxAge;
        int     value       = 0;
        
        while (!stringIsNumeric(*maxAgeVal++));
        
        while (stringIsNumeric(maxAgeVal[i])) {
            number[i] = maxAgeVal[i];
            
            i++;
        }
        number[i] = '\0';
        
        value = stringAToI(number);
        
        debugOutFunctionStatusShort(value, "max-age");
        
        if (value <= 0) {
            debugOutFunctionStatusShort(value, "max-age <= 0");
            return false;
        }
    }
    
    return true;
}

static void cacheAddToBuf(char **buf, unsigned long *bufSize, char *str, unsigned long len)
{
    //unsigned long size;

    if(*buf == NULL) {
        *buf = memMgrChunkNew(len + 1);
        if (*buf == NULL)
            return;
    } else {
        *buf = (char *)memMgrChunkRealloc(*buf, *bufSize + len + 2);
        if (*buf == NULL)
            return;
    }

    stringNCopy(*buf + *bufSize, str, len);
    *bufSize += len;
    *(*buf + *bufSize) = '\0';
}


static char *cacheCreateEncodedPath(char *urlPath)
{
    char            code[4];
    char            *buf  = NULL;
    unsigned long   bufSize = 0;

    while (*urlPath) {
        if (!urlIsMustEscapeChar(*urlPath)) {
            cacheAddToBuf(&buf, &bufSize, urlPath, 1);
        } else {
            if (*urlPath == '%') { // Is this a stray % or a hex encoding
                if ((stringIsNumeric(*(urlPath + 1)) || stringChrInRangeInclusive(*(urlPath + 1), 'A', 'F') ||
                     stringChrInRangeInclusive(*(urlPath + 1), 'a', 'f')) &&
                    (stringIsNumeric(*(urlPath + 2)) || stringChrInRangeInclusive(*(urlPath + 2), 'A', 'F') ||
                     stringChrInRangeInclusive(*(urlPath + 2), 'a', 'f'))) {
                        cacheAddToBuf(&buf, &bufSize, urlPath, 3);
                        
                        urlPath += 3;
                        
                        continue;
                }
            }
            
            code[0] = '%';
            code[1] = "0123456789ABCDEF"[(*urlPath >> 4) & 0x0F];
            code[2] = "0123456789ABCDEF"[ *urlPath       & 0x0F];
            code[3] = 0;

            cacheAddToBuf(&buf, &bufSize, code, 3);           
        }

        urlPath++;
    }
    
    return buf;
}

static char *cacheCreateEncodedURL(LoaderPtr loader, int *err)
{
    char            *path       = NULL;
    char            *pathFixed  = NULL;
    char            *loc        = NULL;
    URLPtr          url         = NULL;
    unsigned long   fixedLen    = 0;
    
    *err = systemErrNone;
    
    // We will now encode the URL and concatinate to form a path in this format:
    // <Cache storage location>/<domain>%2F<path>
    
    if (!loader) {
        *err = loaderErrNoLoader;
        return NULL;
    }

    path    = corePrefGetCacheStorageLoc(loader->globals);
    url     = loaderGetURL(loader);
    
    if(!path || !url || !urlGetHost(url)) {
        *err = loaderErrInvalidParams;
        return NULL;
    }
    
    pathFixed   = (urlGetPath(url))?cacheCreateEncodedPath(urlGetPath(url)):NULL;
    
    fixedLen = stringLen(path) + stringLen(urlGetHost(url)) + stringLen(pathFixed) + 10; // 10 is a buffer
    
    loc = memMgrChunkNew(fixedLen);
    if (!loc) {
        *err= memoryErrNoFreeMemory;
        
        goto cacheCreateEncodedURL_cleanup;
    }
    
    memMgrChunkSet(loc, 0, fixedLen);
    
    stringCopy(loc, path);
    stringCopy(loc + stringLen(loc), urlGetHost(url));
    stringCopy(loc + stringLen(loc), pathFixed); // TODO: stringCat
    
cacheCreateEncodedURL_cleanup:
    if (pathFixed)
        memMgrChunkFree(pathFixed);
        
    return loc;
}

int cacheWriteHeaders(FILE *cacheFile, LoaderPtr loader)
{
    int     error = systemErrNone;

    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->protocol) {
        case loaderHTTP:
            {
                HTTPRequestPtr  request     = loaderGetHTTP(loader);
                HTTPHeaderPtr   header      = NULL;
                WebDataPtr      webData     = loaderGetWebData(loader);
                boolean         haveLen     = false;
                boolean         haveType    = false;
    
                if (!request || !request->headersRequest)
                    return httpErrInvalidRequest;
    
                header = request->headersResponse;
    
                while (header) {
                    if (stringCompare(header->header, HTTP_SET_COOKIE_HDR) &&
                        stringCompare(header->header, HTTP_SET_COOKIE2_HDR)) {
                            // TODO: Should check to see if the content type has a charset attached, if not, add
                            fileWrite(header->header, stringLen(header->header), 1, cacheFile);
                            fileWrite(HTTP_HEADER_SEP_COM, stringLen(HTTP_HEADER_SEP_COM), 1, cacheFile);
                            fileWrite(header->value, stringLen(header->value), 1, cacheFile);
                            fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
                            
                            // Let's make it easier for the HTTP parser if things are missing
                            if (!stringCompare(header->header, HTTP_CONTENTLENGTH)) {
                                haveLen     = true;
                            } else if (!stringCompare(header->header, HTTP_CONTENTTYPE)) {
                                haveType    = true;
                            }
                    }
                    
                    header = header->next;
                }
                
                if (!haveLen) {
                    char    len[50];
                    
                    stringPrintF(len, "%ld", webDataGetContentLength(webData));
                    
                    fileWrite(HTTP_CONTENTLENGTH_HDR, stringLen(HTTP_CONTENTLENGTH_HDR), 1, cacheFile);
                    fileWrite(len, stringLen(len), 1, cacheFile);
                    fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
                }
                if (!haveType) {
                    fileWrite(HTTP_CONTENTTYPE_HDR, stringLen(HTTP_CONTENTTYPE_HDR), 1, cacheFile);
                    fileWrite(webDataGetContentType(webData), stringLen(webDataGetContentType(webData)), 1, cacheFile);
                    fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
                }
                
                fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
            }
            break;
        case loaderWAP:
            break;
        case loaderRELATIVE:
            break;
        case loaderCACHE:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        default:
            error = loaderErrUnsupportedProtocol;
            break;
    }

    return error;
}

/*
 * These cache files are kind of ugly... A lot goes on...
 */
int cacheSaveToCache(LoaderPtr loader)
{
    char            *path       = NULL;
    char            *expires    = NULL;
    WebDataPtr      webData     = NULL;
    FILE            *cacheFile  = NULL;
    int             err         = systemErrNone;
    
    if (!loader)
        return loaderErrNoLoader;

    webData     = loaderGetWebData(loader);
    expires     = cacheGetExpiresHeader(loader);
    
    path = cacheCreateEncodedURL(loader, &err);
    if (!path) {
        return err;
    }

    cacheFile = fileOpen(path, "w");
    if (!cacheFile) {
        err = fileErrFileNotCreated;
        
        goto cacheSaveToCache_cleanup;
    }
    
    // Write Expirary information
    // -1 Means never, 0 means NOW!, and any number means seconds from system's epoch
    if (expires) {
        long    epochSec = timeStringToSeconds(expires);
        char    epochStr[50];
        
        stringPrintF(epochStr, "%ld", epochSec);
        fileWrite(epochStr, stringLen(epochStr), 1, cacheFile);
    } else {
        fileWrite("-1", 2, 1, cacheFile);
    }
    fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
    
    // Write that we had a HTTP 200
    fileWrite(CACHE_HTTP_200, stringLen(CACHE_HTTP_200), 1, cacheFile);
    fileWrite(HTTP_LINE_ENDING, stringLen(HTTP_LINE_ENDING), 1, cacheFile);
    
    // Write headers
    cacheWriteHeaders(cacheFile, loader);
    
    // Write data
    fileWrite(webDataGetContent(webData), webDataGetContentLength(webData), 1, cacheFile);
    
cacheSaveToCache_cleanup:
    if (cacheFile) {
        fileClose(cacheFile);
    }
    
    if (path)
        memMgrChunkFree(path);
    
    return err;
}

static unsigned long cacheFileReadExpiresHeader(FILE *cacheFile, int *err)
{
    char            data[512]; // This information will never be this long under normal circumstances.
                               // If it is, assume corruption.
    char            readByte    = 0;
    int             read        = 0;
    //unsigned long   expires     = 0;
    
    *err = systemErrNone;
    
    if (!cacheFile) {
        *err = loaderErrCacheElementDoesNotExist;
        return 0;
    }
    
    while ((readByte != '\n') && (read < 512) && fileRead(&readByte, 1, 1, cacheFile)) {
        data[read] = readByte;
        read++;
    }
    data[read] = '\0';
    
    debugOutFunctionStatusShort(0, "expire string");
    debugOutFunctionStatusShort(0, data);
    
    if (!stringLen(data)) {
        *err = loaderErrCacheElementDoesNotExist;
        return 0;
    }
    
    return stringAToI(data);
}

int cacheLoadFromCache(LoaderPtr loader)
{
    char            *path       = NULL;
    unsigned long   expires     = 0;
    unsigned long   size        = 0;
    FILE            *cacheFile  = NULL;
    CacheObjectPtr  cache       = NULL;
    int             err         = systemErrNone;
    boolean         deleteFile  = false;
    
    debugOutFunctionStatusShort(0, "top");
    
    if (!loader)
        return loaderErrNoLoader;
    
    path = cacheCreateEncodedURL(loader, &err);
    if (!path) {
        debugOutFunctionStatusShort(err, "!path!!!!");
        return err;
    }
    
    debugOutFunctionStatusShort(0, "path:");
    debugOutFunctionStatusShort(0, path);

    cacheFile = fileOpen(path, "r");
    if (!cacheFile) {
        err = loaderErrCacheElementDoesNotExist;
        
        debugOutFunctionStatusShort(err, "could not open file!!");
        
        goto cacheSaveToCache_cleanup;
    }
    
    size = fileSize(cacheFile, NULL);
    
    debugOutFunctionStatusShort(0, "check expirey");
    
    // Read Expirary information
    // -1 Means never, 0 means NOW!, and any number means seconds from system's epoch
    expires = cacheFileReadExpiresHeader(cacheFile, &err);
    if (!expires) {
        debugOutFunctionStatusShort(err, "expired");
        // TODO: check deletion
        deleteFile = true; // Hmmm... Not a great idea!
        goto cacheSaveToCache_cleanup;
    }
    
    if (expires < timeSinceEpoch()) {
        err = loaderErrCacheElementExpired;
        debugOutFunctionStatusShort(err, "expired");
        // TODO: Check deleteion
        deleteFile = true; // Hmmm... Not a great idea!
        goto cacheSaveToCache_cleanup;
    }
    
    debugOutFunctionStatusShort(0, "create cache obj");
    
    cache = (CacheObjectPtr)memMgrChunkNew(sizeof(CacheObject));
    if (!cache) {
        err= memoryErrNoFreeMemory;
        
        debugOutFunctionStatusShort(err, "no free memory");
        
        goto cacheSaveToCache_cleanup;
    }
    memMgrChunkSet(cache, 0, sizeof(CacheObject));
    
    cache->cacheFile    = cacheFile;
    cache->offset       = 0;
    cache->size         = size - fileTell(cacheFile);
    
    debugOutFunctionStatusShort(0, "set loader protocol");
    
    loaderSetCache(cache, loader);
    loader->protocol    = loaderCACHE;
    
    return systemErrNone;
    
cacheSaveToCache_cleanup:
    if (cacheFile) {
        fileClose(cacheFile);
    }
    
    if (deleteFile && path) {
        fileDelete(path);
    }
    
    if (path)
        memMgrChunkFree(path);
    
    return err;
}

void cacheDestroyCache(LoaderPtr loader)
{
    if (!loader || !loader->cache)
        return;
    
    if (loader->cache->cacheFile)
        fileClose(loader->cache->cacheFile);
    
    memMgrChunkFree(loader->cache);
    loader->cache = NULL;
}

int cacheFlushCache(CoreGlobalsPtr globals)
{
    char    *path   = NULL;
    
    if (!globals)
        return -1;
    
    path = corePrefGetCacheStorageLoc(globals);
    if (!path)
        return -1;
    
    return fileFlushDirectory(path);
}

int cacheSendRequest(LoaderPtr loader)
{
    HTTPRequestPtr  request     = NULL;
    int             err         = systemErrNone;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", 0, __FILE__, __LINE__);
    
    if (!loader) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid loader", 0, __FILE__, __LINE__);
        return loaderErrNoLoader;
    }
        
    request = loaderGetHTTP(loader);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup receive", err, __FILE__, __LINE__);
    
    request->state = HTTP_Parse_Status;
    
    httpSetResponseTimeout(request);

    return err;
}

static unsigned char *cacheHTTPGetOffset(HTTPRequestPtr request)
{
    return request->netData + request->netDataSize;
}

static unsigned long cacheHTTPGetSizeLeft(HTTPRequestPtr request)
{
    return HTTP_BUF_SIZE - request->netDataSize;
}

static void cacheHTTPUpdateSize(unsigned long size, HTTPRequestPtr request)
{
    request->netDataSize += size;
}

int cacheReceiveResponse(LoaderPtr loader)
{
    CacheObjectPtr  cache       = NULL;
    HTTPRequestPtr  request     = NULL;
    short           readRes     = 0;
    int             err         = systemErrNone;
    unsigned char   *bufP       = NULL;
    unsigned short  bufSize     = 0;

    debugOutFunctionFormattedV0(__FUNCTION__, "top", 0, __FILE__, __LINE__);

    if (!loader) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no loader", 0, __FILE__, __LINE__);
        return loaderErrNoLoader;
    }
    
    request = loaderGetHTTP(loader);
    cache   = loaderGetCache(loader);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    if (!cache) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return loaderErrCacheElementDoesNotExist;
    }
    
    if (request->remoteClosed) { // not likely... but... TODO: Remove?
        return systemErrNone;
    }


    bufP    = cacheHTTPGetOffset(request);
    bufSize = (unsigned short)cacheHTTPGetSizeLeft(request);
        
    if (!bufSize) {
        return systemErrNone; // do not need more data
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "receiving data, size left", bufSize, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, "receiving data, buf addy", (unsigned long)bufP, __FILE__, __LINE__);

    readRes = fileRead(bufP, 1, bufSize, cache->cacheFile);

    if (readRes < 0) {
        httpSetError(fileErrReadError, request);
        debugOutFunctionFormattedV0(__FUNCTION__, "error", fileErrReadError, __FILE__, __LINE__);
        
        goto cacheReceiveResponse_cleanup;
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "received", readRes, __FILE__, __LINE__);

    if (err == systemErrNone) {
        if (readRes == 0) {
            request->remoteClosed = true;
        } else {
            cacheHTTPUpdateSize(readRes, request);
        }
    }


cacheReceiveResponse_cleanup:
    return err;

}
