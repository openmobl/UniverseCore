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
#include "WebData.h"

#include "MemMgr.h"


void webDataInit(WebDataPtr webData)
{
    memMgrChunkSet(webData, 0, sizeof(WebDataType));
}

void webDataFree(WebDataPtr *webData)
{
    memMgrChunkFree((*webData)->data);
    memMgrChunkFree((*webData)->contentType);
    memMgrChunkFree((*webData)->charSet);
    memMgrChunkFree((*webData)->boundry);
    
    webDataInit((*webData));
    
    memMgrChunkFree((*webData));
    
    *webData = NULL;
}

void webDataFlush(WebDataPtr webData)
{
    memMgrChunkFree(webData->data);
    memMgrChunkFree(webData->contentType);
    memMgrChunkFree(webData->charSet);
    memMgrChunkFree(webData->boundry);
    
    webDataInit(webData);
}

void webDataSetContentType(char *mime, WebDataPtr webData)
{
    if (!webData)
        return;
    
    webData->contentType = stringDup(mime);
}

char *webDataGetContentType(WebDataPtr webData)
{
    return webData->contentType;
}

void webDataSetContentLength(unsigned long length, WebDataPtr webData)
{
    webData->length = length;
}

unsigned long webDataGetContentLength(WebDataPtr webData)
{
    return webData->length;
}

unsigned char *webDataGetContent(WebDataPtr webData)
{
    return webData->data;
}

void webDataSetContent(unsigned char *content, WebDataPtr webData)
{
    memMgrChunkFree(webData->data);

    webData->data = content;
}

void webDataSetContentNoFree(unsigned char *content, WebDataPtr webData)
{
    webData->data = content;
}

void webDataSetNoPredefLength(boolean value, WebDataPtr webData)
{
    webData->noPredefLength = value;
}

boolean webDataGetNoPredefLength(WebDataPtr webData)
{
    return webData->noPredefLength;
}

boolean webDataIsMIME(char *mime, WebDataPtr webData)
{
    char        *contentType    = webDataGetContentType(webData);
    boolean     isMIMEPlus      = !stringNCompareCaseless(contentType, mime, stringLen(mime));
    boolean     isMIME          = !stringCompareCaseless(contentType, mime);
    
    if (!isMIME && !isMIMEPlus)
        return false;
    
    if (isMIMEPlus && !isMIME) {
        if (!stringChr(contentType, ';'))
            return false;
    }
    
    return true;
}

boolean webDataIsImageMIME(WebDataPtr webData)
{
    return !stringNCompareCaseless(webDataGetContentType(webData), "image/", 6);
}

boolean webDataIsWmlscMIME(WebDataPtr webData)
{
    return webDataIsMIME("application/vnd.wap.wmlscriptc", webData);
    //return !stringCompareCaseless(webDataGetContentType(webData),
    //        "application/vnd.wap.wmlscriptc");
}

boolean webDataIsWmlcMIME(WebDataPtr webData)
{
    return webDataIsMIME("application/vnd.wap.wmlc", webData);
    //return !stringCompareCaseless(webDataGetContentType(webData),
    //        "application/vnd.wap.wmlc");
}

boolean webDataIsWmlMIME(WebDataPtr webData)
{
    return webDataIsMIME("application/wml+xml", webData) ||
           webDataIsMIME("text/vnd.wap.wml", webData);
}

boolean webDataIsHtmlMIME(WebDataPtr webData)
{
    return webDataIsMIME("application/vnd.wap.xhtml+xml", webData) ||
           webDataIsMIME("application/xhtml+xml", webData) ||
           webDataIsMIME("text/html", webData);
    //char  *contentType = webDataGetContentType(webData);
    //return !stringCompareCaseless(contentType, "application/vnd.wap.xhtml+xml") || 
    //       !stringCompareCaseless(contentType, "application/xhtml+xml") || 
    //       !stringCompareCaseless(contentType, "application/wml+xml") ||
    //       !stringCompareCaseless(contentType, "text/html") ||
    //       !stringCompareCaseless(contentType, "text/vnd.wap.wml");
}

boolean webDataIsRssMIME(WebDataPtr webData)
{
    return webDataIsMIME("application/rss+xml", webData);
    //return !stringCompareCaseless(webDataGetContentType(webData),
    //        "application/rss+xml");
}

boolean webDataIsCssMIME(WebDataPtr webData)
{
    return webDataIsMIME("text/css", webData);
    //return !stringCompareCaseless(webDataGetContentType(webData),
    //        "text/css");
}

boolean webDataIsJavaScriptMIME(WebDataPtr webData)
{
    return webDataIsMIME("text/javascript", webData) ||
           webDataIsMIME("application/ecmascript", webData) ||
           webDataIsMIME("application/javascript", webData) ||
           webDataIsMIME("application/x-javascript", webData) ||
           webDataIsMIME("text/x-javascript", webData);
}

boolean webDataIsPageMIME(WebDataPtr webData)
{
    return webDataIsWmlscMIME(webData) || webDataIsWmlcMIME(webData) ||
           webDataIsHtmlMIME(webData)  || webDataIsWmlMIME(webData) ||
           webDataIsRssMIME(webData);
}

boolean webDataIsEmbeded(WebDataPtr webData)
{
    return webData->embeded;
}

void webDataSetEmbeded(boolean embeded, WebDataPtr webData)
{
    webData->embeded = embeded;
}

void webDataSetCharSet(char *charSet, WebDataPtr webData)
{
    webData->charSet = stringDup(charSet);
}

char *webDataGetCharSet(WebDataPtr webData)
{
    return webData->charSet;
}

