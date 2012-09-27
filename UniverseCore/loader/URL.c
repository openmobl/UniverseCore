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
#include "URL.h"

#include "HTTP.h"
#include "MemMgr.h"

void urlAddToBuf(char **buf, unsigned long *bufSize, char *str, unsigned long len);
boolean urlIsMustEscapeChar(int chr);
boolean isHex(int chr);
void urlAppendEncoded(char **buf, unsigned long *bufSize, char *str);


URLPtr urlCreateURL(URLTransport transport, char *userName, char *password,
                     char *host, unsigned short port, char *path, char *query, char *fragment,
                     char *full)
{
    URLPtr  url;

    url = (URLType *)memMgrChunkNew(sizeof(URLType));
    if (url == NULL) {
        return NULL;
    }
    memMgrChunkSet(url, 0, sizeof(URLType));
    
    url->userName = stringDup(userName);
    url->password = stringDup(password);

    url->host = stringDup(host);
    /*if (!url->host) {
        urlDestroyURL(url);
        return NULL;
    }*/

    url->path = stringDup(path);
    /*if (!url->path) {
        urlDestroyURL(url);
        return NULL;
    }*/

    url->transport  = transport;
    url->port       = port;
    
    url->query      = stringDup(query);
    url->fragment   = stringDup(fragment);
    
    url->full       = stringDup(full);

    return url;
}

URLPtr urlParseURL(char *urlP, int *error)
{
    URLTransport    transport   = URL_none;
    char            *url        = NULL;
    char            *user       = NULL;
    char            *pass       = NULL;
    char            *host       = NULL;
    char            *path       = NULL;
    char            *query      = NULL;
    char            *frag       = NULL;
    unsigned short  port        = 0;
    URLPtr          urlStruct   = NULL;
    //char            *temp;
    
    if (!urlP) {
        *error = urlErrNullURL;
        return NULL;
    }

    if (!stringNCompare(urlP, "http://", 7)) {
        transport = URL_http;
        url = urlP + 7;
        
        if (urlParseHTTPURL(url, &user, &pass, &host, &port, &path, &query, &frag) == -1) {
            *error = urlErrMalformedURL;
            return NULL;
        }
            
        if (!port) {
            port = HTTP_PORT;
        }
    } else if (!stringNCompare(urlP, "https://", 8)) {
        transport = URL_https;
        url = urlP + 8;
        
        if (urlParseHTTPURL(url, &user, &pass, &host, &port, &path, &query, &frag) == -1) {
            *error = urlErrMalformedURL;
            return NULL;
        }
            
        if (!port) {
            port = HTTPS_PORT;
        }
    } else if (!stringNCompare(urlP, "file:", 5)) { // host is card name, path is path on card
        transport   = URL_file;
        *error      = urlErrUnsupportedURI;
        return NULL; // for now
    } else if (!stringNCompare(urlP, "data:", 5)) { // query will contain the data past "data:"
        transport   = URL_data;
        *error      = urlErrUnsupportedURI;
        return NULL; // for now
    } else {
        // Try to figure out URL type
        if (urlP[0] == '#') { // A special case, we need to pass back up to the Document handler
            *error = urlErrIsDocumentRelative;
            
            transport   = URL_incomplete;
            frag        = urlP + 1;
        } else if (!stringChr(urlP, ':')) {
            /*if (!stringNCompare(urlP, "./", 2)  || // stay in the same directory; combine with path, replace query and frag
                !stringNCompare(urlP, "../", 3) || // up one directory; combine with path, replace query and frag
                (urlP[0] == '/')                || // Root directory; replace path, query and frag
                
            } else { // stay in the same directory; combine with path, replace query and frag
                
            }*/
            transport   = URL_incomplete;
            *error      = urlErrIncompleteURL;
            
            if (urlParseIncompleteURL(urlP, &path, &query, &frag) == -1) {
                *error = urlErrMalformedURL;
                return NULL;
            }
                
            if (!port) {
                port = 0;
            }
        } else { // Unsupported URI type, pass back to the system somehow!!
            *error = urlErrUnsupportedURI;
            return NULL;
        }
    }

    urlStruct = urlCreateURL(transport, user, pass, host, port, path,
                             query, frag, urlP);
                             
    if (transport != URL_incomplete) { // Incomplete URLs will get handled later
        urlStripSingleDots(urlGetPath(urlStruct));
        urlStripDoubleDots(urlGetPath(urlStruct) + 1); // TODO: +1 gets us past the '/', ignore it in the stripper?
    }

    //if (*error != urlErrIsDocumentRelative)
    //    *error = systemErrNone;

    return urlStruct;
}

int urlParseIncompleteURL(char *urlP, char **path, char **query, char **frag)
{
    char    *pathStart  = NULL;
    char    *queryStart = NULL;
    char    *fragStart  = NULL;

    *path       = NULL;
    *query      = NULL;
    *frag       = NULL;

    if (!urlP)
        return -1;

    // TODO: Reserved word check!! i.e. http://user/name:password@host/ is invalid, but we parse it anyway!!

    pathStart   = stringChr(urlP, URL_PATH_SEP);
    queryStart  = stringChr(urlP, URL_QUERY_SEP);
    fragStart   = stringChr(urlP, URL_FRAG_SEP);
    
    if (pathStart || ((unsigned long)queryStart > (unsigned long)pathStart)) {
        if ((unsigned long)urlP < (unsigned long)pathStart) { // .../...
            pathStart = urlP;
        }

        if (!fragStart && !queryStart) { // /
            *path   = stringDup(pathStart);
            *query  = NULL;
            *frag   = NULL;
        } else if (!fragStart) { // /...?...
            *path   = stringNDup(pathStart, (unsigned long)queryStart - (unsigned long)pathStart);
            *query  = stringDup(queryStart + 1);
            *frag   = NULL;
        } else if (!queryStart) { // /...#...
            *path   = stringNDup(pathStart, (unsigned long)fragStart - (unsigned long)pathStart);
            *query  = NULL;
            *frag   = stringDup(fragStart + 1);
        } else if ((unsigned long)queryStart < (unsigned long)fragStart) { // /...?...#...
            *path   = stringNDup(pathStart, (unsigned long)queryStart - (unsigned long)pathStart);
            *query  = stringNDup(queryStart + 1, ((unsigned long)fragStart - 1) - (unsigned long)queryStart);
            *frag   = stringDup(fragStart + 1);
        } else {  // /...#...?...   INVALID!!
            goto urlParseIncompleteURL_error;
        }
    } else {
        /*if (queryStart && !fragStart) { //
            *path   = NULL;
            *query  = stringDup(queryStart);
            *frag   = NULL;
        } else {
            goto urlParseIncompleteURL_error;
        }*/
        
        goto urlParseIncompleteURL_error;
    }

    return 0;

urlParseIncompleteURL_error:
    memMgrChunkFree(*path);
    memMgrChunkFree(*query);
    memMgrChunkFree(*frag);
    
    *path       = NULL;
    *query      = NULL;
    *frag       = NULL;

    return -1;
}

int urlParseHTTPURL(char *urlP, char **userName, char **password, char **host,
                    unsigned short *port, char **path, char **query, char **frag)
{
    char    *authEnd    = NULL;
    char    *hostStart  = NULL;
    char    *pathStart  = NULL;
    char    *queryStart = NULL;
    char    *fragStart  = NULL;
    char    *semiColon  = NULL;
    char    *slash      = NULL;

    *userName   = NULL;
    *password   = NULL;
    *host       = NULL;
    *port       = 0;
    *path       = NULL;
    *query      = NULL;
    *frag       = NULL;

    // TODO: Reserved word check!! i.e. http://user/name:password@host/ is invalid, but we parse it anyway!!

    // -- Parse Authentication
    if ((authEnd = stringChr(urlP, URL_AUTH_SEP))) {
        authEnd--;
        
        if ((authEnd - urlP) < 1) { // @host
            *userName = NULL;
            *password = NULL;
        } else {
            char    *split = stringChr(urlP, URL_USER_PASS_SEP);
            
            if (split && !(split > authEnd)) { // user:pass@
                *userName = stringNDup(urlP, ((unsigned long)split - (unsigned long)urlP));
                *password = stringNDup(split + 1, ((unsigned long)authEnd - (unsigned long)split));
            } else { // user@
                *userName = stringNDup(urlP, ((unsigned long)authEnd - (unsigned long)urlP) + 1); 
            }
        }
        
        authEnd++;
    }
    
    //printf("user: %s  pass: %s\n", (*userName)?*userName:"", (*password)?*password:"");

    // -- Parse Host
    hostStart   = (authEnd)?(authEnd + 1):urlP;
    
    semiColon   = stringChr(hostStart, URL_PORT_SEP);
    slash       = stringChr(hostStart, URL_PATH_SEP);
    
    if (!slash && !semiColon) { //host
        *host       = stringDup(hostStart);
        pathStart   = NULL;
    } else if (!slash) { // host:port
        *host       = stringNDup(hostStart, ((unsigned long)semiColon) - (unsigned long)hostStart);
        *port       = stringAToI(semiColon + 1);
        pathStart   = NULL;
    } else if (!semiColon) { // host/
        *host       = stringNDup(hostStart, (((unsigned long)slash) - (unsigned long)hostStart));
        pathStart   = slash;
    } else if ((unsigned long)semiColon < (unsigned long)slash) { // host:port/
        char    *portTemp;
        
        *host       = stringNDup(hostStart, ((unsigned long)semiColon) - (unsigned long)hostStart);
        portTemp    = memMgrChunkNew((unsigned long)semiColon - (unsigned long)hostStart + 1);
        if (!portTemp)
            goto urlParseHTTPURL_error;
        stringNCopy(portTemp, semiColon + 1, ((unsigned long)semiColon) - (unsigned long)hostStart);
        *port       = stringAToI(portTemp);
        pathStart   = slash;
        
        memMgrChunkFree(portTemp);
    } else { //host/...:...   INVALID
            goto urlParseHTTPURL_error;
    }
    
    //printf("host: %s  port: %d\n", (*host)?*host:"", *port);
    
    
    // -- Parse Path, Query and Fragment
    if (pathStart) {
        queryStart = stringChr(pathStart, URL_QUERY_SEP);
        fragStart  = stringChr(pathStart, URL_FRAG_SEP);
        
        if (!fragStart && !queryStart) { // /
            *path   = stringDup(pathStart);
            *query  = NULL;
            *frag   = NULL;
        } else if (!fragStart) { // /...?...
            *path   = stringNDup(pathStart, (unsigned long)queryStart - (unsigned long)pathStart);
            *query  = stringDup(queryStart + 1);
            *frag   = NULL;
        } else if (!queryStart) { // /...#...
            *path   = stringNDup(pathStart, (unsigned long)fragStart - (unsigned long)pathStart);
            *query  = NULL;
            *frag   = stringDup(fragStart + 1);
        } else if ((unsigned long)queryStart < (unsigned long)fragStart) { // /...?...#...
            *path   = stringNDup(pathStart, (unsigned long)queryStart - (unsigned long)pathStart);
            *query  = stringNDup(queryStart + 1, ((unsigned long)fragStart - 1) - (unsigned long)queryStart);
            *frag   = stringDup(fragStart + 1);
        } else {  // /...#...?...   INVALID!!
            goto urlParseHTTPURL_error;
        }
    } else {
        *path   = stringDup(URL_PATH_EMPTY);
        *query  = NULL;
        *frag   = NULL;
    }

    //printf("path: %s  query: %s  frag: %s\n", (*path)?*path:"", (*query)?*query:"", (*frag)?*frag:"");

    return 0;

urlParseHTTPURL_error:
    memMgrChunkFree(*userName);
    memMgrChunkFree(*password);
    memMgrChunkFree(*host);
    memMgrChunkFree(*path);
    memMgrChunkFree(*query);
    memMgrChunkFree(*frag);
    
    *userName   = NULL;
    *password   = NULL;
    *host       = NULL;
    *path       = NULL;
    *query      = NULL;
    *frag       = NULL;

    return -1;
}

URLTransport urlGetTransport(URLPtr url)
{
    return (url)?url->transport:URL_none;
}

char *urlGetUserName(URLPtr url)
{
    return (url)?url->userName:NULL;
}

char *urlGetPassword(URLPtr url)
{
    return (url)?url->password:NULL;
}

char *urlGetHost(URLPtr url)
{
    return (url)?url->host:NULL;
}

unsigned short urlGetPort(URLPtr url)
{
    return (url)?url->port:0;
}

char *urlGetPath(URLPtr url)
{
    return (url)?url->path:NULL;
}

char *urlGetQuery(URLPtr url)
{
    return (url)?url->query:NULL;
}

char *urlGetFragment(URLPtr url)
{
    return (url)?url->fragment:NULL;
}

char *urlGetFull(URLPtr url)
{
    // TODO: Maybe create URL dynamically to save memory from storing it?
    return (url)?url->full:NULL;
}

// TODO: Provide a reference to source project for this function
/**
 * Collapses .. references in a path. That is, when invoked with
 * the string b/c/../d, on exit the string will be b/d.
 * @param s a path without leading slash to collapse.
 */
void urlStripDoubleDots(char *s)
{
    unsigned long   len = stringLen(s);
    unsigned short  i;
    unsigned short  j = 0;
    unsigned short  mark = 0, parent_mark = 0;
    unsigned short  dots = 0, seg_len = 0;
    boolean         nothing_collapsed = true;

    char *s2 = (char *)memMgrChunkNew(len + 1);

    if (s2) {
        for (i = 0; /*s[i]*/i < len; i++) {
            s2[j++] = s[i];
           
            if (nothing_collapsed) {
                /* nothing collapsed yet. */
                if (s[i] == '/' && i > 0) {
                    if(dots == 2 && seg_len == 3) {
                        /* found two dots -> collapse to parent. */
                        j = parent_mark;
                        nothing_collapsed = false;
                    } else {
                        parent_mark = mark;
                        mark = j;
                    }
                    
                    dots = 0;
                    seg_len = 0;
                }

                seg_len++;
        
                if(s[i] == '.')
                    dots++;
                else
                    dots = 0;
            }
        }
        
        if(i > 0 && nothing_collapsed && dots == 2 && seg_len == 3) {
            /* found two dots at end of string -> collapse to parent. */
            j = parent_mark;
            nothing_collapsed = false;
        }

        s2[j] = '\0';

        /* try to collapse again. */
        if(nothing_collapsed == false)
            urlStripDoubleDots(s2);

        stringCopy(s, s2);

        memMgrChunkFree(s2);
    }
}


// TODO: Provide a reference to the source project for this function
/**
 * Removes . references from a path.
 * @param s path to remove . references from.
 */
void urlStripSingleDots(char *s)
{
    unsigned long   len = stringLen(s);
    char            *s2 = (char *)memMgrChunkNew(len + 1);

    if (s2) {
        unsigned short i, j = 0;
        unsigned short looking_for_dot = 0;

        for (i = 0; /*s[i]*/i < len; i++) {
            s2[j++] = s[i];
      
            if (s[i] == '/') {
                if (looking_for_dot && (i > 1) && (s[i - 1] == '.') && (s[i - 2] == '/'))
                    j -= 2;

                looking_for_dot++;
            }
        }

        s2[j] = '\0';
        stringCopy(s, s2);
        memMgrChunkFree(s2);
    }
}

static char *urlTransportToString(URLTransport transport)
{
    switch (transport) {
        case URL_http:
            return URL_HTTP_COMP;
            break;
        case URL_https:
            return URL_HTTPS_COMP;
            break;
        case URL_file:
            return URL_FILE_COMP;
            break;
        case URL_data:
            return URL_DATA_COMP;
            break;
        default:
            return "";
            break;
    }
    
    return "";
}

URLPtr urlCombineURLs(URLPtr mainURL, URLPtr incomplete, int *error)
{
    URLPtr          url     = NULL;
    char            *full   = NULL;
    char            *temp   = urlGetPath(incomplete);
    char            *path   = stringDup(urlGetPath(mainURL)); // For safe measures
    unsigned long   len     = stringLenProtected(urlGetUserName(mainURL)) +
                              stringLenProtected(urlGetPassword(mainURL)) +
                              stringLenProtected(urlGetHost(mainURL)) +
                              stringLenProtected(urlGetPath(incomplete)) + stringLenProtected(urlGetPath(mainURL)) + // Combine paths
                              stringLenProtected(urlGetQuery(incomplete)) +
                              stringLenProtected(urlGetFragment(incomplete)) + 10;
    
    full = (char *)memMgrChunkNew(len + 1);
    if (!full) {
        *error = memoryErrNoFreeMemory;
        return NULL;
    }
    memMgrChunkSet(full, 0, len + 1);
    
    if (temp) {
        if (temp[0] == '/') {
            memMgrChunkFree(path);
            path = stringDup(urlGetPath(incomplete));
        } else { // if (!stringNCompare(temp, "../", 3) || !stringNCompare(temp, "./", 2)) { // ../ && ./ && ASCII
            int     i           = 0;
            
            memMgrChunkFree(path);
            
            path = (char *)memMgrChunkNew(stringLenProtected(urlGetPath(incomplete)) + stringLenProtected(urlGetPath(mainURL)));
            if (!path) {
                *error = memoryErrNoFreeMemory;
                return NULL;
            }
            
            stringCopy(path, urlGetPath(mainURL));
            for (i = stringLen(urlGetPath(mainURL)); (i != '/') && (i > 0); i--); // i[0] should be '/' anyway
            stringCopy(path + i + 1, urlGetPath(incomplete));
        }

        urlStripSingleDots(path);
        urlStripDoubleDots(path);
    }
    
    stringCopy(full, urlTransportToString(urlGetTransport(mainURL)));
    if (urlGetUserName(mainURL)) {
        stringCopy(full + stringLen(full), urlGetUserName(mainURL));
        if (urlGetPassword(mainURL)) {
            stringCopy(full + stringLen(full), URL_USER_PASS_SEP_STR);
            stringCopy(full + stringLen(full), urlGetPassword(mainURL));
        }
        
        stringCopy(full + stringLen(full), URL_AUTH_SEP_STR);
    }
    stringCopy(full + stringLen(full), urlGetHost(mainURL));
    if (((urlGetTransport(mainURL) == URL_http) && (urlGetPort(mainURL) != HTTP_PORT)) ||
        ((urlGetTransport(mainURL) == URL_http) && (urlGetPort(mainURL) != HTTPS_PORT))) {
            char    temp[50];
            
            stringPrintF(temp, "%d", urlGetPort(mainURL));
            
            stringCopy(full + stringLen(full), URL_PORT_SEP_STR);
            stringCopy(full + stringLen(full), temp);
    }
    stringCopy(full + stringLen(full), path);
    if (urlGetQuery(url)) {
        stringCopy(full + stringLen(full), URL_QUERY_SEP_STR);
        stringCopy(full + stringLen(full), urlGetQuery(incomplete));
    }
    if (urlGetFragment(url)) {
        stringCopy(full + stringLen(full), URL_FRAG_SEP_STR);
        stringCopy(full + stringLen(full), urlGetFragment(incomplete));
    }
    
    url = urlCreateURL(urlGetTransport(mainURL), urlGetUserName(mainURL), urlGetPassword(mainURL),
                       urlGetHost(mainURL), urlGetPort(mainURL),
                       path, urlGetQuery(incomplete), urlGetFragment(incomplete),
                       full);
    
    memMgrChunkFree(path);
    memMgrChunkFree(full);
    *error = systemErrNone;
    
    return url;
}

void urlDestroyURL(URLPtr url)
{
    if (!url)
        return;

    if (url->userName) {
        memMgrChunkFree(url->userName);
        url->userName = NULL;
    }
    
    if (url->password) {
        memMgrChunkFree(url->password);
        url->password = NULL;
    }

    if (url->host) {
        memMgrChunkFree(url->host);
        url->host = NULL;
    }
    
    if (url->path) {
        memMgrChunkFree(url->path);
        url->path = NULL;
    }

    if (url->query) {
        memMgrChunkFree(url->query);
        url->query = NULL;
    }
    
    if (url->fragment) {
        memMgrChunkFree(url->fragment);
        url->fragment = NULL;
    }
    
    if (url->full) {
        memMgrChunkFree(url->full);
        url->full = NULL;
    }

    memMgrChunkFree(url);
}


// Check these functions!!

void urlAddToBuf(char **buf, unsigned long *bufSize, char *str, unsigned long len)
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
    } /* else if((size = MemPtrSize(*buf)) < *bufSize + len + 1) {
        char *ptr = *buf;

        *buf = memMgrChunkNew(size + len + 2);
        if (*buf == NULL)
            return;
        memMgrChunkMove(*buf, ptr, *bufSize);
        memMgrChunkFree(ptr);
    }*/

    stringNCopy(*buf + *bufSize, str, len);
    *bufSize += len;
    *(*buf + *bufSize) = '\0';
}

boolean urlIsMustEscapeChar(int chr)
{
  return (/* Non US ASCII */
          (chr > 0x8f && chr < 0xff) ||
          /* US ASCII coded characters */
          chr < 0x1f || chr == 0x7f || 
          /* Space */
          chr == ' ' ||
          /* Reserved */
          chr == ';' || chr == '/' || chr == '?' || chr == ':' || 
          /*chr == '@' ||*/ chr == '&' || chr == '=' || chr == '+' ||
          chr == '$' || chr == ',' || chr == '\'' || chr == '!' ||
          chr == '~' ||
          /* Unwise */
          chr == '{' || chr == '}' || chr == '|' || chr == '\\' ||
          chr == '^' || chr == '[' || chr == ']' || chr == '`' ||
          chr == '(' || chr == ')' ||
          /* Delims */
          chr == '<' || chr == '>' || chr == '#' || chr == '%' || 
          chr == '"');
}

boolean isHex(int chr) // TODO: Move to /Platform
{
    return ((chr >= '0' && chr <= '9') ||
            (chr >= 'a' && chr <= 'f') ||
            (chr >= 'A' && chr <= 'F'));
}

void urlAppendEncoded(char **buf, unsigned long *bufSize, char *str)
{
    char code[4];

    while(*str)
    {
        /*if(('A' <= *str && *str <= 'Z') ||       // 'A'..'Z'
           ('a' <= *str && *str <= 'z') ||       // 'a'..'z'
           ('0' <= *str && *str <= '9') ||       // '0'..'9'
           (StrChr("-_.!~*()\'", *str) != NULL)) // unreserved*/
        if (!urlIsMustEscapeChar(*str)) {
            urlAddToBuf(buf, bufSize, str, 1);
        } else if(*str == ' ') {                     // space
            urlAddToBuf(buf, bufSize, "+", 1);
        } else {
            if (*str == '%') { // Is this a stray % or a hex encoding? No need to re-encode the %
                if ((stringIsNumeric(*(str + 1)) || stringChrInRangeInclusive(*(str + 1), 'A', 'F') ||
                     stringChrInRangeInclusive(*(str + 1), 'a', 'f')) &&
                    (stringIsNumeric(*(str + 2)) || stringChrInRangeInclusive(*(str + 2), 'A', 'F') ||
                     stringChrInRangeInclusive(*(str + 2), 'a', 'f'))) {
                        urlAddToBuf(buf, bufSize, str, 3);
                        
                        str += 3;
                        
                        continue;
                }
            }
            
            code[0] = '%';
            code[1] = "0123456789ABCDEF"[(*str >> 4) & 0x0F];
            code[2] = "0123456789ABCDEF"[ *str       & 0x0F];
            code[3] = 0;

            urlAddToBuf(buf, bufSize, code, 3);           
        }

        str++;
    }
}

char *urlEncodeURL(char *url, boolean path)
{
    char    *buf    = NULL;
    unsigned long  bufSize = 0;
    
    if (!path) {
        urlAppendEncoded(&buf, &bufSize, url);
    } else {
        while (*url) {
            if (*url == ' ') {
                urlAddToBuf(&buf, &bufSize, "%20", 3);
            } else {
                urlAddToBuf(&buf, &bufSize, url, 1);
            }
            
            url++;
        }
    }
    
    return buf;
}

boolean urlIsEncoded(char *str, boolean path)
{
    boolean encoded = true; // if there are no characters that
                            // need/were encoding, then it is encoded

    while(*str) {
        if (urlIsMustEscapeChar(*str) && !path) {
            if (*str == '%') {
                str++;
                encoded = (isHex(*str) && isHex(*(str + 1)));
                str++;
            } else {
                encoded = false;
            }
        } else if (*str == ' ') {
            encoded = false;
        }
        
        str++;
    }
    
    return encoded;
}




