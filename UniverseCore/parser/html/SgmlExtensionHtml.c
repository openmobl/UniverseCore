/*
 * libsgml -- SGML state machine parsing library.
 *                                                                  
 * Copyright (c) 2002 Uninformed Research (http://www.uninformed.org)
 * All rights reserved.
 *
 * skape
 * mmiller@hick.org
 */
#include "config.h"

#include "SgmlParser.h"
#include "SgmlExtensionHtml.h"

void sgmlExtensionHtmlCleanupBranch(DOM_NODE *node, HTML_CLEANUP *cleanup);
DOM_NODE *sgmlExtensionHtmlCleanupNextElement(DOM_NODE *node);

void sgmlExtensionOnDivert(struct _sgml_parser *parser, unsigned long newIndex, unsigned long oldState,
                                unsigned long newState, const DOMString lastBuffer, unsigned long lastBufferSize);

  
const DOMString autocloseElements[] = {
    (DOMString)"br",
    (DOMString)"dd",
    (DOMString)"hr",
    (DOMString)"img",
    (DOMString)"input",
    (DOMString)"li",
    (DOMString)"link",
    (DOMString)"meta",
    /*(DOMString)"p",*/ // This is some weird Voo Doo stuff
    (DOMString)"wbr",
    NULL
};

const DOMString passthroughTags[] = {
    (DOMString)"script",
    NULL
};

void sgmlExtensionOnDivert(struct _sgml_parser *parser, unsigned long newIndex, unsigned long oldState,
                                unsigned long newState, const DOMString lastBuffer, unsigned long lastBufferSize)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)sgmlParserGetUserContext(parser);

    if ((oldState & SGML_PARSER_STATE_INELEMENT) && (newState & SGML_PARSER_STATE_INTEXT))
    {
        // If this element is autoclosing, we go up.

        if (oldState & SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL)
        {
            sgmlExtensionHtmlAttributeNew(parser, sgmlParserGetUserContext(parser), lastBuffer, (DOMString)"");
            ext->skipNextAttribute = 1;
        }
         if (oldState & SGML_PARSER_STATE_INATTRIBUTEVALUE_S)
        {
            sgmlExtensionHtmlAttributeNew(parser, sgmlParserGetUserContext(parser), parser->internal.lastAttributeName, lastBuffer);
            ext->skipNextAttribute = 1;
        }

        if ((ext->currElement) && (ext->currElement->autoclose))
            ext->currElement = (ext->currElement)?ext->currElement->parent:NULL;
    
        while ((ext->currElement) && (ext->currElement->deferredClosure))
            ext->currElement = ext->currElement->parent;
    }

    ext->saveDivert(parser, newIndex, oldState, newState, lastBuffer, lastBufferSize);
}

SGML_EXTENSION_HTML *sgmlExtensionHtmlNew(SGML_PARSER *parser)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)memMgrChunkNew(sizeof(SGML_EXTENSION_HTML));
    SGML_HANDLERS handlers;

    memMgrChunkSet(&handlers, 0, sizeof(SGML_HANDLERS));

    handlers.preparse     = sgmlExtensionHtmlInitialize;
    handlers.elementBegin = sgmlExtensionHtmlElementBegin;
    handlers.elementEnd   = sgmlExtensionHtmlElementEnd;
    handlers.attributeNew = sgmlExtensionHtmlAttributeNew;
    handlers.textNew      = sgmlExtensionHtmlTextNew;    
    handlers.commentNew   = sgmlExtensionHtmlCommentNew;
    handlers.cdataNew     = sgmlExtensionHtmlCDATANew;
    handlers.postparse    = sgmlExtensionHtmlDeinitialize;
    ext->flags            = 0;

    ext->document         = NULL;
    ext->currElement      = NULL;

    memMgrChunkMove(&parser->handlers, &handlers, sizeof(SGML_HANDLERS));

    parser->internal.userContext   = ext;

    ext->saveDivert                = parser->internal.onDivert;
    parser->internal.onDivert      = sgmlExtensionOnDivert;

    parser->internal.setExtensionParam = sgmlExtensionHtmlSetParam;
    parser->internal.getExtensionParam = sgmlExtensionHtmlGetParam;

    sgmlParserAddPassthroughTags(parser, (DOMString *)passthroughTags);

    return ext;
}

void sgmlExtensionHtmlDestroy(SGML_PARSER *parser, SGML_EXTENSION_HTML *ext)
{
    domDocumentDestroy(ext->document);

    memMgrChunkFree(ext);

    sgmlParserGetUserContext(parser) = NULL;
}

void sgmlExtensionHtmlInitialize(SGML_PARSER *parser, void *userContext)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;

    if (ext->document)
        domDocumentDestroy(ext->document);

    ext->currElement       = NULL;
    ext->document          = domDocumentNew();
    ext->skipNextAttribute = 0;
}

void sgmlExtensionHtmlElementBegin(SGML_PARSER *parser, void *userContext, const DOMString elementName)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;
    DOM_ELEMENT *element   = domElementNew(elementName);
    int x = 0, match = 0;

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPELEMENT)
        return;

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_ESCAPEUNKNOWNTAGS && ext->knownTags)
    {
        for (match = 0, x = 0; !match && ext->knownTags[x]; x++)
        {
            if (!stringCompareCaseless(ext->knownTags[x], elementName))
                match = 1;
        }

        if (!match)
            element->escapeTags = 1;
    }    

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, element);
    else
        domNodeAppendChild(ext->document, element);

    for (match = 0, x = 0; !match && autocloseElements[x]; x++) {
        if (!stringCompareCaseless(autocloseElements[x], elementName)) {
            match = 1;
            break;
        }
    }

    if (match) {
        element->autoclose = 1;
    }

    ext->currElement = element;
}

void sgmlExtensionHtmlElementEnd(SGML_PARSER *parser, void *userContext, const DOMString elementName)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPELEMENT)
        return;
        
    if ((!stringCompareCaseless((DOMString)"html", ext->currElement->name) || !stringCompareCaseless((DOMString)"body", ext->currElement->name)) &&
        stringCompareCaseless(ext->currElement->name, elementName))
        ext->currElement = (ext->currElement)?ext->currElement->parent:NULL;
    
    if ((!stringCompareCaseless((DOMString)"html", elementName) || !stringCompareCaseless((DOMString)"body", elementName)) &&
        stringCompareCaseless(ext->currElement->name, elementName))
        return;

    /*
     * Ignore closure of element that isn't parent.
     */
     /*
      * TODO: Treat </p> like <p></p>, WinIE, Safari and Firefox do this.
      *       Also treat </br> as <br>
      */
    if ((ext->currElement) && (stringCompareCaseless(ext->currElement->name, elementName)))
    {
        DOM_ELEMENT *curr = ext->currElement;

        for (curr = ext->currElement;
                curr;
                curr = curr->parent)
        {
            if (!stringCompareCaseless(curr->name, elementName))
            {
                curr->deferredClosure = 1;
                curr->autoclose       = 0;
                break;
            }
        }

        return;
    }

    ext->currElement = (ext->currElement)?ext->currElement->parent:NULL;    

    while ((ext->currElement) && (ext->currElement->deferredClosure))
        ext->currElement = ext->currElement->parent;
}

void sgmlExtensionHtmlAttributeNew(SGML_PARSER *parser, void *userContext, const DOMString attributeName, const DOMString attributeValue)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPELEMENT)
        return;

    if (ext->skipNextAttribute)
    {
        ext->skipNextAttribute = 0;
        return;
    }

    domElementSetAttribute(ext->currElement, attributeName, attributeValue);
}

void sgmlExtensionHtmlTextNew(SGML_PARSER *parser, void *userContext, const DOMString textString)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;
    DOM_TEXT *text          = domTextNew(textString);

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPTEXT)
        return;

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_IGNOREBLANKLINES)
    {
        int x = 0, ok = 0, len = stringLen(textString);

        for (x = 0; !ok && x < len; x++)
        {
            switch (textString[x])
            {
                case '\n': case '\r': case '\t': case ' ':
                    break;
                default:
                    ok = 1;
                    break;
            }
        }    

        if (!ok)
            return;
    }

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, text);
    else
        domNodeAppendChild(ext->document, text);
}

void sgmlExtensionHtmlCommentNew(SGML_PARSER *parser, void *userContext, const DOMString commentString)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;
    DOM_COMMENT *comment    = domCommentNew(commentString);

    if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPCOMMENT)
        return;

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, comment);
    else
        domNodeAppendChild(ext->document, comment);
}

void sgmlExtensionHtmlCDATANew(SGML_PARSER *parser, void *userContext, const DOMString data)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)userContext;
    DOM_CDATA *cdata    = domCDATANew(data);

    //if (ext->flags & SGML_EXTENSION_HTML_FLAG_STRIPCDATA)
    //    return;

    if (ext->currElement)
        domNodeAppendChild(ext->currElement, cdata);
    else
        domNodeAppendChild(ext->document, cdata);
}

void sgmlExtensionHtmlDeinitialize(SGML_PARSER *parser, void *userContext)
{
}

DOM_DOCUMENT *sgmlExtensionHtmlGetDocument(SGML_PARSER *parser)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)sgmlParserGetUserContext(parser);

    return ext->document;
}

void sgmlExtensionHtmlSetParam(SGML_PARSER *parser, void *extensionContext, unsigned long param, void *value)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)sgmlParserGetUserContext(parser);


    switch (param)
    {
        case SGML_EXTENSION_HTML_PARAM_FLAG:
            ext->flags = (unsigned long)value;            
            break;
        case SGML_EXTENSION_HTML_PARAM_SETKNOWNTAGS:
            ext->knownTags = (DOMString *)value;
            break;
    }
}

void sgmlExtensionHtmlGetParam(SGML_PARSER *parser, void *extensionContext, unsigned long param, void *value)
{
    SGML_EXTENSION_HTML *ext = (SGML_EXTENSION_HTML *)sgmlParserGetUserContext(parser);

    switch (param)
    {
        case SGML_EXTENSION_HTML_PARAM_FLAG:
            *(unsigned long *)value = ext->flags;
            break;
        case SGML_EXTENSION_HTML_PARAM_SETKNOWNTAGS:
            *(DOMString **)value = ext->knownTags;
            break;
    }
}

DOM_NODE *sgmlExtensionHtmlCleanupNextElement(DOM_NODE *node)
{
    DOM_NODE *curr = domNodeGetNextSibling(node);
    
    if (!curr)
        return NULL;
    
    while (curr && (curr->type != ELEMENT_NODE))
    {
        curr = domNodeGetNextSibling(curr);
    }
    
    return curr;
}

void sgmlExtensionHtmlCleanupBranch(DOM_NODE *node, HTML_CLEANUP *cleanup)
{
    DOM_NODE *curr;

    if (!node)
        return;

    switch (node->type)
    {
        case TEXT_NODE:
            break;
        case ELEMENT_NODE:
            {
                if (!domNodeGetName(node))
                    return;
                
                /* HTML and BODY get special cases */
                if (!stringCompareCaseless(domNodeGetName(node), (DOMString)"html"))
                {
                    cleanup->gotHtmlElem = DOM_TRUE;
                    
                    if (stringCompareCaseless(domNodeGetName(domNodeGetParent(node)), (DOMString)"#document"))
                    {
                        DOM_NODE *dest = node;
                        
                        while (dest && stringCompareCaseless(domNodeGetName(dest), (DOMString)"#document"))
                        {
                            dest = domNodeGetParent(dest);
                        }
                        
                        if (dest)
                        {
                            DOM_NODE *parent = domNodeGetParent(node);
                            DOM_NODE *child = domNodeGetFirstChild(node);
                            DOM_NODE *temp = node;
                            DOM_NODE *append = NULL;
                            
                            while (child)
                            {
                                domNodeRemoveChild(node, child);
                                domNodeAppendChildAfter(parent, temp, child);
                                
                                temp = child;
                                child = domNodeGetFirstChild(node);
                            }
                            
                            domNodeRemoveChild(parent, node);
                            domNodeAppendChild(dest, node);
                            
                            for (append = domNodeGetFirstChild(dest);
                                 append;
                                 append = domNodeGetFirstChild(dest))
                            {
                                if (append == node) // element should be the last node
                                    break;
                
                                domNodeRemoveChild(dest, append);
                                domNodeAppendChild(node, append);
                            }
                        }
                        cleanup->topInElem++;
                    }
                }
                else if (!stringCompareCaseless(domNodeGetName(node), (DOMString)"body"))
                {
                    cleanup->gotBodyElem = DOM_TRUE;
                    
                    if (//cleanup->gotHtmlElem &&
                        stringCompareCaseless(domNodeGetName(domNodeGetParent(node)), (DOMString)"html"))
                    {
                        DOM_NODE *dest = node;
                        
                        while (dest && stringCompareCaseless(domNodeGetName(dest), (DOMString)"html") &&
                                       stringCompareCaseless(domNodeGetName(dest), (DOMString)"#document"))
                        {
                            dest = domNodeGetParent(dest);
                        }
                        
                        if (dest)
                        {
                            DOM_NODE *parent = domNodeGetParent(node);
                            DOM_NODE *child = domNodeGetFirstChild(node);
                            DOM_NODE *temp = node;
                            DOM_NODE *append = NULL;
                            
                            while (child)
                            {
                                domNodeRemoveChild(node, child);
                                domNodeAppendChildAfter(parent, temp, child);
                                
                                temp = child;
                                child = domNodeGetFirstChild(node);
                            }
                            
                            domNodeRemoveChild(parent, node);
                            domNodeAppendChild(dest, node);
                            
                            for (append = domNodeGetFirstChild(dest);
                                 append;
                                 append = domNodeGetFirstChild(dest))
                            {
                                if (!stringCompareCaseless(domNodeGetName(append), (DOMString)"head"))
                                    continue;
                                if (append == node) // element should be the last node
                                    break;
                
                                domNodeRemoveChild(dest, append);
                                domNodeAppendChild(node, append);
                            }

                        }

                        cleanup->topInElem++;
                    }
                }
                /* A head element is not required, but we need to keep track of it. */
                else if (!stringCompareCaseless(domNodeGetName(node), (DOMString)"head"))
                {
                    cleanup->gotHeadElem = DOM_TRUE;
                }
                
                /* We will need to move these elements down if we find a body,
                 * Or create the body before these elements.
                 */
                if (!cleanup->gotBodyElem && !cleanup->gotHeadElem &&
                    stringCompareCaseless(domNodeGetName(node), (DOMString)"html"))
                {
                    cleanup->elemBeforeBody++;
                }
            }
            break;
        case COMMENT_NODE:
            break;
    }

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        sgmlExtensionHtmlCleanupBranch(curr, cleanup);
    
}

void sgmlExtensionHtmlCleanup(DOM_NODE *node, int mode)
{
    DOM_NODE *curr;
    HTML_CLEANUP cleanup;
    //DOM_NODE *save = node;

    if (!node || (node->type != DOCUMENT_NODE))
        return;
    
    curr = domNodeGetFirstChild(node);
    
    if (!curr)
        return;
        
    memMgrChunkSet(&cleanup, 0, sizeof(HTML_CLEANUP));
    cleanup.mode = mode;
    
    do
    {
        sgmlExtensionHtmlCleanupBranch(curr, &cleanup);
        
        cleanup.gotHeadElem = DOM_FALSE;
    } while ((curr = sgmlExtensionHtmlCleanupNextElement(curr)));
    
    if (!cleanup.gotBodyElem)
    {
        DOM_ELEMENT *element = domElementNew((DOMString)"body");
        DOM_NODE *append;
        DOM_NODE *save = node;
        
        if (element)
        {
            if (cleanup.gotHtmlElem)
            {
                DOM_NODE *next = domNodeGetFirstChild(node);
                
                //while (next && (next->type != ELEMENT_NODE))
                //    next = sgmlExtensionHtmlCleanupNextElement(next);
                //
                //if (domNodeGetName(next) &&
                //    !stringCompareCaseless(domNodeGetName(next), (DOMString)"html", 4))
                //{
                //    WinDrawChars("ptr", StrLen("ptr"), 0, 40);
                //    node = next;
                //}
                
                while (next && domNodeGetName(next) &&
                       stringCompareCaseless(domNodeGetName(next), (DOMString)"html"))
                {
                    next = domNodeGetNextSibling(next);
                }
                
                if (next)
                    node = next;
            }
            
            domNodeAppendChild(node, element);
            
            for (append = domNodeGetFirstChild(node);
                 append;
                 append = domNodeGetFirstChild(node))
            {
                if (!stringCompareCaseless(domNodeGetName(append), (DOMString)"head"))
                    continue;
                if (append == element) // element should be the last node
                    break;
                
                domNodeRemoveChild(node, append);
                domNodeAppendChild(element, append);
            }
        }
        
        node = save;
    }
    if (!cleanup.gotHtmlElem)
    {
        DOM_ELEMENT *element = domElementNew((DOMString)"html");
        DOM_NODE *append;
        DOM_NODE *save = node;
        
        if (element)
        {
            domNodeAppendChild(node, element);
            
            for (append = domNodeGetFirstChild(node);
                 append;
                 append = domNodeGetFirstChild(node))
            {
                if (append == element)
                    break;

                domNodeRemoveChild(node, append);
                domNodeAppendChild(element, append);
            }
        }
        
        node = save;
    }
    if (cleanup.topInElem)
    {
        //WinDrawChars("topInElem", 9, 0, 10);
    }
    if (cleanup.elemBeforeBody)
    {
        //WinDrawChars("elemBeforeBody!", 15, 0, 0);
    }
    
}

