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
 
#ifndef Loader_h
#define Loader_h

#include "config.h"

#include "Cookies.h"
#include "HTTP.h"
#include "Net.h"
#include "WebData.h"

typedef enum LoaderProtocolEnum {
    loaderNONE = 0,
    loaderHTTP,
    loaderWAP,
    loaderFILE,
    loaderABOUT,
    loaderDATA,
    loaderRELATIVE,
    loaderCACHE,
    loaderOTHER
} LoaderProtocolEnum;

typedef enum LoaderCallbackStateEnum {
    loaderNoState,
    loaderReceiveHeaderState,
    loaderHeaderDoneState,
    loaderReceiveDataState,
    loaderDataDoneState
} LoaderCallbackStateEnum;

typedef struct LoaderCallbackType {
    void                        *loader; // LoaderPtr
    void                        *userContext;
    
    char                        *header;
    char                        *value;
    
    unsigned char               *dataP;
    unsigned long               dataLen;
    
    boolean                     handled;
    LoaderCallbackStateEnum     state;
} LoaderCallbackType;

typedef LoaderCallbackType *LoaderCallbackPtr;

typedef enum LoaderStateEnum {
    loaderStateNone = 0,
    loaderStateRequesting,
    loaderStateReceiving,
    loaderStateDone,
    loaderStateError
} LoaderStateEnum;

// TODO: dataP needs to be replaced by LoaderCallbackPtr
typedef int (*LoaderNotifyCallbackPtr)(LoaderCallbackPtr data, void *loader, void *dataP); // loader = LoaderPtr
typedef int (*LoaderStateChangeCallbackPtr)(LoaderStateEnum newState, void *loader, void *dataP); // loader = LoaderPtr

typedef struct LoaderType {
    void                    *userContext;
    
    WebDataPtr              webData;
    
    NetSessionPtr           netSession;
    
    struct CacheObject      *cache;

    URLPtr                  url;
    URLPtr                  lastURL;
    
    CoreGlobalsPtr          globals;
    
    union {
        HTTPRequestPtr      http;
        //WAPRequestPtr     wap;
        //FILERequestPtr    file;
        //DATARequestPtr    dataRequest; // ??? Do we want this?
    } request;
    
    LoaderProtocolEnum      protocol;
    
    LoaderStateEnum         state;
    
    LoaderNotifyCallbackPtr callbackP;
    void                    *callbackDataP;
    
    LoaderStateChangeCallbackPtr    stateChngP;
    void                            *stateChngDataP;
    
    // TODO: This is read from the CorePrefs
    boolean                 flagDoNotCache;
    boolean                 flagDoNotReadCache;
} LoaderType;

typedef LoaderType *LoaderPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern LoaderPtr loaderNewLoader(char *url, LoaderProtocolEnum protocol, LoaderNotifyCallbackPtr callbackP,
                            LoaderStateChangeCallbackPtr stateChngP, WebDataPtr webData,
                            HTTPRequestEnum requestType, CoreGlobalsType *globals, URLPtr lastURL, int *err);
extern LoaderPtr loaderNewLoaderFromURL(char *url, LoaderProtocolEnum protocol, LoaderNotifyCallbackPtr callbackP,
                            LoaderStateChangeCallbackPtr stateChngP, WebDataPtr webData,
                            HTTPRequestEnum requestType, CoreGlobalsType *globals, URLPtr lastURL, int *err);
extern LoaderPtr loaderNewLoaderGeneric(char *url, WebDataPtr webData, HTTPRequestEnum requestType,
                                        CoreGlobalsType *globals, URLPtr lastURL, int *err);

#define loaderDestroyNothingFlag    0x0000
#define loaderDestroyWebDataFlag    0x0001
#define loaderDestroyLastURLFlag    0x0010
#define loaderFlushWebDataFlag      0x0100
extern void loaderDestroyLoader(int flags, LoaderPtr *loader);

extern int loaderSetPostData(unsigned char *data, unsigned long size, char *mimeType, LoaderPtr loader);
extern int loaderSetProxy(char *proxy, unsigned short port, char *userName, char *password, LoaderPtr loader);
extern int loaderSetAuthentication(char *userName, char *password, LoaderPtr loader);

extern int loaderSetNotifyCallback(LoaderPtr loader, LoaderNotifyCallbackPtr callbackP, void *dataP);
extern int loaderSetStateChangeCallback(LoaderPtr loader, LoaderStateChangeCallbackPtr stateChngP, void *dataP);

extern HTTPRequestPtr loaderGetHTTP(LoaderPtr loader);
extern int loaderSetHTTP(HTTPRequestPtr request, LoaderPtr loader);

extern void loaderSetNetSession(NetSessionPtr session, LoaderPtr loader);
extern NetSessionPtr loaderGetNetSession(LoaderPtr loader);

extern void loaderSetTimeout(unsigned long timeout, LoaderPtr loader);
extern unsigned long loaderGetTimeout(LoaderPtr loader);

extern int loaderSetUserAgent(char *userAgent, LoaderPtr loader);

extern int loaderStartHeaders(LoaderPtr loader);
extern int loaderFinishHeaders(LoaderPtr loader);

extern LoaderProtocolEnum loaderGetProtocol(LoaderPtr loader);

extern void loaderSetUserContext(void *context, LoaderPtr loader);
extern void *loaderGetUserContext(LoaderPtr loader);
extern void loaderSetHTTPVersion(int major, int minor, LoaderPtr loader);
extern void loaderSetURL(URLPtr url, LoaderPtr loader);
extern URLPtr loaderGetURL(LoaderPtr loader);
extern void loaderSetLastURL(URLPtr url, LoaderPtr loader);
extern URLPtr loaderGetLastURL(LoaderPtr loader);
extern int loaderParseAndSetURL(char *urlP, LoaderPtr loader);
extern unsigned char *loaderGetRedirectURL(LoaderPtr loader);

extern void loaderSetWebData(WebDataPtr webData, LoaderPtr loader);
extern WebDataPtr loaderGetWebData(LoaderPtr loader);

extern int loaderStartLoader(LoaderPtr loader);

extern int loaderStopLoader(LoaderPtr loader);

extern int loaderSendRequest(LoaderPtr loader);
extern int loaderReceiveResponse(LoaderPtr loader);
extern int loaderHandleResponse(LoaderPtr loader);
extern int loaderReceiveAndHandleResponse(LoaderPtr loader);
extern int loaderStateLoop(LoaderPtr loader);

extern boolean loaderIsDone(LoaderPtr loader);
extern boolean loaderHasError(LoaderPtr loader);
extern boolean loaderIsErrorCritical(int error);
extern int loaderGetError(LoaderPtr loader);

extern unsigned short loaderGetResponseCode(LoaderPtr loader);

extern char *loaderFindHeader(char *header, LoaderPtr loader);

extern HTTPRequestEnum loaderGetRequestType(LoaderPtr loader);

#ifdef __cplusplus
}
#endif

#endif
