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
#include "WMLC.h"

#include "DomDocument.h"
#include "DomElement.h"
#include "DomText.h"
#include "DomComment.h"
#include "WBXML.h"

// WML Tages: wap-191-wml-20000219-a.pdf
const char *wmlTag[] = {
    NULL,               // 00, SWITCH_PAGE
    NULL,               // 01, END
    NULL,               // 02, ENTITY
    NULL,               // 03, STR_I
    NULL,               // 04, LITERAL
    "-",                // 05
    "-",                // 06
    "-",                // 07
    "-",                // 08
    "-",                // 09
    "-",                // 0A
    "-",                // 0B
    "-",                // 0C
    "-",                // 0D
    "-",                // 0E
    "-",                // 0F
    
    "-",                // 10
    "-",                // 11
    "-",                // 12
    "-",                // 13
    "-",                // 14
    "-",                // 15
    "-",                // 16
    "-",                // 17
    "-",                // 18
    "-",                // 19
    "-",                // 1A
    "pre",              // 1B
    "a",                // 1C
    "td",               // 1D
    "tr",               // 1E
    "table",            // 1F
    
    "p",                // 20
    "postfield",        // 21
    "anchor",           // 22
    "access",           // 23
    "b",                // 24
    "big",              // 25
    "br",               // 26
    "card",             // 27
    "do",               // 28
    "em",               // 29
    "fieldset",         // 2A
    "go",               // 2B
    "head",             // 2C
    "i",                // 2D
    "img",              // 2E
    "input",            // 2F
    
    "meta",             // 30
    "noop",             // 31
    "prev",             // 32
    "onevent",          // 33
    "optgroup",         // 34
    "option",           // 35
    "refresh",          // 36
    "select",           // 37
    "small",            // 38
    "strong",           // 39
    "-",                // 3A
    "template",         // 3B
    "timer",            // 3C
    "u",                // 3D
    "setvar",           // 3E
    "wml"               // 3F
};

// WML Attributes: wap-191-wml-20000219-a.pdf
const WBXMLAttrValuePair wmlAttrValuePair[] = {
    { NULL,                 NULL,                                   0 },    // 00
    { NULL,                 NULL,                                   0 },    // 01
    { NULL,                 NULL,                                   0 },    // 02
    { NULL,                 NULL,                                   0 },    // 03
    { NULL,                 NULL,                                   0 },    // 04
    { "accept-charset",     NULL,                                   0 },    // 05
    { "align",              "bottom",                               0 },    // 06
    { "align",              "center",                               0 },    // 07
    { "align",              "left",                                 0 },    // 08
    { "align",              "middle",                               0 },    // 09
    { "align",              "right",                                0 },    // 0A
    { "align",              "top",                                  0 },    // 0B
    { "alt",                NULL,                                   0 },    // 0C
    { "content",            NULL,                                   0 },    // 0D
    { NULL,                 NULL,                                   0 },    // 0E
    { "domain",             NULL,                                   0 },    // 0F

    { "emptyok",            "false",                                0 },    // 10
    { "emptyok",            "true",                                 0 },    // 11
    { "format",             NULL,                                   0 },    // 12
    { "height",             NULL,                                   0 },    // 13
    { "hspace",             NULL,                                   0 },    // 14
    { "ivalue",             NULL,                                   0 },    // 15
    { "iname",              NULL,                                   0 },    // 16
    { NULL,                 NULL,                                   0 },    // 17
    { "label",              NULL,                                   0 },    // 18
    { "localsrc",           NULL,                                   0 },    // 19
    { "maxlength",          NULL,                                   0 },    // 1A
    { "method",             "get",                                  0 },    // 1B
    { "method",             "post",                                 0 },    // 1C
    { "mode",               "nowrap",                               0 },    // 1D
    { "mode",               "wrap",                                 0 },    // 1E
    { "multiple",           "false",                                0 },    // 1F

    { "multiple",           "true",                                 0 },    // 20
    { "name",               NULL,                                   0 },    // 21
    { "newcontext",         "false",                                0 },    // 22
    { "newcontext",         "true",                                 0 },    // 23
    { "onpick",             NULL,                                   0 },    // 24
    { "onenterbackward",    NULL,                                   0 },    // 25
    { "onenterforward",     NULL,                                   0 },    // 26
    { "ontimer",            NULL,                                   0 },    // 27
    { "optimal",            "false",                                0 },    // 28
    { "optimal",            "true",                                 0 },    // 29
    { "path",               NULL,                                   0 },    // 2A
    { NULL,                 NULL,                                   0 },    // 2B
    { NULL,                 NULL,                                   0 },    // 2C
    { NULL,                 NULL,                                   0 },    // 2D
    { "scheme",             NULL,                                   0 },    // 2E
    { "sendreferer",        "false",                                0 },    // 2F

    { "sendreferer",        "true",                                 0 },    // 30
    { "size",               NULL,                                   0 },    // 31
    { "src",                NULL,                                   0 },    // 32
    { "ordered",            "true",                                 0 },    // 33
    { "ordered",            "false",                                0 },    // 34
    { "tabindex",           NULL,                                   0 },    // 35
    { "title",              NULL,                                   0 },    // 36
    { "type",               NULL,                                   0 },    // 37
    { "type",               "accept",                               0 },    // 38
    { "type",               "delete",                               0 },    // 39
    { "type",               "help",                                 0 },    // 3A
    { "type",               "password",                             0 },    // 3B
    { "type",               "onpick",                               0 },    // 3C
    { "type",               "onenterbackward",                      0 },    // 3D
    { "type",               "onenterforward",                       0 },    // 3E
    { "type",               "ontimer",                              0 },    // 3F

    { NULL,                 NULL,                                   0 },    // 40
    { NULL,                 NULL,                                   0 },    // 41
    { NULL,                 NULL,                                   0 },    // 42
    { NULL,                 NULL,                                   0 },    // 43
    { NULL,                 NULL,                                   0 },    // 44
    { "type",               "options",                              0 },    // 45
    { "type",               "prev",                                 0 },    // 46
    { "type",               "reset",                                0 },    // 47
    { "type",               "text",                                 0 },    // 48
    { "type",               "vnd.",                                 1 },    // 49
    { "href",               NULL,                                   0 },    // 4A
    { "href",               "http://",                              1 },    // 4B
    { "href",               "https://",                             1 },    // 4C
    { "value",              NULL,                                   0 },    // 4D
    { "vspace",             NULL,                                   0 },    // 4E
    { "width",              NULL,                                   0 },    // 4F

    { "xml:lang",           NULL,                                   0 },    // 50
    { NULL,                 NULL,                                   0 },    // 51
    { "align",              NULL,                                   0 },    // 52
    { "columns",            NULL,                                   0 },    // 53
    { "class",              NULL,                                   0 },    // 54
    { "id",                 NULL,                                   0 },    // 55
    { "forua",              "false",                                0 },    // 56
    { "forua",              "true",                                 0 },    // 57
    { "src",                "http://",                              1 },    // 58
    { "src",                "https://",                             1 },    // 59
    { "http-equiv",         NULL,                                   0 },    // 5A
    { "http-equiv",         "Content-Type",                         0 },    // 5B
    { "content",            "application/vnd.wap.wmlc;charset=",    1 },    // 5C
    { "http-equiv",         "Expires",                              0 },    // 5D
    { "accesskey",          NULL,                                   0 },    // 5E
    { "enctype",            NULL,                                   0 },    // 5F
    
    { "enctype",            "application/x-www-form-urlencoded",    0 },    // 60
    { "enctype",            "multipart/form-data",                  0 }     // 61
};

// WML Attribute Values: wap-191-wml-20000219-a.pdf
const char *wmlAttrVal[] = { 
    ".com/",            // 85
    ".edu/",            // 86
    ".net/",            // 87
    ".org/",            // 88
    "accept",           // 89
    "bottom",           // 8A
    "clear",            // 8B
    "delete",           // 8C
    "help",             // 8D
    "http://",          // 8E
    "http://www.",      // 8F
    
    "https://",         // 90
    "https://www.",     // 91
    "?",                // 92
    "middle",           // 93
    "nowrap",           // 94
    "onpick",           // 95
    "onenterbackward",  // 96
    "onenterforward",   // 97
    "ontimer",          // 98
    "options",          // 99
    "password",         // 9A
    "reset",            // 9B
    "?",                // 9C
    "text",             // 9D
    "top",              // 9E
    "unknown",          // 9F
    "wrap",             // A0
    "www.",             // A1
};


int wbxmlExtensionWMLCNew(WBXMLParserPtr parser)
{
    WBXMLHandlersType       handlers;
    WMLCExtContextPtr       context;

    memMgrChunkSet(&handlers, 0, sizeof(WBXMLHandlersType));

    handlers.preparse     = wbxmlExtensionWMLCInitialize;
    handlers.elementBegin = wbxmlExtensionWMLCElementBegin;
    handlers.elementEnd   = wbxmlExtensionWMLCElementEnd;
    handlers.attributeNew = wbxmlExtensionWMLCAttributeNew;
    handlers.textNew      = wbxmlExtensionWMLCTextNew;
    handlers.postparse    = wbxmlExtensionWMLCDeinitialize;
    handlers.destroy      = wbxmlExtensionWMLCDestroy;

    context = (WMLCExtContextPtr)memMgrChunkNew(sizeof(WMLCExtContextType));
    if (!context)
        return wbxmlErrMem;
    memMgrChunkSet(context, 0, sizeof(WMLCExtContextType));
    
    wbxmlParserInitialize(parser, wbxmlWMLC, &handlers, context);
    
    wbxmlParserAddTokenLookups(parser, (char **)wmlTag, (WBXMLAttrValuePair *)wmlAttrValuePair, (char **)wmlAttrVal);

    return 0;
}

void wbxmlExtensionWMLCDestroy(WBXMLParserPtr parser, void *userContext)
{
    WMLCExtContextPtr   ext = (WMLCExtContextPtr)userContext;
    
    if (ext->document)
        domDocumentDestroy(ext->document);

    memMgrChunkFree(ext);
}

void wbxmlExtensionWMLCInitialize(WBXMLParserPtr parser, void *userContext)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)userContext;

    if (ext->document)
        domDocumentDestroy(ext->document);

    ext->document    = domDocumentNew();
    ext->currElement = NULL;
}

void wbxmlExtensionWMLCElementBegin(WBXMLParserPtr parser, void *userContext, const char *elementName)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)userContext;
    DOM_ELEMENT *element;

    // This is a processing instruction of some sort, we shall ignore it.
    if (elementName[0] == '?') 
        return; 

    element = domElementNew((DOMString)elementName);

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, element);
    else
        domNodeAppendChild(ext->document, element);

    ext->currElement = element;
}

void wbxmlExtensionWMLCElementEnd(WBXMLParserPtr parser, void *userContext, const char *elementName)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)userContext;

    // End of processing instruction.
    if (elementName[0] == '?')
        return;

    if (ext->currElement)
        ext->currElement = ext->currElement->parent;
}

void wbxmlExtensionWMLCAttributeNew(WBXMLParserPtr parser, void *userContext, const char *attributeName, const char *attributeValue)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)userContext;

    if (ext->currElement)
        domElementSetAttribute(ext->currElement, (DOMString)attributeName, (DOMString)attributeValue);
}

void wbxmlExtensionWMLCTextNew(WBXMLParserPtr parser, void *userContext, const char *textValue)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)userContext;
    DOM_TEXT *text = domTextNew((DOMString)textValue);

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, text);
    else
        domNodeAppendChild(ext->document, text);
}

void wbxmlExtensionWMLCDeinitialize(WBXMLParserPtr parser, void *userContext)
{

}

DOMDocumentType *wbxmlExtensionWMLCGetDocument(WBXMLParserPtr parser)
{
    WMLCExtContextPtr ext = (WMLCExtContextPtr)wbxmlParserGetContext(parser);

    return ext->document;
}

