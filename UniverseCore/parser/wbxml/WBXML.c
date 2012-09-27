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
/********************************************************************************
 * WBXML parsing code from WAPUniverse, PalmOS WAP Browser                      *
 * Copyright (c) 1999-2007 Filip Onkelinx.                                      *
 * Portions Copyright (c) 2006-2007 Donald C. Kirker.                           *
 *                                                                              *
 * Released under the GNU GPL v2.                                               *
 ********************************************************************************/
#include "config.h"
#include "WBXML.h"

#include "MemMgr.h"
#include "WAPUtils.h"
#include "WMLC.h"

// Public Identifiers: wap-192-wbxml-20010725-a.pdf
const char *wbxmlPublicIdentifier[] = {
    NULL,                                   // 00, String table index follows; public identifier is encoded as a literal in the string  table.
    NULL,                                   // 01, Unknown or missing public identifier.
    "-//WAPFORUM//DTD WML 1.0//EN",         // 02, WML 1.0 
    "-//WAPFORUM//DTD WTA 1.0//EN",         // 03, WTA Event 1.0, Depriciated
    "-//WAPFORUM//DTD WML 1.1//EN",         // 04, WML 1.1
    "-//WAPFORUM//DTD SI 1.0//EN",          // 05, Service Indication 1.0
    "-//WAPFORUM//DTD SL 1.0//EN",          // 06, Service Loading 1.0
    "-//WAPFORUM//DTD CO 1.0//EN",          // 07, Cache Operation 1.0
    "-//WAPFORUM//DTD CHANNEL 1.1//EN",     // 08, Channel 1.1
    "-//WAPFORUM//DTD WML 1.2//EN",         // 09, WML 1.2
    "-//WAPFORUM//DTD WML 1.3//EN",         // 0A, WML 1.3
    "-//WAPFORUM//DTD PROV 1.0//EN",        // 0B, Provisioning 1.0
    "-//WAPFORUM//DTD WTA-WML 1.2//EN",     // 0C, WTA-WML 1.2
    "-//WAPFORUM//DTD CHANNEL 1.2//EN"      // 0D, Channel 1.2
                                            // 0E - 7F, Reserved
};

int wbxmlPushStack(char *tagname, WBXMLParserPtr wbxml)
{
    char    **ptr;
    int     i;

    if (wbxml->stack == NULL) {
        wbxml->maxstack = 10;
        wbxml->stack = memMgrChunkNew(wbxml->maxstack * sizeof(char *));
        if (!wbxml->stack) {
            wbxml->maxstack = 0;
            return wbxmlErrMem;
        }
    }
    
    if (wbxml->stackDepth >= wbxml->maxstack) {
        i = wbxml->maxstack;
        wbxml->maxstack += 10;
        ptr = wbxml->stack;
        wbxml->stack = memMgrChunkNew(wbxml->maxstack * sizeof(char *));
        if (!wbxml->stack) {
            wbxml->maxstack -= 10;
            wbxml->stack = ptr;
            return wbxmlErrMem;
        }
        
        while (i > 0) {
            i--;
            wbxml->stack[i] = ptr[i];
        }
           
        memMgrChunkFree(ptr);
    }
    
    wbxml->stack[wbxml->stackDepth++] = stringDup(tagname);//wbxml->tag;

    return 0;    
}

void wbxmlFreeStack(WBXMLParserPtr wbxml)
{
    int     i = 0;
    
    if (!wbxml->stack)
        return;
    
    for (i = 0; wbxml->stack[i] && (i < wbxml->maxstack); i++) {
        memMgrChunkFree(wbxml->stack[i]);
    }
    
    wbxml->stackDepth = 0;
    
    if (wbxml->stack != NULL){
        memMgrChunkFree(wbxml->stack);
        wbxml->stack = NULL;
    }
}

char *wbxmlPopStack(WBXMLParserPtr wbxml)
{
    char *c = NULL;

    if (wbxml->stack == NULL)
        return NULL;
    
    wbxml->stackDepth--;
    
    if (wbxml->stackDepth == 0) {
        memMgrChunkFree(wbxml->stack);
        wbxml->stack = NULL;
    } else {
        c = stringDup(wbxml->stack[wbxml->stackDepth]);
    
        memMgrChunkFree(wbxml->stack[wbxml->stackDepth]);
    }
    
    return c;    
}



int wbxmlGetAttributeValue(WBXMLParserPtr wbxml, char *attrstr)
{
    unsigned char   c;
    unsigned long   l, j;

    attrstr[0] = '\0';
    j = 0;
    c = wbxml->data[wbxml->offset++];
    
    while ((c > 128) || (c == ENTITY) || (c == STR_I) || (c == STR_T) ||
           (c == SWITCH_PAGE) || (c == PI) || ((c >= EXT_I_0) && (c <= EXT_I_2)) ||
           ((c >= EXT_T_0) && (c <= EXT_T_2))) {

            switch (c) {
                case ENTITY:
                    l = wapReadInt(wbxml->data, &(wbxml->offset));
                    attrstr[j++] = '<';
                    attrstr[j++] = '!';
                    attrstr[j++] = '>';
                    attrstr[j] = '\0';
                    break;

                case STR_I:
                case EXT_I_0:
                case EXT_I_1:
                case EXT_I_2:
#ifdef WBXML_APPEND_VAR_BYTES
                    if (c != STR_I) {
                        attrstr[j++] = c;
                    }
#endif
                    while (wbxml->data[wbxml->offset] != 0) {
                        attrstr[j++] = wbxml->data[wbxml->offset++];
                    }
#ifdef WBXML_APPEND_VAR_BYTES
                    if (c != STR_I) {
                        attrstr[j++] = END;
                    }
#endif
                    attrstr[j] = '\0';
                    
                    wbxml->offset++;   
                    break;

                case PI: // processing instruction               
                    break; 

                case EXT_T_0:
                case EXT_T_1:
                case EXT_T_2: // 
                    l = wapReadInt(wbxml->data, &(wbxml->offset));
#ifdef WBXML_APPEND_VAR_BYTES
                    attrstr[j++] = c - 0x40;
#endif

                    while (wbxml->strTable[l] !=0)
                        attrstr[j++] = wbxml->strTable[l++];
#ifdef WBXML_APPEND_VAR_BYTES
                    attrstr[j++] = END;
#endif
                    attrstr[j] = '\0';

                    /*if (c==EXT_T_1)
                        ;//escaped
                    if (c==EXT_T_2)
                        ;//unescaped*/
                    break;
                case STR_T: 
                    l = wapReadInt(wbxml->data, &(wbxml->offset));
                    while (wbxml->strTable[l] != 0)
                        attrstr[j++] = wbxml->strTable[l++];                   
                    attrstr[j] = '\0';
                    break;
                
                case EXT_0:
                case EXT_1:
                case EXT_2:
                    //Single byte extension tokens
                    l = wbxml->data[wbxml->offset++];
                    break;
                
                case OPAQUE:
                    break;

                default:
                    //if ((c >= 0x85) && (c <= 0xA1 )) {
                    l = 0;
                    while (wbxml->wbxmlAttrValList[c - 0x85][l] != 0) {
                        attrstr[j++] = wbxml->wbxmlAttrValList[c - 0x85][l++];        
                    }
                    attrstr[j] = '\0';
                    //}
                    break;
        }

        c = wbxml->data[wbxml->offset++];
    }
    
    wbxml->offset--;
    
    return 0;
}

char *wbxmlGetAttribute(WBXMLParserPtr wbxml, unsigned char attr)
{
    unsigned char   c, attr_id;

    wbxml->attrstr[0] = '\0';
    
    if (wbxml->data[wbxml->offset] & WBXML_HAS_ATTRIBUTES) {
        wbxml->offset++;
        c = wbxml->data[wbxml->offset++];
        
        while (c != END) {
            attr_id = c;
            if (attr_id == attr){
                wbxmlGetAttributeValue(wbxml, wbxml->attrstr);
                return wbxml->attrstr;
            } else {
                wbxmlGetAttributeValue(wbxml, wbxml->tmpstr);
            }
            
            c = wbxml->data[wbxml->offset++];
        }
        
        wbxml->offset--;
    }
    
    return NULL;
}

/*int wbxmlGetPredefAttribute(unsigned char token, WBXMLParserPtr wbxml)
{
    
}*/

void wbxmlGetAttributes(WBXMLParserPtr wbxml)
{
    unsigned char   c, attr_id;
    unsigned long   l = 0, i = 0, j = 0;

    wbxml->attrstr[0] = '\0';
    
    if (wbxml->data[wbxml->offset] & WBXML_HAS_ATTRIBUTES) {
        wbxml->offset++;
        c = wbxml->data[wbxml->offset++];
        
        while (c != END) {
            i = 0;
            attr_id = c;
            
            if (attr_id == LITERAL) {
                l = wapReadInt(wbxml->data, &(wbxml->offset));
                j = 0;
                while (wbxml->strTable[l] != 0)
                    wbxml->tmpstr[j++] = wbxml->strTable[l++];                   
                wbxml->tmpstr[j] = '\0';
            } else {
                int attrOff = c; // wbxmlGetPredefAttribute(c, wbxml);
                
                stringCopy(wbxml->tmpstr, wbxml->wbxmlAttrValPairList[attrOff].attr);
                
                if (wbxml->wbxmlAttrValPairList[attrOff].value) {
                    stringCopy(wbxml->attrstr, wbxml->wbxmlAttrValPairList[attrOff].value);
                    i = stringLen(wbxml->wbxmlAttrValPairList[attrOff].value);
                }
                
                //if (wbxml->wbxmlAttrValPairList[attrOff].partialValue) {
                //    i = stringLen(wbxml->wbxmlAttrValPairList[attrOff].value);
                //}
            }
            wbxmlGetAttributeValue(wbxml, wbxml->attrstr + i);
            
            if (wbxml->handlers.attributeNew)
                    wbxml->handlers.attributeNew(wbxml, wbxml->userContext, wbxml->tmpstr, wbxml->attrstr);
            
            c = wbxml->data[wbxml->offset++];
        }
        
        wbxml->offset--;
    }
}



int wbxmlReadElement(WBXMLParserPtr wbxml)
{
    int             startoffset;
    unsigned char   c;
    char            tagname[256];
    unsigned long   i = 0, j = 0;

    startoffset = wbxml->offset;
    wbxml->tag = wbxml->data[wbxml->offset] & WBXML_TAG_ID;
    c = wbxml->data[wbxml->offset];


    if ((wbxml->tag >= 0x05) && (wbxml->tag <= 0x3F)) {
        stringCopy(tagname, wbxml->wbxmlTagList[wbxml->tag]);
    } else if (wbxml->tag == LITERAL) { // we have a LITERAL
        i = wapReadInt(wbxml->data, &(wbxml->offset));
        j = 0;
        
        while (wbxml->strTable[i] != 0)
            tagname[j++] = wbxml->strTable[i++];                   
        tagname[j] = '\0';
    }
    if (wbxml->handlers.elementBegin) {
        wbxml->handlers.elementBegin(wbxml, wbxml->userContext, tagname);
    }
    //wbxmlGetAttribute(wbxml, END);
    
    wbxmlGetAttributes(wbxml);
    
    // check for content
    if (c & WBXML_HAS_CONTENT) {
        wbxmlPushStack(tagname, wbxml);
    } else {
        if (wbxml->handlers.elementEnd)
            wbxml->handlers.elementEnd(wbxml, wbxml->userContext, tagname);
    }
    
    return 0;
}


WBXMLParserPtr wbxmlNewParser(void)
{
    WBXMLParserPtr ret = (WBXMLParserPtr)memMgrChunkNew(sizeof(WBXMLParserType));

    if (!ret)
        return NULL;

    memMgrChunkSet(ret, 0, sizeof(WBXMLParserType));

    ret->type = wbxmlCustom;

    return ret;
}

void wbxmlParserAddTokenLookups(WBXMLParserPtr parser, char **wbxmlTagList,
                                WBXMLAttrValuePair *wbxmlAttrValPairList, char **wbxmlAttrValList)
{
    parser->wbxmlTagList            = wbxmlTagList;
    parser->wbxmlAttrValPairList    = wbxmlAttrValPairList;
    parser->wbxmlAttrValList        = wbxmlAttrValList;
}

int wbxmlParserInitialize(WBXMLParserPtr parser, WBXMLExtensionType type, WBXMLHandlersPtr handlers, void *userContext)
{
    memMgrChunkSet(parser, 0, sizeof(WBXMLParserType));

    parser->type                = type;
    parser->userContext         = userContext;

    if (handlers) {
        memMgrChunkMove(&parser->handlers, handlers, sizeof(WBXMLHandlersType));
    }

    /*switch (parser->type) {
        case wbxmlWMLC:     // Compiled WML
            wmlcTagsetInitialize(parser);
            break;
        case wbxmlSI:       // Service Indication
        case wbxmlSL:       // Service Loading
        case wbxmlCO:       // Cache Object
            //pushTagsetInitialize(parser);
            //break;
        case wbxmlProv:     // Provisioning
            //provTagsetInitialize(parser);
            //break;
        default:
            break;
    }*/

    return 0;
}

WBXMLExtensionType wbxmlGetExtension(WBXMLParserPtr parser)
{
    return parser->type;
}

void *wbxmlParserGetContext(WBXMLParserPtr parser)
{
    return parser->userContext;
}

int wbxmlParseHeader(unsigned char *data, unsigned long length, WBXMLParserPtr wbxml)
{
    int     i;
    int     strTableSz;

    wbxml->stackDepth  = 0;
    wbxml->stack       = NULL;
    wbxml->offset      = 0;
    wbxml->data        = data;
    wbxml->version     = wbxml->data[wbxml->offset++];
    wbxml->publicID    = wbxml->data[wbxml->offset++];
    if (wbxml->publicID == 0) {
        wbxml->offset++;
    }

    // get the CharSet
    wbxml->charSet     = wbxml->data[wbxml->offset++];
    
    // get the StringTable size, allocate memory and load it.
    strTableSz         = wapReadInt(wbxml->data, &(wbxml->offset));
    wbxml->strTable    = memMgrChunkNew(strTableSz + 1);
    wbxml->attrstr     = memMgrChunkNew(TMP_BLOCK_SIZE);
    wbxml->tmpstr      = memMgrChunkNew(TMP_BLOCK_SIZE);
    if (!wbxml->strTable || !wbxml->attrstr || !wbxml->tmpstr) {
        return wbxmlErrMem;
    }
    
    for (i = 0; i < strTableSz; i++) {
        wbxml->strTable[i] = wbxml->data[wbxml->offset++];
    }
    wbxml->strTable[i] = 0;
    
    wbxml->decksize    = length - wbxml->offset;
    wbxml->data        = data + wbxml->offset;
    wbxml->offset      = 0;

    return 0;
}

int wbxmlParse(unsigned char *data, unsigned long length, int offset, WBXMLParserPtr wbxml)
{
    char            *temp;
    char            *tagname;
    int             i, j;

    if (!data || !length || (offset > length)) {
        return wbxmlErrParamError;
    }

    if (!wbxml->wbxmlTagList || !wbxml->wbxmlAttrValPairList || !wbxml->wbxmlAttrValList) {
        return wbxmlErrInvalidTokenLookupArrays;
    }

    if (wbxml->handlers.preparse)
        wbxml->handlers.preparse(wbxml, wbxml->userContext);

    /* The largest buffer we can have is the decksize. This will allow for:
     *   <wml><card><p><-- insert text here --></p></card></wml>
     * where "<-- insert text here -->" may be londer than 4kb.
     */
    temp = memMgrChunkNew(length - offset); //old: TMP_BLOCK_SIZE
    if (!temp) {
        return wbxmlErrMem;
    }

    wbxmlParseHeader(data, length, wbxml);

     wbxml->offset = offset;

    // Parse the WBXML bytecode
    while (wbxml->offset < wbxml->decksize) {
        switch (wbxml->data[wbxml->offset]) {
            case SWITCH_PAGE:
                break;
            case END:
                tagname = wbxmlPopStack(wbxml);

                if (wbxml->handlers.elementEnd)
                    wbxml->handlers.elementEnd(wbxml, wbxml->userContext, tagname);

                memMgrChunkFree(tagname);

                if (wbxml->stackDepth == 0) {  // end of main tag, needed because angelOne seems to sent some trailing garbage.
                    goto wbxmlParse_cleanup;
                }
                break;
            case ENTITY:
                wbxml->offset++;
                j = wapReadInt(wbxml->data, &(wbxml->offset)); // UCS-4 character code
                temp[0] = j;
                temp[1] = '\0';
                
                if (wbxml->handlers.textNew)
                    wbxml->handlers.textNew(wbxml, wbxml->userContext, temp);
                
                wbxml->offset--;
                break;
            case STR_I: //Inline String follows
                wbxml->offset++;
                j = 0;
                
                while (wbxml->data[wbxml->offset] != 0) {
                    temp[j++] = wbxml->data[wbxml->offset++];   
                }
                // TODO: Rely on stringLen and stringCopy?
                /*stringCopy(temp + j, wbxml->data + wbxml->offset);
                j += stringLen(wbxml->data + wbxml->offset);
                wbxml->offset += stringLen(wbxml->data + wbxml->offset);*/
                temp[j] = '\0';

                if (wbxml->handlers.textNew)
                    wbxml->handlers.textNew(wbxml, wbxml->userContext, temp);
                break;
            case EXT_I_0: // variable, inline, escaped
            case EXT_I_1: // variable, inline, unescaped
            case EXT_I_2: // variable, inline
#ifdef WBXML_APPEND_VAR_BYTES
                temp[0] = wbxml->data[wbxml->offset];
                for(j = 1; wbxml->data[++wbxml->offset]; j++) {
#else
                stringCopy(temp, "$(");
                for(j = 2; wbxml->data[++wbxml->offset]; j++) {
#endif
                    temp[j] = wbxml->data[wbxml->offset];
                }
#ifdef WBXML_APPEND_VAR_BYTES
                temp[j++] = END;
#else
                // TODO: esc, noesc, unesc
                stringCopy(temp + j, ")");
                j++;
#endif
                temp[j] = 0;
                
                if (wbxml->handlers.textNew)
                    wbxml->handlers.textNew(wbxml, wbxml->userContext, temp);
                break;
            case EXT_T_0: // variable, lookup table, escaped
            case EXT_T_1: // variable, lookup table, unescaped
            case EXT_T_2: // variable, lookup table
                wbxml->offset++;
                
#ifdef WBXML_APPEND_VAR_BYTES
                temp[0] = wbxml->data[wbxml->offset];
                i = 1;
#else
                stringCopy(temp, "$(");
                i = 2;
#endif
                j = wapReadInt(wbxml->data, &(wbxml->offset));

                stringCopy(temp + i, wbxml->strTable + j);
                i += stringLen(wbxml->strTable + j);
#ifdef WBXML_APPEND_VAR_BYTES
                temp[i++] = END;
#else
                stringCopy(temp + i, ")");
                i++;
#endif
                temp[i] = '\0';

                if (wbxml->handlers.textNew)
                    wbxml->handlers.textNew(wbxml, wbxml->userContext, temp);

                wbxml->offset--;
                break;
            case EXT_0:
            case EXT_1:
            case EXT_2:
            case OPAQUE:
                break;
            case PI:
                break;
            case STR_T:
                wbxml->offset++;
                
                j = wapReadInt(wbxml->data, &(wbxml->offset));
                stringCopy(temp, wbxml->strTable+j);

                if (wbxml->handlers.textNew)
                    wbxml->handlers.textNew(wbxml, wbxml->userContext, temp);
                wbxml->offset--;
                break;
            case LITERAL:
            case LITERAL_A:
            case LITERAL_C:
            case LITERAL_AC:
            default:
                wbxmlReadElement(wbxml);
                break;
        }
        wbxml->offset++;
    }

wbxmlParse_cleanup:
    wbxmlFreeStack(wbxml);
    memMgrChunkFree(temp);

    if (wbxml->handlers.postparse)
        wbxml->handlers.postparse(wbxml, wbxml->userContext);

    return 0;
}

int wbxmlFree(WBXMLParserPtr wbxml)
{
    if (wbxml->attrstr)
        memMgrChunkFree(wbxml->attrstr);
    if (wbxml->tmpstr)
        memMgrChunkFree(wbxml->tmpstr);
    if (wbxml->strTable)
        memMgrChunkFree(wbxml->strTable);
    if (wbxml->stack)
        memMgrChunkFree(wbxml->stack);

    memMgrChunkSet(wbxml, 0, sizeof(WBXMLParserType));

    return 0;
}

void wbxmlParserDestroy(WBXMLParserPtr *parser)
{
    if ((*parser)->handlers.destroy)
        (*parser)->handlers.destroy(*parser, (*parser)->userContext);
    
    wbxmlFree(*parser);
    
    memMgrChunkFree(*parser);
    *parser = NULL;
}

