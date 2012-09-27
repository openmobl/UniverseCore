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
#ifndef Parser_h
#define Parser_h

#include "config.h"
#include "DomNode.h"
#include "DomDocument.h"
#include "SgmlParser.h"
#include "WBXML.h"

typedef enum ParserFormatEnum {
    // SGML Parser
    parserSGML,     // generic
    parserXML,
    parserHTML,
    parserRSS,
    
    // WBXMLParser
    parserWBXML,    // generic
    parserWMLC
} ParserFormatEnum;

typedef struct ParserType {
    ParserFormatEnum    format;
    
    union {
        SGMLParserPtr   sgml;
        WBXMLParserPtr  wbxml;
    } handler;
    
    
} ParserType;

typedef ParserType *ParserPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern ParserType *parserCreateNew(ParserFormatEnum type, unsigned long flags, int *err);
extern int parserParseText(ParserPtr parser, char *data, unsigned long length, char *encoding);
extern DOMDocumentType *parserGetDOMTree(ParserPtr parser, int *err);
extern int parserDestroy(ParserPtr *parser);

#ifdef __cplusplus
}
#endif

#endif
