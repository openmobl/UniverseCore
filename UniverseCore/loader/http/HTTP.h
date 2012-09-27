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
 
#ifndef HTTP_h
#define HTTP_h
 
#include "config.h"

#include "Net.h"
#include "WebData.h"
#include "URL.h"

// TODO: Move these to a list...?
#define HTTP_POST_METH              "POST "
#define HTTP_GET_METH               "GET "
#define HTTP_HEAD_METH              "HEAD "
#define HTTP_PUT_METH               "PUT "
#define HTTP_TRACE_METH             "TRACE "
#define HTTP_OPTION_METH            "OPTION "

#define HTTP_PREFIX                 "HTTP/"
#define HTTP_VERSION_PREFIX         " HTTP/"
#ifdef USE_HTTP_1_1
#define HTTP_VERSION                " HTTP/1.1"
#else
#define HTTP_VERSION                " HTTP/1.0"
#endif

#define HTTP_HEADER_SEP             ':'
#define HTTP_HEADER_SEP_COM         ": "

#if 0

#define HTTP_HOST_HDR               "Host: "
#define HTTP_CONTENTLENGTH          "Content-Length"
#define HTTP_CONTENTLENGTH_HDR      HTTP_CONTENTLENGTH ": "
#define HTTP_LINE_ENDING            "\r\n"
#define HTTP_CONTENTTYPE            "Content-Type"
#define HTTP_CONTENTTYPE_HDR        HTTP_CONTENTTYPE ": "
#define HTTP_LOCATION               "Location"
#define HTTP_LOCATION_HDR           HTTP_LOCATION ": "
#define HTTP_CONTENT_LOCATION       "Content-Location"
#define HTTP_CONTENT_LOCATION_HDR    HTTP_CONTENT_LOCATION ": "
#define HTTP_CACHE_CONTROL_HDR      "Cache-Control: "
#define HTTP_USERAGENT_LINE         "User-Agent: "
#define HTTP_GEN_AGENT              "OpenMoblHTTP/1 (Universal; U)"
#define HTTP_CONNECTION             "Connection: close"
//#define HTTP_ACCEPT_ENC           "Accept-Encoding: "
#define HTTP_ACCEPT_LANG            "Accept-Language: en;g=0.8"
#define HTTP_ACCEPT_CHARSET         "Accept-charset: utf-8;q=1.0,us-ascii,iso-8859-1;g=0.5"
#define HTTP_X_WAP_PROFILE_START    "X-Wap-Profile: \""
#define HTTP_X_WAP_PROFILE_END      "\""
#define HTTP_ACCEPT                 "Accept: application/vnd.wap.wmlc, application/vnd.wap.wbxml, application/vnd.wap.wmlscriptc, " \
                                    "application/xhtml+xml, application/wml+xml, application/vnd.wap.xhtml+xml, application/rss+xml, text/html, text/x-html, " \
                                    "text/vnd.wap.wml, text/plain, text/xml, image/vnd.wap.wbmp, image/jpg, image/jpeg, image/bmp, " \
                                    "image/gif, image/pbmp, image/png, */*"
#define HTTP_PROXY_AUTH             "Proxy-Authorization: Basic "
#define HTTP_WEB_AUTH               "Authorization: Basic "
#define HTTP_REALM_HDR              "WWW-Authenticate: "
#define HTTP_COOKIE_HDR             "Cookie: "
#define HTTP_SET_COOKIE_HDR         "Set-Cookie: "
#define HTTP_COOKIE2_HDR            "Cookie2: "
#define HTTP_SET_COOKIE2_HDR        "Set-Cookie2: "

#endif

#define HTTP_HOST                   "Host"
#define HTTP_CONTENTLENGTH          "Content-Length"
#define HTTP_CONTENTLENGTH_HDR      HTTP_CONTENTLENGTH ": "
#define HTTP_LINE_ENDING            "\r\n"
#define HTTP_CONTENTTYPE            "Content-Type"
#define HTTP_CONTENTTYPE_HDR        HTTP_CONTENTTYPE ": "
#define HTTP_LOCATION               "Location"
#define HTTP_LOCATION_HDR           HTTP_LOCATION ": "
#define HTTP_CONTENT_LOCATION       "Content-Location"
#define HTTP_CONTENT_LOCATION_HDR    HTTP_CONTENT_LOCATION ": "
#define HTTP_CACHE_CONTROL          "Cache-Control"
#define HTTP_CACHE_CONTROL_HDR      "Cache-Control: "
#define HTTP_USERAGENT_LINE         "User-Agent: "
#define HTTP_USERAGENT              "User-Agent"
#define HTTP_GEN_AGENT              "OpenMoblHTTP/1 (Universal; U)"
#define HTTP_CONNECTION             "Connection"
#define HTTP_CONNECTION_CLOSE       "close"
//#define HTTP_ACCEPT_ENC           "Accept-Encoding: "
#define HTTP_ACCEPT_LANG            "Accept-Language"
#define HTTP_ACCEPT_LANG_VAL        "en;g=0.8"
#define HTTP_ACCEPT_CHARSET         "Accept-charset"
#define HTTP_ACCEPT_CHARSET_VAL     "utf-8;q=1.0,us-ascii,iso-8859-1;g=0.5"
#define HTTP_X_WAP_PROFILE_START    "X-Wap-Profile: \""
#define HTTP_X_WAP_PROFILE_END      "\""
#define HTTP_X_WAP_PROFILE          "X-Wap-Profile"
#define HTTP_ACCEPT                 "Accept"
#define HTTP_ACCEPT_VALUE           "application/vnd.wap.wmlc, application/vnd.wap.wbxml, application/vnd.wap.wmlscriptc, " \
                                    "application/xhtml+xml, application/wml+xml, application/vnd.wap.xhtml+xml, application/rss+xml, text/html, text/x-html, " \
                                    "text/vnd.wap.wml, text/plain, text/xml, image/vnd.wap.wbmp, image/jpg, image/jpeg, image/bmp, " \
                                    "image/gif, image/pbmp, image/png, */*"
#define HTTP_PROXY_AUTH             "Proxy-Authorization"
#define HTTP_WEB_AUTH               "Authorization"
#define HTTP_AUTH_BASIC             "Basic "
#define HTTP_REALM_HDR              "WWW-Authenticate"
#define HTTP_COOKIE_HDR             "Cookie"
#define HTTP_SET_COOKIE_HDR         "Set-Cookie"
#define HTTP_COOKIE2_HDR            "Cookie2"
#define HTTP_SET_COOKIE2_HDR        "Set-Cookie2"

#define HTTP_PORT                   80
#define HTTPS_PORT                  443

#define HTTP_CR                     '\r'
#define HTTP_LF                     '\n'
#define HTTP_CRLF                   "\r\n"

#define KILOBYTE        1024
#define HTTP_BUF_SIZE   (4 * KILOBYTE)

// Enable this to internally control the timeout
// Probably not a great thing, we should let the client do it themselves.
//#define INTERNAL_RECEIVE_LOOP


typedef enum HTTPRequestEnum {
    HTTP_NONE = 0,
    HTTP_GET,               // supported
    HTTP_POST,              // supported
    HTTP_HEAD,              // unsupported
    HTTP_PUT,               // unsupported
    HTTP_TRACE,             // unsupported
    HTTP_OPTION             // unsupported
} HTTPRequestEnum;

typedef enum HTTPCarrierEnum {
    HTTP_DIRECT,
    HTTP_PROXY
} HTTPCarrierEnum;

typedef enum HTTPStateEnum {
    HTTP_Send_Request,
    HTTP_Parse_Status,
    HTTP_Parse_Header,
    HTTP_Parse_Data,
    HTTP_Error,
    HTTP_Done
} HTTPStateEnum;

typedef struct HTTPProxyType {
    char            *address;
    char            *userName;
    char            *password;
    unsigned short  port;
} HTTPProxyType;

typedef struct HTTPAuthType {
    char            *realm;
} HTTPAuthType;

typedef struct HTTPPostRequest {
    unsigned char   *dataP;
    char            *dataTypeP;
    unsigned long   dataLength;
} HTTPPostRequest;

typedef enum HTTPCallbackStateEnum {
    HTTP_Receive_Header,
    HTTP_Header_Done,
    HTTP_Receive_Data,
    HTTP_Data_Done
} HTTPCallbackStateEnum;

typedef struct HTTPCallbackType {
    unsigned long           usedDataSize;
    void                    *request; // HTTPRequestType
    boolean                 handled;
    HTTPCallbackStateEnum   state;
} HTTPCallbackType;

typedef HTTPCallbackType *HTTPCallbackPtr;



typedef struct HTTPHeaderType {
    struct HTTPHeaderType   *next;
    
    char                    *header;
    char                    *value;
} HTTPHeaderType;

typedef HTTPHeaderType *HTTPHeaderPtr;

// Lets just give them everything...

// Notified when a header is received that the HTTP library does not handle
typedef int (*HTTPReceiveHeaderCallbackPtr)(char *header, char *value,
                                            WebDataPtr webData,
                                            HTTPCallbackPtr callDataP);
// Notified when data is received after the headers. This can allow
// for various download mechanisms implemented by the client.
typedef int (*HTTPReceiveDataCallbackPtr)(unsigned char *dataP, unsigned long dataLen,
                                            WebDataPtr webData,
                                            HTTPCallbackPtr callDataP);

typedef struct HTTPRequestType {
    void            *userContext;
#ifdef CONFIG_HTTP_OWN_URL
    URLPtr          url;
#endif
    
    //CookiePtr    cookies;
    
    void            *sslContext;
    
    char            *userName;
    char            *password;
    char            *userAgent;
    char            *UAProf;
    
    char            *redirectURL;
    
    unsigned char   *netData;
    unsigned long   netDataSize;
    unsigned char   *requestHeader;
    unsigned long   requestHeaderLength;
    
    unsigned long   dataHandled;
    
    unsigned long   stopTicks;
    
    HTTPHeaderPtr   headersRequest;
    HTTPHeaderPtr   headersResponse;
    
    HTTPReceiveHeaderCallbackPtr    headerReceiveCallbackP; // When we have the header, call this
    HTTPReceiveDataCallbackPtr      dataReceiveCallbackP;   // when parsing body, pass data to this
    
    HTTPPostRequest post;
    HTTPProxyType   proxy;
    HTTPRequestEnum requestType;
    HTTPCarrierEnum carrier;
    HTTPAuthType    authRequest;
    
    HTTPStateEnum   state;
    int             error;
    char            errorDetails[256];
    
    unsigned short  httpVersionMajor;
    unsigned short  httpVersionMinor;
    
    unsigned short  serverHttpVerMajor;
    unsigned short  serverHttpVerMinor;
    unsigned short  serverStatus;
    char            serverStatusVal[256];
    
    void            *loader;
    boolean         remoteClosed;
} HTTPRequestType;

typedef HTTPRequestType *HTTPRequestPtr;


#ifdef __cplusplus
extern "C" {
#endif

extern boolean httpHasError(HTTPRequestPtr request);
extern int httpGetError(HTTPRequestPtr request);
extern boolean httpIsDone(HTTPRequestPtr request);
extern void httpSetDone(HTTPRequestPtr request);
extern int httpSetErrorExt(int error, char *file, unsigned short line,
                    const char *function, HTTPRequestPtr request);
#define httpSetError(a,b) \
        httpSetErrorExt(a, __FILE__, __LINE__, __FUNCTION__, b)
extern void httpDisplayError(char *msg, HTTPRequestPtr request);



extern char *httpGetFullURL(HTTPRequestPtr request);

extern void httpSetLoader(void *loader, HTTPRequestPtr request);
extern void *httpGetLoader(HTTPRequestPtr request);
extern void httpSetContentType(char *mime, HTTPRequestPtr request);
extern char *httpGetContentType(HTTPRequestPtr request);
extern void httpSetContentLength(unsigned long length, HTTPRequestPtr request);
extern unsigned long httpGetContentLength(HTTPRequestPtr request);

extern unsigned short httpGetResponseCode(HTTPRequestPtr request);
extern void httpSetResponseCode(unsigned short response, HTTPRequestPtr request);
extern char *httpLookupGenericServerResp(HTTPRequestPtr request);

extern HTTPRequestEnum httpGetRequestType(HTTPRequestPtr request);

extern WebDataPtr httpGetWebData(HTTPRequestPtr request);

#ifdef CONFIG_HTTP_OWN_URL
extern HTTPRequestPtr httpCreateRequest(char *urlP, HTTPRequestEnum requestType, int *err);
#else
extern HTTPRequestPtr httpCreateRequest(HTTPRequestEnum requestType, int *err);
#endif

extern int httpSetUserContext(void *context, HTTPRequestPtr request);
extern void *httpGetUserContext(HTTPRequestPtr request);

extern int httpSetRequestProxy(char *proxy, unsigned short port, char *userName,
                        char *password, HTTPRequestPtr request);
extern int httpSetRequestAuthentication(char *userName, char *password,
                                 HTTPRequestPtr request);
extern int httpSetRequestUserAgent(char *userAgent, HTTPRequestPtr request);
extern int httpSetRequestUAProf(char *profile, HTTPRequestPtr request);
extern int httpSetRequestHTTPVersion(unsigned short major, unsigned short minor, HTTPRequestPtr request);
extern int httpSetReceiveHeaderNotify(HTTPReceiveHeaderCallbackPtr callbackP,
                               HTTPRequestPtr request);
extern int httpSetReceiveDataNotify(HTTPReceiveDataCallbackPtr callbackP,
                             HTTPRequestPtr request);
//extern int httpSetRequestCookies(CookiePtr cookieJar, boolean private, HTTPRequestPtr request);
extern int httpRequestAddHeader(char *header, char *value, HTTPRequestPtr request);
extern int httpRequestPostAddContent(unsigned char *dataP, unsigned long dataLen, char *dataType,
                               HTTPRequestPtr request);

extern char *httpFindHeader(char *header, HTTPHeaderPtr headersP);
extern HTTPHeaderPtr httpGetResponseHeaders(HTTPRequestPtr request);
extern HTTPHeaderPtr httpGetRequestHeaders(HTTPRequestPtr request);

extern int httpCreateRequestHeader(HTTPRequestPtr request);
extern int httpRequestCompleteHeader(HTTPRequestPtr request);
extern int httpOpenRequest(HTTPRequestPtr request);
extern int httpSendRequest(HTTPRequestPtr request);
extern int httpSetResponseTimeout(HTTPRequestPtr request);
extern int httpReceiveResponse(HTTPRequestPtr request);
extern int httpHandleResponse(HTTPRequestPtr request);
extern int httpCloseRequest(HTTPRequestPtr request);
extern int httpFinish(HTTPRequestPtr *request);

extern boolean httpIsResponseStatusOK(HTTPRequestPtr request);
extern char *httpGetResponseRedirectURL(HTTPRequestPtr request);
extern void httpSetResponseRedirectURL(char *val, HTTPRequestPtr request);

extern unsigned long httpGetTimeoutTime(HTTPRequestPtr request);

#ifdef __cplusplus
}
#endif

#endif // HTTP_h
