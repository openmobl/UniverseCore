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

#include <HTTP.h>
#include <Loader.h>
#include <WebData.h>
#include <URL.h>

#define HTTP_TESTER_VER 1.0

#define errAssert(x,y,z)    if (x) {                \
                                fprintf(stderr, y); \
                                goto z;             \
                            }

int testerBuildHTTPHeader(char *url, HTTPRequestPtr *request, unsigned char *post,
                           unsigned long postLen, char *postMIME, 
                           char *user, char *pass)
{
    int           error   = 0;
    HTTPRequestPtr  new     = NULL;
    
    if (!url || !request) { // important parameters
        return httpErrParameterError;
    }
    
    new = httpCreateRequest(url, HTTP_GET, &error);
    if (!new) {
        *request = NULL;
        return error;
    }
    
    *request = new;
    
    error = httpSetRequestHTTPVersion(1, 0, new);
    errAssert(error != errNone, "HTTPSetRequestHTTPVersion failed",
              browserBuildHTTPHeader_cleanup);
    
#if 0
        error = httpSetRequestProxy(ipaddress, atoi(port), proxyuser, proxypass, new);
        errAssert(error != errNone, "HTTPSetRequestProxy failed",
                  browserBuildHTTPHeader_cleanup);
#endif
#if 0
    error = httpSetRequestAuthentication(user, pass, new);
    errAssert(error != errNone, "HTTPSetRequestAuthentication failed",
              browserBuildHTTPHeader_cleanup);
#endif

    error = httpSetRequestUserAgent("HTTPTester/1.0 UniverseCore/0.2", new);
    errAssert(error != errNone, "HTTPSetRequestUserAgent failed",
              browserBuildHTTPHeader_cleanup);
    
#if 0
        httpSetRequestUAProf(UNIVERSE_UA_PROF, new);
        errAssert(error != errNone, "HTTPSetRequestUAProf failed",
                  browserBuildHTTPHeader_cleanup);
#endif

#if 0
    error = httpSetRequestCookies(g->CookieJar, g->privateBrowsing, new);
    errAssert(error != errNone, "HTTPSetRequestCookies failed",
              browserBuildHTTPHeader_cleanup);
#endif
    
    if (post) {
        error = httpRequestPostAddContent(post, postLen, postMIME, new);
        errAssert(error != errNone, "HTTPRequestPostAddContent failed",
                  browserBuildHTTPHeader_cleanup);
    }

#if 0
    error = httpSetNetworkLibRefnum(AppNetRefnum, new);
    errAssert(error != errNone, "HTTPSetNetworkLibRefnum failed",
              browserBuildHTTPHeader_cleanup);
#endif

    error = httpCreateRequestHeader(new);
    error = httpRequestCompleteHeader(new);

browserBuildHTTPHeader_cleanup:
    return error;
}

int testerSendHTTPRequest(HTTPRequestPtr request)
{
    int     error = systemErrNone;
    
    error = httpOpenRequest(request); // connect
    if (error) {
        goto testerSendHTTPRequest_cleanup;
    }
    
    error = httpSendRequest(request);
    
testerSendHTTPRequest_cleanup:
    return error;
}

void testerSetupReceiveHTTPResponse(HTTPRequestPtr request)
{
    httpSetResponseTimeout(timeGetCurrentTick() +
                    (60 * timeGetTicksPerSecond()), request);
}

typedef struct testerHTTPContext {
    char    name[1024];
    FILE    *file;
} testerHTTPContext;

int testerHTTPReceiveHeaderCallback(char *header, char *value,
                                            WebDataPtr webData,
                                            HTTPCallbackPtr callDataP)
{
    //WebDataPtr      httpWebData = httpGetWebData(callDataP->request);
    //int             error = errNone;
    
    if ((callDataP->state == HTTP_Header_Done) &&
        httpIsResponseStatusOK(callDataP->request)) {
        //if (!webDataIsPageMIME(httpWebData) &&
        //    !webDataIsEmbeded(httpWebData)) {
            
            // For now, download to file stream
            
            testerHTTPContext   *context = NULL;
            
            context = malloc(sizeof(testerHTTPContext));
            if (context) {
                httpSetUserContext(context, callDataP->request);
                
                printf("Please enter a name to save the file as: ");
                fscanf(stdin, "%s", context->name);
                
                context->file = fopen(context->name, "w");
            }
        //}
    } else if (callDataP->state == HTTP_Header_Done) {
        // We have an error
    } else {
    
    }  

    return errNone;
}

#define MAX_REDIRECT_COUNT  8

static unsigned long totLen = 0;
static int redirects = 0;


int testerHTTPReceiveDataCallback(unsigned char *dataP, unsigned long dataLen,
                                            WebDataPtr webData,
                                            HTTPCallbackPtr callDataP)
{
    //WebDataPtr      httpWebData = httpGetWebData(callDataP->request);
    //int             error       = 0;
    
    if (callDataP->state == HTTP_Data_Done) {
        
        // this is semi-un-neccessary since the above will determine if we have a
        // download or not, since we just let the HTTP code handle out download
        if (!httpIsResponseStatusOK(callDataP->request)) {
            switch (httpGetResponseCode(callDataP->request)) {
                case 300 ... 399: // Handle redirection requests
                    if (redirects >= MAX_REDIRECT_COUNT) {
                        fprintf(stderr, "The browser has been redirected more than the maximum alotted redirect limit.\n");
                        redirects = 0;
                        return httpErrMaxRedirectReached;
                    }

                    if (!httpGetResponseRedirectURL(callDataP->request)) {
                        // we assume that this is the result of a malformed header...
                        return httpErrMalformedHeader;
                    }
                    
                    redirects++;

                    printf("Redirect URL: %s\n", httpGetResponseRedirectURL(callDataP->request));
                    
                    return httpErrRedirectRequested;
                    break;
                default:
                    break;
            }
            
            redirects = 0;
            
            if (!httpGetContentLength(callDataP->request)) {
                char    message[512];
                
                sprintf(message, "Server Response: %s\n",
                    httpLookupGenericServerResp(callDataP->request));
                
                fprintf(stderr, message);
                
                return httpErrNotOKStatusHandled;
            }
        }
    } else {
        testerHTTPContext   *context = NULL;
        char                temp[100];
        
        totLen += dataLen;
    
        sprintf(temp, "downloaded: %ld", totLen);
        printf(temp);
        printf("\n");
        
        context = httpGetUserContext(callDataP->request);
        if (!context)
            return 0;
        
        fwrite(dataP, dataLen, 1, context->file);
#if 0
        if (DownloaderGetDest(this) == Dest_VFS) {
            callDataP->handled = true;
            
            callDataP->usedDataSize = dataLen;
            
            error = DownloaderSaveToVFS(dataP, dataLen, this);
            if (error) {
                return error;
            }
        } else if (DownloaderGetDest(this) == Dest_Internal) {
            callDataP->handled = true;
            
            callDataP->usedDataSize = dataLen;
            
            error = DownloaderSaveToInternal(dataP, dataLen, this);
            if (error) {
                return error;
            }
        } else if (DownloaderGetDest(this) == Dest_Exchange) {
        
        }
#endif
    }

    return errNone;
}

int testerSetupHTTPCallbacks(HTTPRequestPtr request)
{
    httpSetReceiveHeaderNotify(testerHTTPReceiveHeaderCallback,
                               request);
    httpSetReceiveDataNotify(testerHTTPReceiveDataCallback,
                               request);
    
    return errNone;
}

int testerReceiveHTTPRequest(HTTPRequestPtr request)
{
    return httpReceiveResponse(request);
}

int testerHandleHTTPRequest(HTTPRequestPtr request)
{
    return httpHandleResponse(request);
}

int testerHTTPIsDone(HTTPRequestPtr request)
{
    return httpIsDone(request);
}

int testerHTTPHasError(HTTPRequestPtr request)
{
    return httpHasError(request);
}

void testerDisplayHTTPErr(char *msg, HTTPRequestPtr request)
{
    httpDisplayError(msg, request);
}

void testerCleanupHTTPRequest(HTTPRequestPtr *request)
{
    if (!request || !*request) {
        return;
    }
    
    httpCloseRequest(*request);
    httpFinish(request);
}


int main(int argc, char argv[])
{
    int             error       = 0;
    char            *url        = NULL;
    WebDataType     webData;
    HTTPRequestPtr  httpRequest = NULL;
    unsigned char   *post       = NULL;
    unsigned long   postLen     = 0;
    char            *postMIME   = NULL;
    char            *user       = NULL;
    char            *pass       = NULL;
    LoaderPtr       loader      = NULL;
    
    webDataInit(&webData);
    
    printf("UniverseCore HTTP Tester v%.3f\n", HTTP_TESTER_VER);
    
    url = malloc(1024);
    if (!url)
        goto cleanup;
    user = malloc(512);
    if (!user)
        goto cleanup;
    pass = malloc(512);
    if (!pass)
        goto cleanup;
    
    printf("Please enter a URL to test: ");
    fscanf(stdin, "%s", url);
    
    printf("Please enter a user name (if required, or press ctrl+d): ");
    fscanf(stdin, "%s", user);

    printf("Please enter a password (if required, or press ctrl+d): ");
    fscanf(stdin, "%s", pass);

http_load:
    error = testerBuildHTTPHeader(url, &httpRequest, post, postLen,
                                    postMIME, (strlen(user))?user:NULL, (strlen(pass))?pass:NULL);
    if (error) {
        fprintf(stderr, "Error: %d", error);
        return 0;
    }
    
    loader = malloc(sizeof(LoaderType));
    httpSetLoader(loader, httpRequest);
    loaderSetWebData(&webData, loader);
    
    testerSetupReceiveHTTPResponse(httpRequest);
    testerSetupHTTPCallbacks(httpRequest);

    testerSendHTTPRequest(httpRequest);
    
    while (!testerHTTPIsDone(httpRequest) && !testerHTTPHasError(httpRequest)) {
        testerReceiveHTTPRequest(httpRequest);
        
        testerHandleHTTPRequest(httpRequest);
    }
    
    if (httpGetResponseRedirectURL(httpRequest)) {
        char    *tempUrl = malloc(strlen(httpGetResponseRedirectURL(httpRequest)) + strlen(url) + 1); //strdup(httpGetResponseRedirectURL(httpRequest));
        
        strcpy(tempUrl, url);
        strcat(tempUrl, httpGetResponseRedirectURL(httpRequest));
        
        free(url);
        
        url = tempUrl;
        
        testerCleanupHTTPRequest(&httpRequest);
        
        goto http_load;
    }

    testerCleanupHTTPRequest(&httpRequest);
    
cleanup:
    if (loader)
        free(loader);

    if (url)
        free(url);
    if (user)
        free(user);
    if (pass)
        free(pass);

    return 0;
}
