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
#include "HTTP.h"

#include "Base64.h"
#include "Debug.h"
#include "Error.h"
#include "Loader.h"
#include "MemMgr.h"
#include "Net.h"
#include "PlatformTime.h"

// Internal functions:
static void httpAddToHeader(char *value, HTTPRequestPtr request);
static boolean httpIsDefaultPort(HTTPRequestPtr request);
static int httpCreateRequestGETPOSTHeader(HTTPRequestPtr request);
static unsigned char *httpGetOffset(HTTPRequestPtr request);
static unsigned long httpGetSizeLeft(HTTPRequestPtr request);
static void httpUpdateSize(unsigned long size, HTTPRequestPtr request);
static void httpShrinkSize(unsigned long reduce, HTTPRequestPtr request);
static char *httpSearchToken(unsigned char token, unsigned char *string, unsigned long length);
static char *httpSearchString(char *token, char *string, unsigned long length);
static int httpParseStatus(HTTPRequestPtr request);
static boolean httpIsWhitespace(int tok);
static char *httpGetLine(HTTPRequestPtr request);
static URLPtr httpGetURL(HTTPRequestPtr request);
static int httpBuildRequestHeader(HTTPRequestPtr request);
static int httpSessionAddHeader(char *header, char *value,
                                HTTPHeaderPtr *headersP);
static void httpSessionDeleteHeaders(HTTPHeaderPtr *headersP);
static int httpSplitHeader(char *line, char **header, char **value);
static int httpParseHeader(HTTPRequestPtr request);
static int httpParseData(HTTPRequestPtr request);
static int httpMoveData(unsigned char *content, unsigned long *used, HTTPRequestPtr request);
static boolean httpIsResponseStatusInRange(unsigned short range,
                                    HTTPRequestPtr request);
static int httpCallHeaderReceiveCallback(HTTPCallbackStateEnum state, char *header,
                    char *value, HTTPCallbackType *callDataP,
                    HTTPRequestPtr request);
static int httpCallDataReceiveCallback(HTTPCallbackStateEnum state,
                        HTTPCallbackType *callDataP, HTTPRequestPtr request);


typedef struct {
  short         code;
  unsigned char wspVal;
  char          *error;
} HTTPResponseCode;

static const HTTPResponseCode code_list[] =
{
    { 300 , 0x30 , "300: Multiple choices"              },
    { 301 , 0x31 , "301: Moved permanently"             },
    { 302 , 0x32 , "302: Moved temporarily"             },
    { 303 , 0x33 , "303: See other"                     },
    { 304 , 0x34 , "304: Not modified"                  },
    { 305 , 0x35 , "305: Use proxy"                     },
    { 307 , 0x37 , "307: Temporary Redirect"            },
    { 400 , 0x40 , "400: Bad request"                   },
    { 401 , 0x41 , "401: Unauthorized"                  },
    { 402 , 0x42 , "402: Payment required"              },
    { 403 , 0x43 , "403: Forbidden"                     },
    { 404 , 0x44 , "404: Not found"                     },
    { 405 , 0x45 , "405: Method not Allowed"            },
    { 406 , 0x46 , "406: Not acceptable"                },
    { 407 , 0x47 , "407: Proxy authentication required" },
    { 408 , 0x48 , "408: Request timeout"               },
    { 409 , 0x49 , "409: Conflict"                      },
    { 410 , 0x4A , "410: Gone"                          },
    { 411 , 0x4B , "411: Length required"               },
    { 412 , 0x4C , "412: Precondition failed"           },
    { 413 , 0x4D , "413: Request entity too large"      },
    { 414 , 0x4E , "414: Request-URI too large"         },
    { 415 , 0x4F , "415: Unsupported media type"        },
    { 416 , 0x50 , "416: Request Range Not Satisfiable" },
    { 417 , 0x51 , "417: Expectation Failed"            },
    { 500 , 0x60 , "500: Internal server error"         },
    { 501 , 0x61 , "501: Not implemented"               },
    { 502 , 0x62 , "502: Bad Gateway"                   },
    { 503 , 0x63 , "503: Service unavailable"           },
    { 504 , 0x64 , "504: Gateway Timeout"               },
    { 505 , 0x65 , "505: HTTP version not supported"    },
    { 000 , 0x00 , "000: Unknown!!"                     }
};


static boolean httpIsWhitespace(int tok)
{
    return tok == (int)' ' || tok == (int)'\t';//return TxtCharIsSpace(tok);
}

boolean httpHasError(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "hasError request processes", (request->state == HTTP_Error), __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, "error retrieved", request->error, __FILE__, __LINE__);
    return (request->state == HTTP_Error);
}

int httpGetError(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "error retrieved", request->error, __FILE__, __LINE__);
    return request->error;
}

boolean httpIsDone(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "isDone request processes", (request->state == HTTP_Done), __FILE__, __LINE__);
    return (request->state == HTTP_Done);
}

void httpSetDone(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "state set to done", (request->state == HTTP_Done), __FILE__, __LINE__);
    request->state = HTTP_Done;
}

int httpSetErrorExt(int error, char *file, unsigned short line,
                    const char *function, HTTPRequestPtr request)
{
    char    *netErr = NULL;
    
    request->state  = HTTP_Error;
    request->error  = error;
    
    netErr = netLookupError(error);
    
    stringPrintF(request->errorDetails, "%s\n%s:%d\n%s [%d]",
              function, file, line, (netErr)?netErr:" ", error);
    //stringCopy(request->errorDetails, "test");
    
    return error;
}

void httpDisplayError(char *msg, HTTPRequestPtr request)
{
    /*short        tapped;
    
    tapped = FrmCustomAlert(alNetError, msg, "", NULL);
    switch (tapped) {
        case 1:
            FrmCustomAlert(alNetErrorDetails, request->errorDetails, "", NULL);
            break;
        default:
            break;
    }*/
}

void httpSetLoader(void *loader, HTTPRequestPtr request)
{
    request->loader = loader;
}

void *httpGetLoader(HTTPRequestPtr request)
{
    return request->loader;
}

WebDataPtr httpGetWebData(HTTPRequestPtr request)
{
    return loaderGetWebData(httpGetLoader(request));
}

void httpSetContentType(char *mime, HTTPRequestPtr request)
{
    webDataSetContentType(mime, httpGetWebData(request));
}

char *httpGetContentType(HTTPRequestPtr request)
{
    return webDataGetContentType(httpGetWebData(request));
}

void httpSetContentLength(unsigned long length, HTTPRequestPtr request)
{
    webDataSetContentLength(length, httpGetWebData(request));
}

unsigned long httpGetContentLength(HTTPRequestPtr request)
{
    return webDataGetContentLength(httpGetWebData(request));
}

static URLPtr httpGetURL(HTTPRequestPtr request)
{
    return loaderGetURL(httpGetLoader(request));
}

char *httpGetFullURL(HTTPRequestPtr request)
{
#ifdef CONFIG_HTTP_OWN_URL
    return request->url->full;
#else
    URLPtr  url = httpGetURL(request);
    
    return (url)?url->full:NULL;
#endif
}

HTTPRequestEnum httpGetRequestType(HTTPRequestPtr request)
{
    if (!request)
        return HTTP_NONE;
    
    return request->requestType;
}

#ifdef CONFIG_HTTP_OWN_URL
HTTPRequestPtr httpCreateRequest(char *urlP, HTTPRequestEnum requestType, int *err)
{
    HTTPRequestPtr  request = NULL;
    URLPtr          url     = NULL;
    
    *err = systemErrNone;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!urlP) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no URL", 0, __FILE__, __LINE__);
        return NULL;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "parse URL", 0, __FILE__, __LINE__);
    
    url = urlParseURL(urlP, err);
    if (!url) {
        debugOutFunctionFormattedV0(__FUNCTION__, "could not parse URL", 0, __FILE__, __LINE__);
        //*err = urlErrMalformedURL;
        
        return NULL;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "alloc memory", 0, __FILE__, __LINE__);
    
    request = (HTTPRequestType *)memMgrChunkNew(sizeof(HTTPRequestType));
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "alloc failed!", 0, __FILE__, __LINE__);
        *err = httpErrInvalidRequest;
        
        return NULL;
    }
    memMgrChunkSet(request, 0, sizeof(HTTPRequestType));
    
    debugOutFunctionFormattedV0(__FUNCTION__, "request ptr", (long)request, __FILE__, __LINE__);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "set variables", 0, __FILE__, __LINE__);

    request->url            = url;
    request->requestType    = requestType;
    request->netData        = (unsigned char *)memMgrChunkNew(HTTP_BUF_SIZE);
    if (!request->netData) {
        memMgrChunkFree(request);
        *err = memoryErrNoFreeMemory;
        
        return NULL;
    }
    memMgrChunkSet(request->netData, 0, HTTP_BUF_SIZE);
    request->netDataSize    = 0;
    request->state          = HTTP_Send_Request;
    request->carrier        = HTTP_DIRECT; // Client must change
    
    debugOutFunctionFormattedV0(__FUNCTION__, "defualt http version", 0, __FILE__, __LINE__);
    
    httpSetRequestHTTPVersion(1, 1, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "done", 0, __FILE__, __LINE__);
    
    return request;
}
#else
HTTPRequestPtr httpCreateRequest(HTTPRequestEnum requestType, int *err)
{
    HTTPRequestPtr  request = NULL;
    
    *err = systemErrNone;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "alloc memory", 0, __FILE__, __LINE__);
    
    request = (HTTPRequestType *)memMgrChunkNew(sizeof(HTTPRequestType));
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "alloc failed!", 0, __FILE__, __LINE__);
        *err = httpErrInvalidRequest;
        
        return NULL;
    }
    memMgrChunkSet(request, 0, sizeof(HTTPRequestType));
    
    debugOutFunctionFormattedV0(__FUNCTION__, "request ptr", (long)request, __FILE__, __LINE__);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "set variables", 0, __FILE__, __LINE__);

    request->requestType    = requestType;
    request->netData        = (unsigned char *)memMgrChunkNew(HTTP_BUF_SIZE);
    if (!request->netData) {
        memMgrChunkFree(request);
        *err = memoryErrNoFreeMemory;
        
        return NULL;
    }
    memMgrChunkSet(request->netData, 0, HTTP_BUF_SIZE);
    request->netDataSize    = 0;
    request->state          = HTTP_Send_Request;
    request->carrier        = HTTP_DIRECT; // Client must change
    
    debugOutFunctionFormattedV0(__FUNCTION__, "defualt http version", 0, __FILE__, __LINE__);
    
    httpSetRequestHTTPVersion(1, 1, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "done", 0, __FILE__, __LINE__);
    
    return request;
}

#endif

int httpSetRequestProxy(char *proxy, unsigned short port, char *userName,
                        char *password, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invalid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->proxy.address  = stringDup(proxy);
    request->proxy.port     = port;
    request->proxy.userName = stringDup(userName);
    request->proxy.password = stringDup(password);
    
    request->carrier = HTTP_PROXY;
    
    return systemErrNone;
}

int httpSetRequestAuthentication(char *userName, char *password,
                                 HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invalid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    if (!userName || !password) {
        return systemErrNone; // a generic function is setting this, so return
    }
    
    request->userName = stringDup(userName);
    request->password = stringDup(password);
    
    return systemErrNone;
}

int httpSetRequestUserAgent(char *userAgent, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->userAgent = stringDup(userAgent);
    
    return systemErrNone;
}

int httpSetRequestUAProf(char *profile, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->UAProf = stringDup(profile);
    
    return systemErrNone;
}

int httpSetRequestHTTPVersion(unsigned short major, unsigned short minor, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->httpVersionMajor = major;
    request->httpVersionMinor = minor;
    
    return systemErrNone;
}

int httpSetReceiveHeaderNotify(HTTPReceiveHeaderCallbackPtr callbackP,
                               HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->headerReceiveCallbackP = callbackP;
    
    return systemErrNone;
}

int httpSetReceiveDataNotify(HTTPReceiveDataCallbackPtr callbackP,
                             HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->dataReceiveCallbackP = callbackP;
    
    return systemErrNone;
}


/*int httpSetRequestCookies(CookiePtr cookieJar, boolean private, HTTPRequestPtr request)
{
    URLPtr  url;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    if (!request->g) {
        debugOutFunctionFormattedV0(__FUNCTION__, "param err", 0, __FILE__, __LINE__);
        return httpErrParameterError;
    }
    
    url = request->url;
    
    //request->cookies = CookieCreateLinkedList(cookieJar, url->host, url->path,
    //                                false, private, (GlobalsType *)request->g);
    
    return systemErrNone;
}*/

int httpSetUserContext(void *context, HTTPRequestPtr request)
{
    request->userContext = context;
    
    return systemErrNone;
}

void *httpGetUserContext(HTTPRequestPtr request)
{
    return request->userContext;
}

int httpRequestAddHeader(char *header, char *value, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    return httpSessionAddHeader(header, value, &request->headersRequest); //, &request->headersRequestCount);
}

/**
 * Set the request post data. DO NOT FREE DATA UNTIL THE REQUEST IS SENT!
 * @param dataP The post data to send.
 * @param dataLen The post data len.
 * @param dataType The post data mime type.
 */
int httpRequestPostAddContent(unsigned char *dataP, unsigned long dataLen, char *dataType,
                               HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->post.dataP         = dataP;
    request->post.dataLength    = dataLen;
    request->post.dataTypeP     = stringDup(dataType);
    
    return systemErrNone;
}

static void httpAddToHeader(char *value, HTTPRequestPtr request)
{
    request->requestHeader = (unsigned char *)stringCombine((char *)request->requestHeader, value);
    
    if (request->requestHeader) {
        request->requestHeaderLength += stringLen(value);
    }
}

static boolean httpIsDefaultPort(HTTPRequestPtr request)
{
    URLPtr  url = NULL;
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return true; // can't do much else?...
    }

#ifdef CONFIG_HTTP_OWN_URL
    url = request->url;
#else
    url = httpGetURL(request);
#endif

    if (!url) {
        return true;
    }
    
    switch (url->transport)
    {
        case URL_http:
            return (url->port == HTTP_PORT);
            break;
        case URL_https:
            return (url->port == HTTPS_PORT);
            break;
        default:
            break;
    }
    
    return false;
}

static int httpBuildRequestHeader(HTTPRequestPtr request)
{
    HTTPHeaderPtr   header = NULL;
    
    if (!request || !request->headersRequest)
        return httpErrInvalidRequest;
    
    header = request->headersRequest;
    
    while (header) {
        httpAddToHeader(header->header, request);
        httpAddToHeader(HTTP_HEADER_SEP_COM, request);
        httpAddToHeader(header->value, request);
        httpAddToHeader(HTTP_LINE_ENDING, request);
        
        header = header->next;
    }
    
    return systemErrNone;
}

static int httpCreateRequestGETPOSTHeader(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "set up auth", 0, __FILE__, __LINE__);
    // If we have been asked for authentication, send it
    if (request->userName) {
        char                        *auth;
        char                        *value;
        struct base64BufferType     encoded;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "encode", 0, __FILE__, __LINE__);
        auth = (char *)memMgrChunkNew(stringLen(request->userName) + stringLen(request->password) + 2);
        if (!auth) {
            return memoryErrNoFreeMemory;
        }
        stringPrintF(auth, "%s:%s", request->userName, request->password);
        base64Encode(&encoded, auth, stringLen(auth));
        
        value = (char *)memMgrChunkNew(stringLen(HTTP_AUTH_BASIC) + stringLen(encoded.data) + 1);
        if (!value) {
            memMgrChunkFree(auth);
            base64BufferDelete(&encoded);
            
            return memoryErrNoFreeMemory;
        }
        memMgrChunkSet(value, 0, stringLen(HTTP_AUTH_BASIC) + stringLen(encoded.data) + 1);
        stringCopy(value, HTTP_AUTH_BASIC);
        stringCopy(value + stringLen(value), encoded.data);
        
        debugOutFunctionFormattedV0(__FUNCTION__, "add", 0, __FILE__, __LINE__);
        httpRequestAddHeader(HTTP_WEB_AUTH, value, request);

        debugOutFunctionFormattedV0(__FUNCTION__, "delete encoded", 0, __FILE__, __LINE__);
        base64BufferDelete(&encoded);
        memMgrChunkFree(auth);
    }
    
    // TODO: Break the headers that are currently statically defined as optional
    // (i.e. "Accept: ", "Accept-charset: ")
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup connection", 0, __FILE__, __LINE__);
    // Send the connection
    httpRequestAddHeader(HTTP_CONNECTION, HTTP_CONNECTION_CLOSE, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup useragent", 0, __FILE__, __LINE__);
    // Send the user agent.
    if (request->userAgent) {
        httpRequestAddHeader(HTTP_USERAGENT, request->userAgent, request);
    } else {
        httpRequestAddHeader(HTTP_USERAGENT, HTTP_GEN_AGENT, request);
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup accept char set", 0, __FILE__, __LINE__);
    // Send the accepted char set
    httpRequestAddHeader(HTTP_ACCEPT_CHARSET, HTTP_ACCEPT_CHARSET_VAL, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup sccept languages", 0, __FILE__, __LINE__);
    // Send the accepted languages
    httpRequestAddHeader(HTTP_ACCEPT_LANG, HTTP_ACCEPT_LANG_VAL, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup UAProf", 0, __FILE__, __LINE__);
    // If a User Agent Profile is set, send it
    if (request->UAProf) {
        char    *profile = (char *)memMgrChunkNew(stringLen(request->UAProf) + 3);
        
        if (profile) {
            stringPrintF(profile, "\"%s\"", request->UAProf);
            httpRequestAddHeader(HTTP_X_WAP_PROFILE, profile, request);
            
            memMgrChunkFree(profile);
        }
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup accept", 0, __FILE__, __LINE__);
    // Send accptable content types
    httpRequestAddHeader(HTTP_ACCEPT, HTTP_ACCEPT_VALUE, request);
    
    /*debugOutFunctionFormattedV0(__FUNCTION__, "setup cookies", 0, __FILE__, __LINE__);
    if (request->cookies && request->g) {
        char    *cookieHeader = NULL;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "create cookie header", 0, __FILE__, __LINE__);
        cookieHeader = CookieCreateCookieHeader(request->cookies,
                            (GlobalsType *)request->g);
        if (cookieHeader) {
            debugOutFunctionFormattedV0(__FUNCTION__, "append cookie header", 0, __FILE__, __LINE__);
            
            httpAddToHeader(HTTP_COOKIE_HDR, request);
            httpAddToHeader(cookieHeader, request);
            httpAddToHeader(HTTP_LINE_ENDING, request);
            memMgrChunkFree(cookieHeader);
        }
    }*/
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup post data", 0, __FILE__, __LINE__);
    // Send the post data, including a content length
    if (request->requestType == HTTP_POST) {
        HTTPPostRequest *post = &request->post;
        char            contentLenStr[50];
        
        httpRequestAddHeader(HTTP_CONTENTTYPE, post->dataTypeP, request);
        
        debugOutFunctionFormattedV0(__FUNCTION__, "add date length", 0, __FILE__, __LINE__);
        
        memMgrChunkSet(contentLenStr, 0, sizeof(contentLenStr));
        stringPrintF(contentLenStr, "%ld", post->dataLength);
        httpRequestAddHeader(HTTP_CONTENTLENGTH, contentLenStr, request);
        
        /*debugOutFunctionFormattedV0(__FUNCTION__, "copy data", 0, __FILE__, __LINE__);
        size = request->requestHeaderLength; // MemPtrSize(request->requestHeader);
        tempHeader = memMgrChunkRealloc(request->requestHeader, size + post->dataLength);
        if (!tempHeader) {
            memMgrChunkFree(request->requestHeader);
            return memErrNotEnoughSpace;
        }
            
        memMgrChunkMove(tempHeader + size, post->dataP, post->dataLength);
        request->requestHeader = tempHeader;
        request->requestHeaderLength += post->dataLength;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "post data setup", 0, __FILE__, __LINE__);*/
    }

    return systemErrNone;
}

int httpCreateRequestHeader(HTTPRequestPtr request)
{
    int     error = systemErrNone;
    URLPtr  url = NULL;
    char    httpVersion[11];
    char    *urlStr = NULL;
    boolean encoded = false;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }

#ifdef CONFIG_HTTP_OWN_URL
    url = request->url;
#else
    url = httpGetURL(request);
#endif
    
    debugOutFunctionFormattedV0(__FUNCTION__, "method", request->requestType, __FILE__, __LINE__);
    // Set the request type, GET,POST,HEAD,PUT
    switch (request->requestType) {
        case HTTP_GET:
            request->requestHeader = (unsigned char *)stringDup(HTTP_GET_METH);
            break;
        case HTTP_POST:
            request->requestHeader = (unsigned char *)stringDup(HTTP_POST_METH);
            break;
        case HTTP_HEAD:
            request->requestHeader = (unsigned char *)stringDup(HTTP_HEAD_METH);
            break;
        case HTTP_PUT:
            request->requestHeader = (unsigned char *)stringDup(HTTP_PUT_METH);
            break;
        default:
            break;
    }
    request->requestHeaderLength = stringLen((char *)request->requestHeader);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup version", 0, __FILE__, __LINE__);
    // set the HTTP version, " HTTP/x.x"
    stringPrintF(httpVersion, "%d.%d", request->httpVersionMajor,
              request->httpVersionMinor);
    if (request->carrier == HTTP_PROXY) {
        //httpAddToHeader(url->full, request);
        urlStr = url->full;
    } else {
        //httpAddToHeader(url->path, request);
        urlStr = url->path;
    }
    encoded = urlIsEncoded(urlStr, true);
    if (!encoded) {
        urlStr = urlEncodeURL(urlStr, true);
    }
    httpAddToHeader(urlStr, request);
    if (!encoded) {
        memMgrChunkFree(urlStr);
        urlStr = NULL;
    }
    httpAddToHeader(HTTP_VERSION_PREFIX, request);
    httpAddToHeader(httpVersion, request);
    httpAddToHeader(HTTP_LINE_ENDING, request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup host", 0, __FILE__, __LINE__);
    // Set the host (include port if passing thorugh proxy)
    if ((request->carrier == HTTP_PROXY) || !httpIsDefaultPort(request)) {
        char            temp[50];
        char            *host;
        unsigned long   len = 0;
        
        stringPrintF(temp, "%d", url->port);
        
        len = stringLen(url->host) + stringLen(temp) + 2;
        
        host = (char *)memMgrChunkNew(len);
        if (!host) {
            httpRequestAddHeader(HTTP_HOST, url->host, request);
        } else {
            memMgrChunkSet(host, 0, len);
            stringCopy(host, url->host);
            stringCopy(host + stringLen(url->host), ":");
            stringCopy(host + stringLen(url->host) + 1, temp);
            
            httpRequestAddHeader(HTTP_HOST, host, request);
            
            memMgrChunkFree(host);
        }
    } else {
        httpRequestAddHeader(HTTP_HOST, url->host, request);
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "setup proxy auth", 0, __FILE__, __LINE__);
    // If we are connecting through a proxy set the user name and password
    /*if (request->carrier == HTTP_PROXY) {
        char                        auth[512];
        struct base64BufferType     encoded;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "create enc", 0, __FILE__, __LINE__);
        stringPrintF(auth, "%s:%s", request->proxy.userName, request->proxy.password);
        base64Encode(&encoded, auth, stringLen(auth));
        
        debugOutFunctionFormattedV0(__FUNCTION__, "copy proxy auth", 0, __FILE__, __LINE__);
        httpAddToHeader(HTTP_PROXY_AUTH, request);
        httpAddToHeader(encoded.data, request);
        httpAddToHeader(HTTP_LINE_ENDING, request);
        
        debugOutFunctionFormattedV0(__FUNCTION__, "delete enc", 0, __FILE__, __LINE__);
        base64BufferDelete(&encoded);
    }*/
    
    debugOutFunctionFormattedV0(__FUNCTION__, "handle methods", 0, __FILE__, __LINE__);
    // handle individual methods (TODO: I think a switch would look "prettier"
    if ((request->requestType == HTTP_GET) ||
        (request->requestType == HTTP_POST)) {
        debugOutFunctionFormattedV0(__FUNCTION__, "handle get/post", 0, __FILE__, __LINE__);
        error = httpCreateRequestGETPOSTHeader(request);
    } else if (request->requestType == HTTP_HEAD) {
        debugOutFunctionFormattedV0(__FUNCTION__, "handle head", 0, __FILE__, __LINE__);
        error = httpErrUnsupportedMethod;
    } else if (request->requestType == HTTP_PUT) {
        debugOutFunctionFormattedV0(__FUNCTION__, "handle put", 0, __FILE__, __LINE__);
        error = httpErrUnsupportedMethod;
    } else if (request->requestType == HTTP_TRACE) {
        debugOutFunctionFormattedV0(__FUNCTION__, "handle trace", 0, __FILE__, __LINE__);
        error = httpErrUnsupportedMethod;
    } else if (request->requestType == HTTP_OPTION) {
        debugOutFunctionFormattedV0(__FUNCTION__, "handle option", 0, __FILE__, __LINE__);
        error = httpErrUnsupportedMethod;
    } else {
        debugOutFunctionFormattedV0(__FUNCTION__, "invalid method", 0, __FILE__, __LINE__);
        error = httpErrInvalidMethod;
    }

    return error;
}

int httpRequestCompleteHeader(HTTPRequestPtr request)
{
    int     error = systemErrNone;
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    httpBuildRequestHeader(request);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "last line end", 0, __FILE__, __LINE__);
    // Tack on the last line ending
    httpAddToHeader(HTTP_LINE_ENDING, request);
    
    if ((request->requestType == HTTP_POST) && request->post.dataP) {
        HTTPPostRequest *post = &request->post;
        unsigned long   size = 0;
        unsigned char   *tempHeader = NULL;
        
        
        debugOutFunctionFormattedV0(__FUNCTION__, "copy data", 0, __FILE__, __LINE__);
        size = request->requestHeaderLength;
        tempHeader = memMgrChunkRealloc(request->requestHeader, size + post->dataLength);
        if (!tempHeader) {
            memMgrChunkFree(request->requestHeader);
            return memErrNotEnoughSpace;
        }
        
        memMgrChunkMove(tempHeader + size, post->dataP, post->dataLength);
        request->requestHeader = tempHeader;
        request->requestHeaderLength += post->dataLength;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "post data setup", 0, __FILE__, __LINE__);
    }
    
    
    if (error != systemErrNone) {
        debugOutFunctionFormattedV0(__FUNCTION__, "error", error, __FILE__, __LINE__);
        memMgrChunkFree(request->requestHeader);
        request->requestHeader = NULL;
    } else if (!request->requestHeader) {
        error = httpErrGenericRequestError;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "bottom", 0, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, request->requestHeader, 0, __FILE__, __LINE__);
    
    return error;
}


int httpOpenRequest(HTTPRequestPtr request)
{
    int             error   = systemErrNone;
    NetSessionPtr   session = loaderGetNetSession(request->loader);
    URLPtr          url     = NULL;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "set url", 0, __FILE__, __LINE__);

#ifdef CONFIG_HTTP_OWN_URL
    url = request->url;
#else
    url = httpGetURL(request);
#endif
    
    debugOutFunctionFormattedV0(__FUNCTION__, "open network", 0, __FILE__, __LINE__);

    session = netOpenNetwork(session);
    if (!session) {
        error = networkErrSessionNotCreated; //netGetError(session);
        debugOutFunctionFormattedV0(__FUNCTION__, "net not open", 0, __FILE__, __LINE__);
        goto httpOpenRequest_cleanup;
    }
    
    if (netGetError(session)) {
        error = netGetError(session);
        netCloseNetwork(&session, true);
        debugOutFunctionFormattedV0(__FUNCTION__, "net not open", 0, __FILE__, __LINE__);
        goto httpOpenRequest_cleanup;
    }
    
    if (loaderGetNetSession(request->loader) != session) {
        loaderSetNetSession(session, request->loader);
    }
    
    netSetTimeout(session, loaderGetTimeout(request->loader));
    
    debugOutFunctionFormattedV0(__FUNCTION__, "creat sock", 0, __FILE__, __LINE__);
    
    error = netConnectionEstablish(session, (request->carrier == HTTP_PROXY)?request->proxy.address:url->host,
                        NULL, (request->carrier == HTTP_PROXY)?request->proxy.port:url->port);
    if (error != systemErrNone) {
        debugOutFunctionFormattedV0(__FUNCTION__, "connect error", 0, __FILE__, __LINE__);
        goto httpOpenRequest_cleanup;
    }
    if (session->socket < 0) {
        debugOutFunctionFormattedV0(__FUNCTION__, "bad sock", 0, __FILE__, __LINE__);
        error = networkErrSocketNotOpen;
        goto httpOpenRequest_cleanup;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "return", 0, __FILE__, __LINE__);
    
httpOpenRequest_cleanup:
    if (error != systemErrNone)
        httpSetError(error, request);
    
    return error;
}

int httpSendRequest(HTTPRequestPtr request)
{
    NetSessionPtr   session     = loaderGetNetSession(httpGetLoader(request));
    unsigned long   written     = 0;
    unsigned long   length      = 0;
    unsigned char   *data       = NULL;
    long            thisWrite;
    int             err         = systemErrNone;
    unsigned long   maxTimer    = 0;
    //unsigned long   timeout     = loaderGetTimeout(httpGetLoader(request)) * timeGetTicksPerSecond();
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request || !session) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "Setting variables", 0, __FILE__, __LINE__);

    data    = request->requestHeader;
    length  = request->requestHeaderLength;

    debugOutFunctionFormattedV0(__FUNCTION__, "sending data", length, __FILE__, __LINE__);

    maxTimer = timeGetCurrentTick() + (timeGetTicksPerSecond() * loaderGetTimeout(httpGetLoader(request)));

    while (written < length) {
        debugOutFunctionFormattedV0(__FUNCTION__, "send data chunk", written, __FILE__, __LINE__);
        

        thisWrite = netSendData(session, data + written, length - written, &err);

        if ((thisWrite == 0) || ((thisWrite < 0) &&
                    !((err == networkErrTimeout) && (maxTimer > timeGetCurrentTick())))) {
                debugOutFunctionFormattedV0(__FUNCTION__, "send error", err, __FILE__, __LINE__);
                
                httpSetError(err, request);
                goto httpSendRequest_cleanup;
        }
        
        written += thisWrite;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "sent", written, __FILE__, __LINE__);
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "result", err, __FILE__, __LINE__);
    
    request->state = HTTP_Parse_Status;
    
    httpSetResponseTimeout(request);

httpSendRequest_cleanup:
    return err;
}

static unsigned char *httpGetOffset(HTTPRequestPtr request)
{
    return request->netData + request->netDataSize;
}

static unsigned long httpGetSizeLeft(HTTPRequestPtr request)
{
    return HTTP_BUF_SIZE - request->netDataSize;
}

static void httpUpdateSize(unsigned long size, HTTPRequestPtr request)
{
    request->netDataSize += size;
}

static void httpShrinkSize(unsigned long reduce, HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (reduce > request->netDataSize) {
        debugOutFunctionFormattedV0(__FUNCTION__, "request larger than size!", 0, __FILE__, __LINE__);
        return;
    }
    
    if (!reduce) {
        debugOutFunctionFormattedV0(__FUNCTION__, "!reduce", request->netDataSize, __FILE__, __LINE__);
        return;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "netDataSize", request->netDataSize, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, "reduce", reduce, __FILE__, __LINE__);
    
    // Resize size
    request->netDataSize -= reduce;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "new netDataSize", request->netDataSize, __FILE__, __LINE__);
    
    // Move data forward
    if (request->netDataSize) {
        debugOutFunctionFormattedV0(__FUNCTION__, "size left", request->netDataSize, __FILE__, __LINE__);
        
        debugOutFunctionFormattedV0(__FUNCTION__, "move end data forward", 0, __FILE__, __LINE__);
        memMgrChunkMove(request->netData,
                        request->netData + reduce, request->netDataSize);
                    
        debugOutFunctionFormattedV0(__FUNCTION__, "clear out old data", 0, __FILE__, __LINE__);
        memMgrChunkSet(request->netData + request->netDataSize,
                        0, HTTP_BUF_SIZE - request->netDataSize);

        debugOutFunctionFormattedV0(__FUNCTION__, "cleared", 0, __FILE__, __LINE__);
    } else {
        debugOutFunctionFormattedV0(__FUNCTION__, "no size left", 0, __FILE__, __LINE__);
        
        memMgrChunkSet(request->netData, 0, HTTP_BUF_SIZE);
    }
    
    //debugOutFunctionFormattedV0(__FUNCTION__, request->netData, request->netDataSize, __FILE__, __LINE__);
}

static char *httpSearchToken(unsigned char token, unsigned char *string, unsigned long length)
{
    unsigned char   *temp = string;
    
    while ((*temp != token) &&
           ((unsigned long)temp < ((unsigned long)string + (unsigned long)length)))
        temp++;
    
    if ((unsigned long)temp == ((unsigned long)string + (unsigned long)length)) {
        return NULL;
    }
    
    return (char *)temp;
}

static char *httpSearchString(char *token, char *string, unsigned long length)
{
    char   *search = NULL;
    char   *pos    = NULL;
    long   offset  = 0;
    
    search = memMgrChunkNew(length + 1);
    if (!search) {
        return NULL;
    }
    
    stringNCopy(search, string, length);
    search[length] = '\0';
    
    pos = stringStr(search, token);
    if (!pos) {
        memMgrChunkFree(search);
        return NULL;
    }
    
    offset = (unsigned long)pos - (unsigned long)search;
    
    memMgrChunkFree(search);
    
    return string + offset;
}

int httpSetResponseTimeout(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    request->stopTicks = timeGetCurrentTick() +
                         ((loaderGetTimeout(httpGetLoader(request)) + 1) * timeGetTicksPerSecond()); // Add 1 second to deal with any delays
    
    return systemErrNone;
}

unsigned long httpGetTimeoutTime(HTTPRequestPtr request)
{
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return 0; // bad?? --It should kill any process looking for a tick since XXX count
    }
    
    return request->stopTicks;
}

int httpReceiveResponse(HTTPRequestPtr request)
{
    NetSessionPtr   session     = loaderGetNetSession(request->loader);
    long            readRes     = 0;
    int             err         = systemErrNone;
#ifdef INTERNAL_RECEIVE_LOOP
    unsigned long   maxTimer    = 0;
    unsigned short  length      = httpGetSizeLeft(request);
#endif
    //unsigned long   timeout     = NET_BLOCK_TIMEOUT * timeGetTicksPerSecond();
    unsigned char   *bufP       = NULL;
    unsigned short  bufSize     = 0;

    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    if (request->remoteClosed) {
        return systemErrNone;
    }


#ifdef INTERNAL_RECEIVE_LOOP
    maxTimer = timeGetCurrentTick() + (timeGetTicksPerSecond() * loaderGetTimeout(httpGetLoader(request)));

    while (written < length) {
#endif
        bufP    = httpGetOffset(request);
        bufSize = (unsigned short)httpGetSizeLeft(request);
        
        if (!bufSize) {
            return systemErrNone; // do not need more data
        }

        debugOutFunctionFormattedV0(__FUNCTION__, "receiving data, size left", bufSize, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, "receiving data, buf addy", (unsigned long)bufP, __FILE__, __LINE__);

        readRes = netReceiveData(session, bufP, bufSize, &err);

        if (readRes < 0) {
            if (err == networkErrTimeout) {
#ifdef INTERNAL_RECEIVE_LOOP
                if (timeGetCurrentTick() > maxTimer) {
#else
                if (timeGetCurrentTick() > httpGetTimeoutTime(request)) {
#endif
                    httpSetError(err, request);
                    debugOutFunctionFormattedV0(__FUNCTION__, "error", err, __FILE__, __LINE__);
                }
            } else {
                httpSetError(err, request);
                debugOutFunctionFormattedV0(__FUNCTION__, "error", err, __FILE__, __LINE__);
            }
            
            goto httpReceiveResponse_cleanup;
        }
#ifdef INTERNAL_RECEIVE_LOOP
    }
#endif

    debugOutFunctionFormattedV0(__FUNCTION__, "received", readRes, __FILE__, __LINE__);

    if (err == systemErrNone) {
        if (readRes == 0) {
            request->remoteClosed = true;
        } else {
            httpUpdateSize(readRes, request);
        }
    }


httpReceiveResponse_cleanup:
    return err;

}

#ifdef HTTP_GETLINE_TESTER
void httpTestGetLine(HTTPRequestPtr request)
{
    char    *h1     = NULL;
    char    *h2     = NULL;
    char    *h3     = NULL;
    char    temp[200];
    char    *header = NULL;
    char    *value  = NULL;
    
    //request->netData        = memMgrChunkNew(HTTP_BUF_SIZE + 1);
    if (!request->netData) {
        return;
    }
    stringCopy(request->netData, "HTTP/1.1 202 OK\r\nContent-length: 0\nContent-type: text/plain\r\n\r\n");
    request->netDataSize    = stringLen(request->netData);
    
    httpParseStatus(request);
    
    {
        char    status[200];
        
        stringPrintF(status, "vers,mj:%d vers,mi:%d  code:%d msg:%s",
                  request->serverHttpVerMajor, request->serverHttpVerMinor,
                  request->serverStatus, request->serverStatusVal);
        MyErrorFunc(status, "status");
    }
    
    h1 = httpGetLine(request);
    h2 = httpGetLine(request);
    h3 = httpGetLine(request);

    stringPrintF(temp, "h1:%s h2:%s h3:%s", (h1)?h1:"-", (h2)?h2:"-", (h3)?h3:"-");
    MyErrorFunc(temp, NULL);
    
    
    httpSplitHeader(h2, &header, &value);
    
    stringPrintF(temp, "header: %s  value: %s", (header)?header:"-", (value)?value:"-");
    MyErrorFunc(temp, NULL);
    
    memMgrChunkFree(h1);
    memMgrChunkFree(h2);
    memMgrChunkFree(h3);
    memMgrChunkFree(header);
    memMgrChunkFree(value);
}
#endif


static char *httpGetLine(HTTPRequestPtr request)
{
    unsigned char   *data   = request->netData;
    char            *line   = NULL;
    char            *eol    = NULL;
    unsigned long   length  = request->netDataSize;
    unsigned long   lineLen = 0;
    char            lf      = 1;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return NULL;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "find crlf", 0, __FILE__, __LINE__);
    
    eol = httpSearchString(HTTP_CRLF, (char *)data, length);
    if (!eol) {
        debugOutFunctionFormattedV0(__FUNCTION__, "find lf", 0, __FILE__, __LINE__);
        
        eol = httpSearchToken(HTTP_LF, data, length);
        if (!eol) {
            debugOutFunctionFormattedV0(__FUNCTION__, "find cr", 0, __FILE__, __LINE__);
            
            eol = httpSearchToken(HTTP_LF, data, length);
            if (!eol) {
                debugOutFunctionFormattedV0(__FUNCTION__, "none found!", 0, __FILE__, __LINE__);
                
                return NULL;
            }
        }
    } else {
        char    *lineFeed = httpSearchToken(HTTP_LF, data, length);
        
        if (lineFeed && (lineFeed < eol)) {
            debugOutFunctionFormattedV0(__FUNCTION__, "have lf sooner", 0, __FILE__, __LINE__);
            
            eol = lineFeed;
        } else {
            lf++;
        }
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "found 1", 0, __FILE__, __LINE__);
    
    lineLen = (unsigned long)eol - (unsigned long)data;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "len", lineLen, __FILE__, __LINE__);
    
    line = (char *)memMgrChunkNew(lineLen + 1);
    if (!line) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no mem", 0, __FILE__, __LINE__);
        httpSetError(memErrNotEnoughSpace, request);
        return NULL;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "copy", 0, __FILE__, __LINE__);
    
    stringNCopy(line, (char *)data, lineLen);
    line[lineLen] = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, "advance", 0, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, line, 0, __FILE__, __LINE__);
    
    // Now that we have eaten a line, incriment the counters...
    httpShrinkSize(lineLen + lf, request);
    
    return line;
}


// Expects a line of:
//  HTTP/<ver maj>.<ver min> <status code> <status msg>
// Seems a bit long for a simple function...
static int httpParseStatus(HTTPRequestPtr request)
{
    char    *line           = httpGetLine(request);
    char    num[11];
    unsigned long  pos             = 0;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    // We do not have the first line, so just ignore
    if (!line) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no line", 0, __FILE__, __LINE__);
        return systemErrNone;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "Check prefix", 0, __FILE__, __LINE__);
    
    if (stringNCompare(line, HTTP_PREFIX, stringLen(HTTP_PREFIX))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "Malformed!", 0, __FILE__, __LINE__);
        memMgrChunkFree(line);
        return httpSetError(httpErrMalformedHeader, request);
    }
    pos += stringLen(HTTP_PREFIX);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "check version maj", 0, __FILE__, __LINE__);
    
    num[0] = line[pos++];
    num[1] = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, num, 0, __FILE__, __LINE__);

    request->serverHttpVerMajor = (unsigned short)stringAToI(num);
    debugOutFunctionFormattedV0(__FUNCTION__, num, request->serverHttpVerMajor, __FILE__, __LINE__);
    
    if (line[pos++] != '.') {
        debugOutFunctionFormattedV0(__FUNCTION__, "Malformed!", 0, __FILE__, __LINE__);
        memMgrChunkFree(line);
        return httpSetError(httpErrMalformedHeader, request);
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "check version min", 0, __FILE__, __LINE__);
    
    num[0] = line[pos++];
    num[1] = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, num, 0, __FILE__, __LINE__);

    request->serverHttpVerMinor = (unsigned short)stringAToI(num);
    debugOutFunctionFormattedV0(__FUNCTION__, num, request->serverHttpVerMinor, __FILE__, __LINE__);

    while (httpIsWhitespace(line[pos])) {
        pos++;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "check status", 0, __FILE__, __LINE__);
    
    stringNCopy(num, line + pos, 3);
    pos += 3;
    num[3] = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, num, 0, __FILE__, __LINE__);

    request->serverStatus = (unsigned short)stringAToI(num);
    debugOutFunctionFormattedV0(__FUNCTION__, num, request->serverStatus, __FILE__, __LINE__);
                    
    while (httpIsWhitespace(line[pos])) {
        pos++;
    }
    
    stringCopy(request->serverStatusVal, line + pos);
    request->serverStatusVal[stringLen(line) - pos] = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, "status val", 0, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, request->serverStatusVal, request->serverStatus, __FILE__, __LINE__);
    
    memMgrChunkFree(line);
    
    request->state = HTTP_Parse_Header; // Move to next state
    
    return systemErrNone;
}

static int httpSessionAddHeader(char *header, char *value,
                                HTTPHeaderPtr *headersP)
{
    HTTPHeaderPtr       temp = NULL;

    debugOutFunctionFormattedV0(__FUNCTION__, "top", 0, __FILE__, __LINE__);
    
    if (!header || !value) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invalid header", 0, __FILE__, __LINE__);
        return httpErrParameterError;
    }

    temp = (HTTPHeaderPtr)memMgrChunkNew(sizeof(HTTPHeaderType));
    if (!temp) {
        return memoryErrNoFreeMemory;
    }
    
    temp->header    = stringDup(header);
    temp->value     = stringDup(value);
    temp->next      = NULL;
    
    if (!*headersP) {
        *headersP = temp;
    } else {
        HTTPHeaderPtr   last = *headersP;
        
        while (last->next)
            last = last->next;
        
        last->next = temp;
    }
    
    return systemErrNone;
}

static void httpSessionDeleteHeaders(HTTPHeaderPtr *headersP)
{
    if (!*headersP)
        return;
    
    while (*headersP) {
        HTTPHeaderPtr   temp = *headersP;
        
        if (temp->header)
            memMgrChunkFree(temp->header);
        if (temp->value)
            memMgrChunkFree(temp->value);
        
        *headersP = temp->next;
        
        memMgrChunkSet(temp, 0, sizeof(HTTPHeaderType));
        memMgrChunkFree(temp);
    }
    
    *headersP = NULL;
}

HTTPHeaderPtr httpGetResponseHeaders(HTTPRequestPtr request)
{
    return (request)?request->headersResponse:NULL;
}

HTTPHeaderPtr httpGetRequestHeaders(HTTPRequestPtr request)
{
    return (request)?request->headersRequest:NULL;
}

char *httpFindHeader(char *header, HTTPHeaderPtr headersP)
{
    HTTPHeaderPtr   headerList  = headersP;
    
    while (headerList) {
        if (!stringCompareCaseless(headerList->header, header)) {
            return headerList->value;
        }
        
        headerList = headerList->next;
    }
    
    return NULL;
}

static int httpSplitHeader(char *line, char **header, char **value)
{
    char    *tokPos     = NULL;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", stringLen(line), __FILE__, __LINE__);
    
    tokPos = stringChr(line, HTTP_HEADER_SEP);
    if (!tokPos) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no,  sep found", 0, __FILE__, __LINE__);
        return httpErrMalformedHeader;
    }
    *tokPos = '\0';
    
    debugOutFunctionFormattedV0(__FUNCTION__, "dup", 0, __FILE__, __LINE__);
    
    *header = stringDup(line);
    if (!*header) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no mem for header", 0, __FILE__, __LINE__);
        return memoryErrNoFreeMemory;
    }
    debugOutFunctionFormattedV0(__FUNCTION__, "duped", 0, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, "offset", (unsigned long)tokPos - (unsigned long)line, __FILE__, __LINE__);
    
    debugOutFunctionFormattedV0(__FUNCTION__, *header, 0, __FILE__, __LINE__);
    
    tokPos++;
    
    while (httpIsWhitespace(*tokPos)) {
        tokPos++;
    }
        
    debugOutFunctionFormattedV0(__FUNCTION__, "dup", 0, __FILE__, __LINE__);
    
    *value = stringDup(tokPos);
    if (!*value) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no mem for val", 0, __FILE__, __LINE__);
        memMgrChunkFree(*header);
        return memoryErrNoFreeMemory;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, *value, 0, __FILE__, __LINE__);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "bottom", 0, __FILE__, __LINE__);
    
    return systemErrNone;
}

static int httpCallHeaderReceiveCallback(HTTPCallbackStateEnum state, char *header,
                    char *value, HTTPCallbackType *callDataP,
                    HTTPRequestPtr request)
{
    int     error = systemErrNone;

    if (!request->headerReceiveCallbackP) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no callback", 0, __FILE__, __LINE__);
        return httpErrInvalidCallback;
    }

    callDataP->handled   = false;
    callDataP->request   = request;
    callDataP->state     = state;

    if (header) {
        debugOutFunctionFormattedV0(__FUNCTION__, "header", 0, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, header, 0, __FILE__, __LINE__);
    }
    if (value) {
        debugOutFunctionFormattedV0(__FUNCTION__, "value", 0, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, value, 0, __FILE__, __LINE__);
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);

    error = request->headerReceiveCallbackP(header, value,
                                        httpGetWebData(request), callDataP);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "called", 0, __FILE__, __LINE__);
                        
    return error;
}

static int httpCallDataReceiveCallback(HTTPCallbackStateEnum state,
                        HTTPCallbackType *callDataP, HTTPRequestPtr request)
{
    int     error = systemErrNone;

    callDataP->handled   = false;
    callDataP->request   = request;
    callDataP->state     = state;

    if (!request->dataReceiveCallbackP) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no callback", 0, __FILE__, __LINE__);
        return httpErrInvalidCallback;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);

    error = request->dataReceiveCallbackP(request->netData,
                        request->netDataSize, httpGetWebData(request), callDataP);
                        
    debugOutFunctionFormattedV0(__FUNCTION__, "called", 0, __FILE__, __LINE__);
    
    return error;
}

static int httpParseHeader(HTTPRequestPtr request)
{
    char                *line       = httpGetLine(request);
    char                *header     = NULL;
    char                *value      = NULL;
    int                 error       = systemErrNone;
    HTTPCallbackType    callData;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    // We do not have the first line, so just ignore
    if (!line) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no line", 0, __FILE__, __LINE__);
        return systemErrNone;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "have line:", (long)line, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, line, 0, __FILE__, __LINE__);
    
    // check to see if we are done with the header block
    if (stringLen(line) == 0) {
        debugOutFunctionFormattedV0(__FUNCTION__, "done with headers", 0, __FILE__, __LINE__);

        /*callData.handled    = false;
        callData.request    = request;
        callData.state      = HTTP_Header_Done;
    
        if (request->headerReceiveCallbackP &&
                httpIsResponseStatusOK(request)) {
            debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);
            
            error = request->headerReceiveCallbackP(NULL, NULL,
                                    httpGetWebData(request), &callData);
            if (error) {
                return httpSetError(error, request);
            }
        }*/
        
        if (request->headerReceiveCallbackP) {
            error = httpCallHeaderReceiveCallback(HTTP_Header_Done, NULL, NULL,
                        &callData, request);
                        
            if (error != systemErrNone) {
                memMgrChunkFree(line);
                return httpSetError(error, request);
            }
        }

        memMgrChunkFree(line);
        
        request->state = HTTP_Parse_Data; // Move to next state
        return systemErrNone;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "splitting header", 0, __FILE__, __LINE__);
    
    error = httpSplitHeader(line, &header, &value); // should give us a bone
    if (!header || !value) {
        httpSetError(error, request);
        goto httpParseHeader_cleanup;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, header, 0, __FILE__, __LINE__);
    debugOutFunctionFormattedV0(__FUNCTION__, value, 0, __FILE__, __LINE__);
    
    //debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);
    
    //callData.handled    = false;
    //callData.request    = request;
    //callData.state      = HTTP_Receive_Header;
    
    // Add the header to the array, for possible future inspection??
    error = httpSessionAddHeader(header, value,
                                &request->headersResponse);//,
                                //&request->headersResponseCount);
    
    if (request->headerReceiveCallbackP) {
        debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);
            
        /*error = request->headerReceiveCallbackP(header, value,
                                    httpGetWebData(request), &callData);
        if (error) {
            return httpSetError(error, request);
        }*/
        
        error = httpCallHeaderReceiveCallback(HTTP_Receive_Header, header, value,
                    &callData, request);
                        
        if (error != systemErrNone) {
            memMgrChunkFree(line);
            return httpSetError(error, request);
        }
        
        if (callData.handled) {
            goto httpParseHeader_cleanup;
        }
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "handling internally", 0, __FILE__, __LINE__);
    
    if (!stringNCompareCaseless(header, HTTP_CONTENTLENGTH,
                        stringLen(HTTP_CONTENTLENGTH))) {
        unsigned long  length = stringAToI(value);
        
#if 0
        // Why did I put this here??
        if (length < 0) {
            error = httpErrMalformedHeader;
            httpSetError(error, request);
            goto httpParseHeader_cleanup;
        }
#endif
        
        debugOutFunctionFormattedV0(__FUNCTION__, "content length", length, __FILE__, __LINE__);
        
        httpSetContentLength(length, request);
    } else if (!stringNCompareCaseless(header, HTTP_CONTENTTYPE,
                    stringLen(HTTP_CONTENTTYPE))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "content type", 0, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, value, 0, __FILE__, __LINE__);
        
        httpSetContentType(value, request);
    } else if (!stringNCompareCaseless(header, HTTP_LOCATION,
                    stringLen(HTTP_LOCATION))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "location", 0, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, value, 0, __FILE__, __LINE__);
        
        httpSetResponseRedirectURL(value, request);
    } /*else {
        debugOutFunctionFormattedV0(__FUNCTION__, "unused header", 0, __FILE__, __LINE__);
        
        if (request->headerReceiveCallbackP) {
            debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);
            
            error = request->headerReceiveCallbackP(header, value, httpGetWebData(request));
        }
    }*/

    debugOutFunctionFormattedV0(__FUNCTION__, "bottom", 0, __FILE__, __LINE__);
    
httpParseHeader_cleanup:
    memMgrChunkFree(line);
    memMgrChunkFree(header);
    memMgrChunkFree(value);

    return error;
}

static int httpMoveData(unsigned char *content, unsigned long *used, HTTPRequestPtr request)
{
    int     error = systemErrNone;
    
    if ((error = memMgrChunkMove(content + request->dataHandled, request->netData,
                    request->netDataSize)) == systemErrNone) {
            *used = request->netDataSize;
    } else {
        debugOutFunctionFormattedV0(__FUNCTION__, "memory error", 0, __FILE__, __LINE__);
        return httpSetError(error, request);
    }
    
    return error;
}

static int httpParseData(HTTPRequestPtr request)
{
    int                 error       = systemErrNone;
    HTTPCallbackType    callData;
    unsigned long       used        = 0;
    unsigned char       *content    = NULL;
    unsigned long       length      = webDataGetContentLength(httpGetWebData(request));
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request->netDataSize) {
        return systemErrNone;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "test callback", 0, __FILE__, __LINE__);
    
    /*callData.handled    = false;
    callData.request    = request;
    callData.state      = HTTP_Receive_Data;
    
    if (request->dataReceiveCallbackP) {
        debugOutFunctionFormattedV0(__FUNCTION__, "calling callback", 0, __FILE__, __LINE__);
            
        error = request->dataReceiveCallbackP(request->netData,
                        request->netDataSize, httpGetWebData(request), &callData);
        
        debugOutFunctionFormattedV0(__FUNCTION__, "called", 0, __FILE__, __LINE__);
        if (error) {
            return httpSetError(error, request);
        }
    }*/

    if (request->dataReceiveCallbackP) {
        error = httpCallDataReceiveCallback(HTTP_Receive_Data,
                        &callData, request);
                        
        if (error != systemErrNone) {
            return httpSetError(error, request);
        }
        
        if (callData.handled) {
            used = callData.usedDataSize;
        
            goto httpParseData_cleanup;
        }
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "post callback", 0, __FILE__, __LINE__);
    
    if (!request->netDataSize) {
        return systemErrNone;
    }
    
    if (!httpIsResponseStatusOK(request) &&
        !webDataGetContentLength(httpGetWebData(request))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "non-OK response w/no data", 0,  __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, "HTTP non-200 status", 0, __FILE__, __LINE__);
        // It is up to the client to determine if this is an error or not
        request->state = HTTP_Done;
        
        if (request->dataReceiveCallbackP) {
            error = httpCallDataReceiveCallback(HTTP_Data_Done,
                        &callData, request);
                        
            if (error != systemErrNone) {
                return httpSetError(error, request);
            }
        }
        
        return httpErrStatusNotOK;
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "prep handle", 0, __FILE__, __LINE__);

    content = webDataGetContent(httpGetWebData(request));
    
    if (length && !content) {
        debugOutFunctionFormattedV0(__FUNCTION__, "length && !content", length, __FILE__, __LINE__);
        
        webDataSetContent(memMgrChunkNew(length), httpGetWebData(request));
        
        content = webDataGetContent(httpGetWebData(request));
        if (!content) {
            debugOutFunctionFormattedV0(__FUNCTION__, "no mem for data", 0, __FILE__, __LINE__);
            
            return httpSetError(memErrNotEnoughSpace, request); // Move to next state
        }
    }
    
    content = webDataGetContent(httpGetWebData(request));
    length  = webDataGetContentLength(httpGetWebData(request));
    
    if (!length || webDataGetNoPredefLength(httpGetWebData(request))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no length", 0, __FILE__, __LINE__);
        
        length += request->netDataSize;
        
        debugOutFunctionFormattedV0(__FUNCTION__, "no length", length, __FILE__, __LINE__);
        
        webDataSetContentNoFree(memMgrChunkRealloc(content, length),
                          httpGetWebData(request)); 
                          
        debugOutFunctionFormattedV0(__FUNCTION__, "attempted relocation", 0, __FILE__, __LINE__);
        
        content = webDataGetContent(httpGetWebData(request));
        if (!content) { // || (memMgrChunkSize(content) != length)) { // we have no new size
            debugOutFunctionFormattedV0(__FUNCTION__, "no mem for data", 0, __FILE__, __LINE__);
            
            return httpSetError(memErrNotEnoughSpace, request); // Move to next state
        }
        
        debugOutFunctionFormattedV0(__FUNCTION__, "update vals", 0, __FILE__, __LINE__);

        webDataSetContentLength(length, httpGetWebData(request));
        webDataSetNoPredefLength(true, httpGetWebData(request));
    } else if ((request->dataHandled + request->netDataSize) > length) {
        char    temp[50];
        
        debugOutFunctionFormattedV0(__FUNCTION__, "no mem for val", 0, __FILE__, __LINE__);
        
        stringPrintF(temp, "vals %lu %lu %lu", request->dataHandled, request->netDataSize, length);
        debugOutFunctionFormattedV0(__FUNCTION__, temp, 0, __FILE__, __LINE__);
        
        return httpSetError(memErrNotEnoughSpace, request); // Move to next state
    }
    
    debugOutFunctionFormattedV0(__FUNCTION__, "handling internally", 0, __FILE__, __LINE__);
    
    /*if ((error = memMgrChunkMove(content + request->dataHandled, request->netData,
                    request->netDataSize)) == systemErrNone) {
            used = request->netDataSize;
    } else {
        debugOutFunctionFormattedV0(__FUNCTION__, "memory error", 0, __FILE__, __LINE__);
        return httpSetError(error, request);
    }*/

    error = httpMoveData(content, &used, request);
    if (error != systemErrNone) {
        return httpSetError(error, request);
    }

    debugOutFunctionFormattedV0(__FUNCTION__, "handled", 0, __FILE__, __LINE__);

httpParseData_cleanup:
    if (used) {
        httpShrinkSize(used, request);
        
        request->dataHandled += used;
        
        if ((request->dataHandled >= length) &&
            !webDataGetNoPredefLength(httpGetWebData(request))) { // If we are > then, uh oh!!
            request->state = HTTP_Done; // No more data to need! Done!!
            
            if (request->dataReceiveCallbackP) {
                error = httpCallDataReceiveCallback(HTTP_Data_Done,
                        &callData, request);
                        
                if (error != systemErrNone) {
                    return httpSetError(error, request);
                }
            }
        }
    }
    
    return systemErrNone;
}

char *httpGetResponseRedirectURL(HTTPRequestPtr request)
{
    return request->redirectURL;
}

void httpSetResponseRedirectURL(char *val, HTTPRequestPtr request)
{
    memMgrChunkFree(request->redirectURL);
    request->redirectURL = stringDup(val);
}

static boolean httpIsResponseStatusInRange(unsigned short range, HTTPRequestPtr request)
{
    short   leftOvers = request->serverStatus - range;
    
    return (leftOvers >= 0) && (leftOvers < 100); // between range and range+99
}

boolean httpIsResponseStatusOK(HTTPRequestPtr request)
{
    return httpIsResponseStatusInRange(200, request);
}

unsigned short httpGetResponseCode(HTTPRequestPtr request)
{
    return request->serverStatus;
}

void httpSetResponseCode(unsigned short response, HTTPRequestPtr request)
{
    request->serverStatus = response;
}

char *httpLookupGenericServerResp(HTTPRequestPtr request)
{
    short   i = 0;
    short   respCode    = httpGetResponseCode(request);
    
    while (code_list[i].error &&
           (code_list[i].code != respCode)) {
        i++;
    }
    
    return code_list[i].error;
}

int httpHandleResponse(HTTPRequestPtr request)
{
    int error = systemErrNone;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    // We some how came in with an error, not good. All HTTPParse* functions
    // will properly report errors and those will get passed back to the caller.
    if (request->state == HTTP_Error) {
        debugOutFunctionFormattedV0(__FUNCTION__, "state is error", request->error, __FILE__, __LINE__);
        return httpErrStateError;
    }
    
    if (!request->netDataSize) {
        debugOutFunctionFormattedV0(__FUNCTION__, "no net data size", 0, __FILE__, __LINE__);
        if (request->remoteClosed) {
            debugOutFunctionFormattedV0(__FUNCTION__, "remote closed", 0, __FILE__, __LINE__);
            if (request->state == HTTP_Parse_Data) {
                debugOutFunctionFormattedV0(__FUNCTION__, "done parsing data", 0, __FILE__, __LINE__);
                request->state = HTTP_Done;
                
                if (request->dataReceiveCallbackP) {
                    HTTPCallbackType    callData;
                    
                    error = httpCallDataReceiveCallback(HTTP_Data_Done,
                        &callData, request);
                        
                    if (error != systemErrNone) {
                        return httpSetError(error, request);
                    }
                }
                
                return systemErrNone;
            } else {
                debugOutFunctionFormattedV0(__FUNCTION__, "was not parsing data", 0, __FILE__, __LINE__);
                return httpSetError(httpErrRemoteClosed, request);
            }
        }
        
        return systemErrNone;
    }
    
    switch (request->state)
    {
        case HTTP_Parse_Status:
            error = httpParseStatus(request);
            break;
        case HTTP_Parse_Header:
            error = httpParseHeader(request);
            break;
        case HTTP_Parse_Data:
            error = httpParseData(request);
            break;
        case HTTP_Send_Request:
        case HTTP_Error:
        default:
            return httpErrInvalidState;
            break;
    }
    
    return error;
}

int httpCloseRequest(HTTPRequestPtr request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)request, __FILE__, __LINE__);
    
    if (!request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    netConnectionClose(loaderGetNetSession(request->loader));
    
    return systemErrNone;
}

int httpFinish(HTTPRequestPtr *request)
{
    debugOutFunctionFormattedV0(__FUNCTION__, "top", (long)*request, __FILE__, __LINE__);
    
    if (!*request) {
        debugOutFunctionFormattedV0(__FUNCTION__, "invlid request", 0, __FILE__, __LINE__);
        return httpErrInvalidRequest;
    }
    
    //debugOutFunctionFormattedV0(__FUNCTION__, "destroy URL", 0, __FILE__, __LINE__);
    // We do not touch webData
    //urlDestroyURL((*request)->url);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup auth data", 0, __FILE__, __LINE__);
    // With memMgrChunkFree we are allowed to skip checking for a valid ptr
    memMgrChunkFree((*request)->userName);
    memMgrChunkFree((*request)->password);
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup UA data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->userAgent);
    memMgrChunkFree((*request)->UAProf);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup redir data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->redirectURL);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup net data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->netData);
    //memMgrChunkFree((*request)->httpLine);
    memMgrChunkFree((*request)->requestHeader);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup post data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->post.dataTypeP);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup proxy data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->proxy.address);
    memMgrChunkFree((*request)->proxy.userName);
    memMgrChunkFree((*request)->proxy.password);
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup realm data", 0, __FILE__, __LINE__);
    memMgrChunkFree((*request)->authRequest.realm);
    
    /*debugOutFunctionFormattedV0(__FUNCTION__, "cleanup cookies", 0, __FILE__, __LINE__);
    if ((*request)->g && (*request)->cookies) {
        CookieCleanCookiePtr((*request)->cookies, (GlobalsType *)(*request)->g);
    }*/
    
    debugOutFunctionFormattedV0(__FUNCTION__, "cleanup headers", 0, __FILE__, __LINE__);

    httpSessionDeleteHeaders(&(*request)->headersRequest);
    httpSessionDeleteHeaders(&(*request)->headersResponse);

    debugOutFunctionFormattedV0(__FUNCTION__, "initialize to 0", 0, __FILE__, __LINE__);
    memMgrChunkSet(*request, 0, sizeof(HTTPRequestType));
    
    debugOutFunctionFormattedV0(__FUNCTION__, "free", 0, __FILE__, __LINE__);
    memMgrChunkFree(*request);
    *request = NULL;
    
    debugOutFunctionFormattedV0(__FUNCTION__, "bottom", 0, __FILE__, __LINE__);
    
    return systemErrNone;
}
