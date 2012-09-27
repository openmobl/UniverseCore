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
#include "Loader.h"

#include "Cache.h"
#include "Cookies.h"
#include "CorePrefs.h"
#include "HTTP.h"
#include "MemMgr.h"

// TODO: Instead of using switch statements, we should use a callback

int loaderHTTPReceiveHeader(char *header, char *value, WebDataPtr webData, HTTPCallbackPtr callDataP);
int loaderHTTPReceiveData(unsigned char *dataP, unsigned long dataLen, WebDataPtr webData, HTTPCallbackPtr callDataP);
static int loaderCallCallback(LoaderCallbackPtr data, LoaderPtr loader);
static int loaderCallStateChangeCallback(LoaderStateEnum newState, LoaderPtr loader);
static int loaderGrabAuthFromURL(LoaderPtr loader);
static int loaderChangeState(LoaderStateEnum newState, LoaderPtr loader);
static LoaderProtocolEnum loaderDetermineProtocol(char *url);

// Use loaderNewLoaderGeneric for now.... Unless you want to force WAP

// HTTPRequestEnum is protocol generic. All protocols will support a GET method.
LoaderPtr loaderNewLoader(char *url, LoaderProtocolEnum protocol, LoaderNotifyCallbackPtr callbackP,
                            LoaderStateChangeCallbackPtr stateChngP, WebDataPtr webData,
                            HTTPRequestEnum requestType, CoreGlobalsType *globals, URLPtr lastURL, int *err)
{
    LoaderPtr   loader  = NULL;
    int         error   = systemErrNone;
    int         urlErr  = systemErrNone;
    int         setErr  = systemErrNone;
    
    if (!url || !globals) {
        *err = loaderErrInvalidParams;
        return NULL;
    }
    
    loader = (LoaderType *)memMgrChunkNew(sizeof(LoaderType));
    if (!loader) {
        *err = memoryErrNoFreeMemory;
        return NULL;
    }
    memMgrChunkSet(loader, 0, sizeof(LoaderType));
    
    loader->state       = loaderStateNone; // No state
    
    loaderSetWebData(webData, loader);
    
    loader->protocol    = protocol;
    loader->callbackP   = callbackP;
    loader->stateChngP  = stateChngP;
    
    loader->globals     = globals;

    //loaderSetTimeout(globals->loader.timeout, loader);
    
    loaderSetLastURL(lastURL, loader);
    
    urlErr = loaderParseAndSetURL(url, loader);
    if (urlErr) {
        memMgrChunkFree(loader);
        *err = urlErr;
        
        return NULL;
    }
    
    loaderGrabAuthFromURL(loader);
    
    if (urlErr == urlErrIsDocumentRelative) { // TODO: A quick way to let the caller know that it should skip trying to "load" a page
        *err = urlErrIsDocumentRelative;
        
        loader->protocol = loaderRELATIVE;
        
        return loader;
    } // else {
    
    // Check Cache
    cacheLoadFromCache(loader); // This will check the cache and auto-setup the proper stuff to get us to read the cache
                                // if anything exists. Oherwise it will act "transparent."
    // }


    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            // TODO: loader->request.http should be gotten and set using a getter/setter, setter should probably be static
            setErr = loaderSetHTTP(httpCreateRequest(requestType, &error), loader);
            if (error != systemErrNone) {
                *err = error;
                
                goto loaderNewLoader_bottom;
            } else if (setErr != systemErrNone) {
                *err = setErr;
                
                goto loaderNewLoader_bottom;
            }
            
            httpSetLoader(loader, loaderGetHTTP(loader));
            
            httpSetReceiveHeaderNotify(loaderHTTPReceiveHeader, loaderGetHTTP(loader));
            httpSetReceiveDataNotify(loaderHTTPReceiveData, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            *err = loaderErrUnsupportedProtocol;
            break;
    }
    
loaderNewLoader_bottom:

    return loader;
}

// Pass in protocol to specify a default protocol. For instance if protocol is loaderWAP and the URL begins
// with http: then WAP will be used. Set to loaderNONE or loaderHTTP to ignore.
LoaderPtr loaderNewLoaderFromURL(char *url, LoaderProtocolEnum protocol, LoaderNotifyCallbackPtr callbackP,
                            LoaderStateChangeCallbackPtr stateChngP, WebDataPtr webData,
                            HTTPRequestEnum requestType, CoreGlobalsType *globals, URLPtr lastURL, int *err)
{
    LoaderPtr           loader = NULL;
    LoaderProtocolEnum  protoNew = loaderNONE;

    if (!url)
        return NULL;
    
    protoNew = (protocol == loaderWAP)?loaderWAP:loaderDetermineProtocol(url);
    
    loader = loaderNewLoader(url, protoNew, callbackP, stateChngP, webData, requestType, globals, lastURL, err);
    
    return loader;
}

LoaderPtr loaderNewLoaderGeneric(char *url, WebDataPtr webData, HTTPRequestEnum requestType,
                                 CoreGlobalsType *globals, URLPtr lastURL, int *err)
{
    LoaderPtr           loader      = NULL;
    
    loader = loaderNewLoaderFromURL(url, loaderNONE, NULL, NULL, webData, requestType, globals, lastURL, err);
    
    return loader;
}

void loaderDestroyLoader(int flags, LoaderPtr *loader)
{
    if (!loader || !*loader)
        return;
    
    switch ((*loader)->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            httpCloseRequest((*loader)->request.http);
            httpFinish(&(*loader)->request.http);
            (*loader)->request.http = NULL;
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }
    
    netConnectionClose((*loader)->netSession);
    netCloseNetwork(&(*loader)->netSession, false);
    
    cacheDestroyCache((*loader));
    
    if (flags & loaderDestroyWebDataFlag) {
        webDataFree(&(*loader)->webData);
    } else if (flags & loaderFlushWebDataFlag) {
        webDataFlush((*loader)->webData);
    }
    if (flags & loaderDestroyLastURLFlag) {
        urlDestroyURL((*loader)->lastURL);
        (*loader)->lastURL = NULL;
    }
    
    memMgrChunkFree(*loader);
    
    *loader = NULL;
}

static LoaderProtocolEnum loaderDetermineProtocol(char *url)
{
    LoaderProtocolEnum  new = loaderNONE;
    
    // TODO: Make this a lookup table, just itterate through
    if (!stringNCompareCaseless(url, "http", 4)) {
        new = loaderHTTP;
    } else if (!stringNCompareCaseless(url, "file", 4)) {
        new = loaderFILE;
    } else if (!stringNCompareCaseless(url, "about", 5)) {
        new = loaderABOUT;
    } else if (!stringNCompareCaseless(url, "data", 4)) {
        new = loaderDATA;
    } else {
        new = loaderOTHER;
    }
    
    return new;
}

int loaderSetPostData(unsigned char *data, unsigned long size, char *mimeType, LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err =  httpRequestPostAddContent(data, size, mimeType, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE: // No POST!!
        default:
            err = loaderErrUnsupportedProtocol;
            break;
    }
    
    return err;
}

int loaderSetNotifyCallback(LoaderPtr loader, LoaderNotifyCallbackPtr callbackP, void *dataP)
{
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    loader->callbackP       = callbackP;
    loader->callbackDataP   = dataP;
    
    return systemErrNone;
}

int loaderSetStateChangeCallback(LoaderPtr loader, LoaderStateChangeCallbackPtr stateChngP, void *dataP)
{
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    loader->stateChngP      = stateChngP;
    loader->stateChngDataP  = dataP;
    
    return systemErrNone;
}

HTTPRequestPtr loaderGetHTTP(LoaderPtr loader)
{
    return (loader)?loader->request.http:NULL;
}

int loaderSetHTTP(HTTPRequestPtr request, LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
    
    loader->request.http = request;
    
    return systemErrNone;
}

CacheObjectPtr loaderGetCache(LoaderPtr loader)
{
    return (loader)?loader->cache:NULL;
}

int loaderSetCache(CacheObjectPtr cache, LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
    
    loader->cache = cache;
    
    return systemErrNone;
}

void loaderSetHTTPVersion(int major, int minor, LoaderPtr loader)
{
    if (!loader)
        return;

    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            httpSetRequestHTTPVersion((unsigned short)major, (unsigned short)minor, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }
}

int loaderSetProxy(char *proxy, unsigned short port, char *userName, char *password, LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err = httpSetRequestProxy(proxy, port, userName, password, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            err = loaderErrUnsupportedProtocol;
            break;
    }
    
    return err;
}

void loaderSetUserContext(void *context, LoaderPtr loader)
{
    if (!loader)
        return;
        
    loader->userContext = context;
}

void *loaderGetUserContext(LoaderPtr loader)
{
    if (!loader)
        return NULL;
    
    return loader->userContext;
}

int loaderSetAuthentication(char *userName, char *password, LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err = httpSetRequestAuthentication(userName, password, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE:
        default:
            err = loaderErrUnsupportedProtocol;
            break;
    }
    
    return err;
}

int loaderGrabAuthFromURL(LoaderPtr loader)
{
    URLPtr  url     = loaderGetURL(loader);
    char    *user   = NULL;
    char    *pass   = NULL;
    
    if (!url)
        return loaderErrInvalidParams;
        
    user = urlGetUserName(url);
    pass = urlGetPassword(url);
    
    return loaderSetAuthentication(user, pass, loader);
}

int loaderStartHeaders(LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err = httpCreateRequestHeader(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE:
        default:
            break;
    }
    
    return err;
}

int loaderFinishHeaders(LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err = httpRequestCompleteHeader(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE:
        default:
            break;
    }
    
    return err;
}

int loaderSetUserAgent(char *userAgent, LoaderPtr loader)
{
    int     err = systemErrNone;
    
    if (!loader) {
        return loaderErrNoLoader;
    }
    
    switch (loader->protocol) {
        case loaderHTTP:
            err = httpSetRequestUserAgent(userAgent, loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE:
        default:
            break;
    }
    
    return err;
}

void loaderSetNetSession(NetSessionPtr session, LoaderPtr loader)
{
    if (!loader)
        return;
    
    loader->netSession = session;
}

NetSessionPtr loaderGetNetSession(LoaderPtr loader)
{
    if (!loader)
        return NULL;
    
    return loader->netSession;
}

// Nothing for now
void loaderSetTimeout(unsigned long timeout, LoaderPtr loader)
{
    if (!loader)
        return;
    
    //loader->timeout = timeout;
}

unsigned long loaderGetTimeout(LoaderPtr loader)
{
    if (!loader)
        return 0;
    
    return corePrefsGetTimeout(loader->globals);
}

LoaderProtocolEnum loaderGetProtocol(LoaderPtr loader)
{
    return (loader)?loader->protocol:loaderNONE;
}

void loaderSetURL(URLPtr url, LoaderPtr loader)
{
    if (!loader)
        return;
    
    loader->url = url;
}

URLPtr loaderGetURL(LoaderPtr loader)
{
    return (loader)?loader->url:NULL;
}

void loaderSetLastURL(URLPtr url, LoaderPtr loader)
{
    if (!loader)
        return;
    
    loader->lastURL = url;
}

URLPtr loaderGetLastURL(LoaderPtr loader)
{
    return (loader)?loader->lastURL:NULL;
}

int loaderParseAndSetURL(char *urlP, LoaderPtr loader)
{
    URLPtr  url     = NULL;
    int     error   = systemErrNone;
    
    url = urlParseURL(urlP, &error);
    if (!url) {
        return error;
    }
    
    if ((error == urlErrIncompleteURL) && loaderGetLastURL(loader)) {
        URLPtr  urlTemp = url;
        
        url = urlCombineURLs(loaderGetLastURL(loader), urlTemp, &error);
        
        urlDestroyURL(urlTemp);
        
        if (!url) {
            return error;
        }
        
        loader->protocol = loaderDetermineProtocol(urlGetFull(url));
    } else if (error) {
        urlDestroyURL(url);
        
        return urlErrMalformedURL;
    }
    
    loaderSetURL(url, loader);
    
    return systemErrNone;
}

unsigned char *loaderGetRedirectURL(LoaderPtr loader)
{
    char    *url = NULL;
    
    if (!loader)
        return 0;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            url = httpGetResponseRedirectURL(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }
    
    return (unsigned char *)url;
}

void loaderSetWebData(WebDataPtr webData, LoaderPtr loader)
{
    if (!loader)
        return;
    
    loader->webData = webData;
}

WebDataPtr loaderGetWebData(LoaderPtr loader)
{
    return (loader)?loader->webData:NULL;
}


static int loaderCallStateChangeCallback(LoaderStateEnum newState, LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
    
    return (loader->stateChngP)?loader->stateChngP(newState, loader, loader->stateChngDataP):loaderErrCallbackUndefined;
}

static int loaderChangeState(LoaderStateEnum newState, LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
        
    loader->state = newState;
    
    return loaderCallStateChangeCallback(newState, loader);
}

int loaderStartLoader(LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
        
    loaderChangeState(loaderStateRequesting, loader);
    
    return systemErrNone;
}

int loaderStopLoader(LoaderPtr loader)
{
    if (!loader)
        return loaderErrNoLoader;
    
    // TODO: Stop loader
    //loaderChangeState(loaderStateRequesting, loader);
    
    return systemErrNone;
}

int loaderSendRequest(LoaderPtr loader)
{
    int     error = systemErrNone;

    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->protocol) {
        case loaderHTTP:
            error = httpOpenRequest(loaderGetHTTP(loader)); // connect
            if (error) {
                goto loaderSendRequest_cleanup;
            }
    
            error = httpSendRequest(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderRELATIVE:
            break;
        case loaderCACHE:
            error = cacheSendRequest(loader);
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        default:
            error = loaderErrUnsupportedProtocol;
            break;
    }
    
    // Dealt with below
    /*if (error == systemErrNone) {
        loaderChangeState(loaderStateReceiving, loader);
    }*/

loaderSendRequest_cleanup:
    return error;
}

int loaderReceiveResponse(LoaderPtr loader)
{
    int     error = systemErrNone;

    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->protocol) {
        case loaderHTTP:
            error = httpReceiveResponse(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderRELATIVE:
            break;
        case loaderCACHE:
            error = cacheReceiveResponse(loader);
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        default:
            error = loaderErrUnsupportedProtocol;
            break;
    }

    // Dealt with below
    /*if (error != systemErrNone) {
        loaderChangeState(loaderStateNone, loader);
    }*/

    return error;
}

int loaderHandleResponse(LoaderPtr loader)
{
    int     error = systemErrNone;

    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            error = httpHandleResponse(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderRELATIVE:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        default:
            error = loaderErrUnsupportedProtocol;
            break;
    }
    
    // Dealt with below
    /*if (loaderIsErrorCritical(error)) {
        loaderChangeState(loaderStateNone, loader);
    }*/

    return error;
}

boolean loaderIsDone(LoaderPtr loader)
{
    boolean     done    = true;
    
    if (!loader)
        return true;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            done = httpIsDone(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderRELATIVE:
            done = true; // we have net activity so we are always done :)
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        default:
            break;
    }

    return done;
}

boolean loaderHasError(LoaderPtr loader)
{
    boolean     error    = false;
    
    if (!loader)
        return true;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            error = httpHasError(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }

    return error;
}

boolean loaderIsErrorCritical(int error)
{
    return ((error != systemErrNone) && (error != httpErrRedirectRequested) &&
            (error != httpErrStatusNotOK) && (error != urlErrIsDocumentRelative));
}

int loaderGetError(LoaderPtr loader)
{
    int     error    = systemErrNone;
    
    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            error = httpGetError(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }

    return error;
}

// This function is best looped, call send, then receiveandhandle
// This will cache the page if possible
int loaderReceiveAndHandleResponse(LoaderPtr loader)
{
    int     error = systemErrNone;

    if (!loader)
        return loaderErrNoLoader;
    
    if (loaderIsDone(loader)) {
        loaderChangeState(loaderStateDone, loader);
        return systemErrNone;
    }
    
    if (loader->state != loaderStateReceiving) {
        error = loaderErrInvalidState;
        
        goto loaderReceiveAndHandleResponse_cleanup;
    }
    
    if (loaderHasError(loader)) {
        error = loaderGetError(loader);
        
        goto loaderReceiveAndHandleResponse_cleanup;
    }
        
    error = loaderReceiveResponse(loader);
    if (error != systemErrNone) {
        goto loaderReceiveAndHandleResponse_cleanup;
    }
    
    error = loaderHandleResponse(loader);
    if (error != systemErrNone) {
        goto loaderReceiveAndHandleResponse_cleanup;
    }
    
    if (loaderIsDone(loader)) {
        loaderChangeState(loaderStateDone, loader);
        
        // Now cache if we are allowed
        if (cacheObjectIsCacheable(loader))
            cacheSaveToCache(loader);
    }

    return systemErrNone;
    
loaderReceiveAndHandleResponse_cleanup:
    loaderChangeState(loaderStateError, loader);
    
    return error;
}


int loaderStateLoop(LoaderPtr loader)
{
    int error   = systemErrNone;
    
    if (!loader)
        return loaderErrNoLoader;
    
    switch (loader->state) {
        case loaderStateRequesting:
            error = loaderSendRequest(loader);
            
            if (!loaderIsErrorCritical(error)) {
                loaderChangeState(loaderStateReceiving, loader);
            } else {
                loaderChangeState(loaderStateError, loader);
                return error;
            }
            break;
        case loaderStateReceiving:
            error = loaderReceiveAndHandleResponse(loader);
            break;
        case loaderStateDone:
            break;
        case loaderStateError:
        case loaderStateNone:
            break;
    }
    
    return error;
}

unsigned short loaderGetResponseCode(LoaderPtr loader)
{
    unsigned short  response = 0;
    
    if (!loader)
        return 0;
    
    switch (loader->protocol) {
        case loaderHTTP:
        case loaderCACHE:
            response = httpGetResponseCode(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            response = 200;
            break;
    }
    
    return response;
}

HTTPRequestEnum loaderGetRequestType(LoaderPtr loader)
{
    HTTPRequestEnum request = HTTP_GET;
    
    if (!loader)
        return HTTP_NONE;
    
    switch (loader->protocol) {
        case loaderHTTP:
            request = httpGetRequestType(loaderGetHTTP(loader));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        case loaderCACHE: // Leave here. Always a HTTP_GET.
        default:
            break;
    }
    
    return request;
}

char *loaderFindHeader(char *header, LoaderPtr loader)
{
    char    *value  = NULL;
    
    if (!loader)
        return NULL;
    
    switch (loader->protocol) {
        case loaderHTTP:
            value = httpFindHeader(header, httpGetResponseHeaders(loaderGetHTTP(loader)));
            break;
        case loaderWAP:
            break;
        case loaderFILE:
        case loaderABOUT:
        case loaderDATA:
        case loaderOTHER:
        case loaderRELATIVE:
        default:
            break;
    }
    
    return value;
}

static int loaderCallCallback(LoaderCallbackPtr data, LoaderPtr loader)
{
    if (!loader || !data)
        return loaderErrNoLoader;
    
    return (loader->callbackP)?loader->callbackP(data, loader, loader->callbackDataP):loaderErrCallbackUndefined;
}

int loaderHTTPReceiveHeader(char *header, char *value, WebDataPtr webData, HTTPCallbackPtr callDataP)
{
    LoaderCallbackType  callback;
    LoaderPtr           loaderP = httpGetLoader(callDataP->request);
    int                 ret     = systemErrNone;
    
    memMgrChunkSet(&callback, 0, sizeof(LoaderCallbackType));
    
    switch (callDataP->state) {
        case HTTP_Header_Done:
            callback.state = loaderHeaderDoneState;
            break;
        case HTTP_Receive_Header:
            callback.state = loaderReceiveHeaderState;
            break;
        default:
            return loaderErrInvalidProtocolState;
            break;
    }
    
    callback.header     = header;
    callback.value      = value;
    
    ret = loaderCallCallback(&callback, loaderP);
    
    return ret;
}

int loaderHTTPReceiveData(unsigned char *dataP, unsigned long dataLen, WebDataPtr webData, HTTPCallbackPtr callDataP)
{
    LoaderCallbackType  callback;
    LoaderPtr           loaderP = httpGetLoader(callDataP->request);
    int                 ret     = systemErrNone;
    
    memMgrChunkSet(&callback, 0, sizeof(LoaderCallbackType));
    
    switch (callDataP->state) {
        case HTTP_Data_Done:
            callback.state = loaderDataDoneState;
            break;
        case HTTP_Receive_Data:
            callback.state = loaderReceiveDataState;
            break;
        default:
            return loaderErrInvalidProtocolState;
            break;
    }
    
    callback.dataP          = dataP;
    callback.dataLen        = dataLen;
    
    callback.userContext    = loaderGetUserContext(loaderP);
    
    ret = loaderCallCallback(&callback, loaderP);
    
    return ret;
}
