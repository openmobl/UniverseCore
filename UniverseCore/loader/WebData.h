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
 
#ifndef WebData_h
#define WebData_h

#include "config.h"


// A generic Web data type
typedef struct WebDataType {
    // Shared
    unsigned char   *data;
    char            *contentType;
    char            *charSet;
    char            *boundry;
    unsigned long   length;

    // WSP
    unsigned char   transaction_id;
    unsigned char   pdu_type;
    unsigned char   status_code;
    
    boolean         embeded;
    boolean         noPredefLength;
} WebDataType;

typedef WebDataType * WebDataPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern void webDataSetContentType(char *mime, WebDataPtr webData);
extern char *webDataGetContentType(WebDataPtr webData);
extern void webDataFree(WebDataPtr *webData);
extern void webDataInit(WebDataPtr webData);
extern void webDataFlush(WebDataPtr webData);
extern void webDataSetContentLength(unsigned long length, WebDataPtr webData);
extern unsigned long webDataGetContentLength(WebDataPtr webData);
extern unsigned char *webDataGetContent(WebDataPtr webData);
extern void webDataSetContent(unsigned char *content, WebDataPtr webData);
extern void webDataSetContentNoFree(unsigned char *content, WebDataPtr webData);
extern void webDataSetNoPredefLength(boolean value, WebDataPtr webData);
extern boolean webDataGetNoPredefLength(WebDataPtr webData);
extern boolean webDataIsMIME(char *mime, WebDataPtr webData);
extern boolean webDataIsImageMIME(WebDataPtr webData);
extern boolean webDataIsJavaScriptMIME(WebDataPtr webData);
extern boolean webDataIsCssMIME(WebDataPtr webData);
extern boolean webDataIsWmlMIME(WebDataPtr webData);
extern boolean webDataIsWmlcMIME(WebDataPtr webData);
extern boolean webDataIsWmlscMIME(WebDataPtr webData);
extern boolean webDataIsHtmlMIME(WebDataPtr webData);
extern boolean webDataIsRssMIME(WebDataPtr webData);
extern boolean webDataIsPageMIME(WebDataPtr webData);
extern boolean webDataIsEmbeded(WebDataPtr webbData);
extern void webDataSetEmbeded(boolean embeded, WebDataPtr webData);
extern void webDataSetCharSet(char *charSet, WebDataPtr webData);
extern char *webDataGetCharSet(WebDataPtr webData);

#ifdef __cplusplus
}
#endif

#endif
