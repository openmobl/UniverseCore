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
#include "Parser.h"

#include "DomDocument.h"
#include "Errors.h"
#include "MemMgr.h"
#include "SgmlExtensionHtml.h"
#include "SgmlExtensionXml.h"
#include "SgmlParser.h"
#include "WBXML.h"
#include "WMLC.h"

ParserType *parserCreateNew(ParserFormatEnum type, unsigned long flags, int *err)
{
    ParserType          *parser = NULL;
    SGMLParserType      *sgml   = NULL;
    WBXMLParserPtr      wbxml   = NULL;
    
    parser = memMgrChunkNew(sizeof(ParserType));
    if (!parser)
        return NULL;
    
    parser->format = type;
    
    switch (type) {
        case parserSGML:
        case parserXML:
        case parserHTML:
            sgml = memMgrChunkNew(sizeof(SGMLParserType));
            if (!sgml) {
                *err = memoryErrNoFreeMemory;
                goto parserCreatNew_error;
            }
            
            if (type == parserHTML) {
                sgmlParserInitialize(sgml, SGML_EXTENSION_TYPE_HTML, NULL, NULL);
                sgmlParserExtensionSetParam(sgml, SGML_EXTENSION_HTML_PARAM_FLAG, (void *)flags);
            } else {
                sgmlParserInitialize(sgml, SGML_EXTENSION_TYPE_XML, NULL, NULL);
            }
            
            parser->handler.sgml = sgml;
            break;
        case parserRSS:
            *err = parserErrFormatNotSupported;
            goto parserCreatNew_error;
            break;
        case parserWBXML:
        case parserWMLC:
            wbxml = wbxmlNewParser();
            if (!wbxml) {
                *err = memoryErrNoFreeMemory;
                goto parserCreatNew_error;
            }
            
            if (type == parserWMLC) {
                wbxmlExtensionWMLCNew(wbxml);
            }
            
            parser->handler.wbxml = wbxml;
            break;
        default:
            break;
    }
    
    return parser;

parserCreatNew_error:
    if (parser) {
        memMgrChunkFree(parser);
    }
    return NULL;
}

int parserParseText(ParserPtr parser, char *data, unsigned long length, char *encoding)
{
    int     err = systemErrNone;
    
    if (!parser)
        return parserErrNoParser;
    
    switch (parser->format) {
        case parserSGML:
        case parserXML:
        case parserHTML:
            {
                char            *convertedChunk = NULL;
                unsigned long   convertedSize   = 0;
                
                if (!sgmlConvertToUTF8(data, length, encoding, &convertedChunk, &convertedSize)) {
                        return parserErrToUTFFailed;
                }
                                            
                if (!sgmlParserParseString(parser->handler.sgml, convertedChunk, convertedSize)) {
                    memMgrChunkFree(convertedChunk);
                    return parserErrGenericParseError;
                }
                
                memMgrChunkFree(convertedChunk);
            }
            break;
        case parserRSS:
            return parserErrFormatNotSupported;
            break;
        case parserWBXML:
        case parserWMLC:
            err = wbxmlParse((unsigned char *)data, length, 0, parser->handler.wbxml);
            break;
    }
    
    return err;
}

DOMDocumentType *parserGetDOMTree(ParserPtr parser, int *err)
{
    DOMDocumentType *domTree = NULL;
    
    if (!parser) {
        *err = parserErrNoParser;
        return NULL;
    }
    
    switch (parser->format) {
        case parserSGML:
        case parserXML:
            if (!parser->handler.sgml) {
                *err = parserErrNoHandler;
                return NULL;
            }
            domTree = sgmlExtensionXmlGetDocument(parser->handler.sgml);
            break;
        case parserHTML:
            if (!parser->handler.sgml) {
                *err = parserErrNoHandler;
                return NULL;
            }
            domTree = sgmlExtensionHtmlGetDocument(parser->handler.sgml);
            break;
        case parserRSS:
            *err = parserErrFormatNotSupported;
            return NULL;
            break;
        case parserWBXML:
            break;
        case parserWMLC:
            if (!parser->handler.wbxml) {
                *err = parserErrNoHandler;
                return NULL;
            }
            domTree = wbxmlExtensionWMLCGetDocument(parser->handler.wbxml);
            break;
    }
    
    return domTree;
}

int parserDestroy(ParserPtr *parser)
{
    int     err = systemErrNone;
    
    if (!*parser)
        return parserErrNoParser;
        
    switch ((*parser)->format) {
        case parserSGML:
        case parserXML:
        case parserHTML:
            if (!(*parser)->handler.sgml) {
                err = parserErrNoHandler;
            }
            
            sgmlParserDestroy((*parser)->handler.sgml, 1);
            (*parser)->handler.sgml = NULL;
            break;
        case parserRSS:
            return parserErrFormatNotSupported;
            break;
        case parserWBXML:
        case parserWMLC:
            if (!(*parser)->handler.wbxml) {
                err = parserErrNoHandler;
            }
            
            wbxmlParserDestroy(&((*parser)->handler.wbxml));
            break;
    }
    
    memMgrChunkFree(*parser);
    
    *parser = NULL;
    
    return err;
}
