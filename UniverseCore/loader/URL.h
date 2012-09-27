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
 
#ifndef URL_h
#define URL_h

#include "config.h"

#define URL_PATH_EMPTY      "/"

#define URL_AUTH_SEP            '@'
#define URL_USER_PASS_SEP       ':'
#define URL_PORT_SEP            ':'
#define URL_PATH_SEP            '/'
#define URL_QUERY_SEP           '?'
#define URL_FRAG_SEP            '#'
#define URL_AUTH_SEP_STR        "@"
#define URL_USER_PASS_SEP_STR   ":"
#define URL_PORT_SEP_STR        ":"
#define URL_PATH_SEP_STR        "/"
#define URL_QUERY_SEP_STR       "?"
#define URL_FRAG_SEP_STR        "#"

#define URL_HTTP_COMP    "http://"
#define URL_HTTPS_COMP   "https://"
#define URL_FILE_COMP    "file://"
#define URL_DATA_COMP    "data:"

// TODO: Cleanup
typedef enum URLTransport {
    URL_none,
    URL_http,
    URL_https,
    URL_file,
    URL_data,
    URL_incomplete  // a URL that is just a path or fragment
} URLTransport;

typedef struct URLType {
    URLTransport    transport;
    unsigned short  reserved1;
    
    char            *userName;
    char            *password;
    
    char            *host;
    unsigned short  port;
    
    unsigned short  reserved2;
    
    char            *path;
    char            *query;
    char            *fragment;
    
    char            *full;
} URLType;

typedef URLType *URLPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern URLPtr urlCreateURL(URLTransport transport, char *userName, char *password,
                     char *host, unsigned short port, char *path, char *query, char *fragment,
                     char *full);
extern int urlParseIncompleteURL(char *urlP, char **path, char **query, char **frag);
extern int urlParseHTTPURL(char *urlP, char **userName, char **password, char **host,
                    unsigned short *port, char **path, char **query, char **frag);
extern URLPtr urlParseURL(char *urlP, int *error);

extern URLTransport urlGetTransport(URLPtr url);
extern char *urlGetUserName(URLPtr url);
extern char *urlGetPassword(URLPtr url);
extern char *urlGetHost(URLPtr url);
extern unsigned short urlGetPort(URLPtr url);
extern char *urlGetPath(URLPtr url);
extern char *urlGetQuery(URLPtr url);
extern char *urlGetFragment(URLPtr url);
extern char *urlGetFull(URLPtr url);

extern void urlStripDoubleDots(char *s);
extern void urlStripSingleDots(char *s);

extern URLPtr urlCombineURLs(URLPtr mainURL, URLPtr incomplete, int *error);

extern void urlDestroyURL(URLPtr url);

extern char *urlEncodeURL(char *url, boolean path);
extern boolean urlIsEncoded(char *str, boolean path);
extern boolean urlIsMustEscapeChar(int chr);

#ifdef __cplusplus
}
#endif

#endif
