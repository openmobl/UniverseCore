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

#include "DomNode.h"
#include "DomNodeList.h"

DOM_NODE *domNodeNew(unsigned long type, const DOMString name, const DOMString value)
{
    DOM_NODE *node = (DOM_NODE *)memMgrChunkNew(sizeof(DOM_NODE));

    memMgrChunkSet(node, 0, sizeof(DOM_NODE));

    node->type  = type;

    if (name)
        node->name  = (DOMString)stringDup(name);
    if (value)
        node->value = (DOMString)stringDup(value);

    return node;
}

void domNodeDestroySpecific(DOM_NODE *node)
{
    domNodeDestroy(node->attributes);

    if (node->name)
        memMgrChunkFree(node->name);
    if (node->value)
        memMgrChunkFree(node->value);

    domNodeRemoveChild(node->parent, node);

    memMgrChunkFree(node);
}

void domNodeDestroy(DOM_NODE *node)
{
    DOM_NODE *curr, *next;

    if (!node)
        return;

    curr = node->firstChild;
/*
// The ifdefing is to reduce some recursion on the PalmOS.
// The recursion is fatal with large DOM trees.
#ifdef PALMOS

#else*/
    while (curr)
    {
        next = curr->nextSibling;

        domNodeDestroy(curr);

        curr = next;
    }

    if (!node->parent)
    {
        for (curr = node->nextSibling;
                curr;
                curr = curr->nextSibling)
            domNodeDestroy(curr);
    }
//#endif

    domNodeDestroySpecific(node);
}

void domNodeAppendChild(DOM_NODE *parent, DOM_NODE *child)
{
    if (!parent || !child)
        return;

    if (!parent->firstChild)
        parent->firstChild = child;

    if (parent->lastChild)
    {
        parent->lastChild->nextSibling = child;
        child->prevSibling             = parent->lastChild;
    }
    else
        child->prevSibling = NULL;
    
    parent->lastChild  = child;
    child->parent      = parent;
    child->nextSibling = NULL;
}

void domNodeAppendChildAfter(DOM_NODE *parent, DOM_NODE *after, DOM_NODE *child)
{
    DOM_NODE *temp = DOM_NULL;
    
    if (!parent || !child || !after)
        return;

    if (after->parent != parent)
        return;

    temp = after->nextSibling;
    after->nextSibling = child;
    child->prevSibling = after;
    if (temp)
    {
        after->nextSibling->prevSibling = child;
        child->nextSibling = temp;
    }
    else
    {
        child->nextSibling = NULL;
        parent->lastChild = after;
    }
    
    child->parent = parent;

    /*if (!parent->firstChild)
        parent->firstChild = child;

    if (parent->lastChild)
    {
        parent->lastChild->nextSibling = child;
        child->prevSibling             = parent->lastChild;
    }
    else
        child->prevSibling = NULL;
    
    parent->lastChild  = child;
    child->parent      = parent;
    child->nextSibling = NULL;*/
}

void domNodeAppendChildAsFirst(DOM_NODE *parent, DOM_NODE *child)
{
    DOM_NODE *first = DOM_NULL;
    
    if (!parent || !child)
        return;
    
    first = parent->firstChild;
    if (first)
    {
        child->prevSibling = NULL;
        child->nextSibling = first;
    }
    else
    {
        child->prevSibling = NULL;
        child->nextSibling = NULL;
        parent->lastChild = child;
    }
    
    parent->firstChild = child;
    child->parent = parent;

    /*if (!parent->firstChild)
        parent->firstChild = child;

    if (parent->lastChild)
    {
        parent->lastChild->nextSibling = child;
        child->prevSibling             = parent->lastChild;
    }
    else
        child->prevSibling = NULL;
    
    parent->lastChild  = child;
    child->parent      = parent;
    child->nextSibling = NULL;*/
}

void domNodeRemoveChild(DOM_NODE *parent, DOM_NODE *child)
{
    DOM_NODE *curr, *prev = NULL;

    if (!child)
        return;

    if (parent)
    {
        for (curr = parent->firstChild, prev = NULL;
                curr;
                prev = curr, curr = curr->nextSibling)
        {
            if (curr == child)
                break;
        }
    }
    else
        curr = child;

    if (!curr)
        return;

    if (prev)
        prev->nextSibling  = curr->nextSibling;
    else if (parent)
        parent->firstChild = curr->nextSibling;
        
    if (curr->nextSibling)
        curr->nextSibling->prevSibling = prev;

    if ((parent) && (parent->lastChild == curr))
        parent->lastChild = (curr->nextSibling)?curr->nextSibling:prev;

    curr->parent      = NULL;
    curr->nextSibling = NULL;
    curr->prevSibling = NULL;
}

void domNodeAppendSibling(DOM_NODE *node, DOM_NODE *sibling)
{
    DOM_NODE *prev;

    if (!node || !sibling)
        return;

    if ((node->parent) && (prev = node->parent->lastChild))
    {
        prev->nextSibling    = sibling;
        sibling->prevSibling = prev;
    }
    else
    {
        DOM_NODE *curr;

        for (curr = node, prev = NULL;
                curr;
                prev = curr, curr = curr->nextSibling);

        if (!prev)
            prev = node;

        prev->nextSibling    = sibling;
        sibling->prevSibling = prev;
    }

    if (node->parent)
    {
        if (!node->parent->firstChild)
            node->parent->firstChild = sibling;

        node->parent->lastChild = sibling;
    }    

    sibling->parent      = node->parent;
    sibling->nextSibling = NULL;
}

DOM_NODE *domNodeGetParent(DOM_NODE *node)
{
    return (node)?node->parent:NULL;
}

DOM_NODE *domNodeGetFirstChild(DOM_NODE *node)
{
    return (node)?node->firstChild:NULL;
}

DOM_NODE *domNodeGetPreviousSibling(DOM_NODE *node)
{
    return (node)?node->prevSibling:NULL;
}

DOM_NODE *domNodeGetNextSibling(DOM_NODE *node)
{
    return (node)?node->nextSibling:NULL;
}

void domNodeSetName(DOM_NODE *node, const DOMString name)
{
    if ((!node) || (!name))
        return;

    if (node->name)
        memMgrChunkFree(node->name);

    node->name = (DOMString)stringDup(name);
}

/*const*/ DOMString domNodeGetName(DOM_NODE *node)
{
    return (node)?node->name:NULL;
}

void domNodeSetValue(DOM_NODE *node, const DOMString value)
{
    if ((!node) || (!value))
        return;

    if (node->value)
        memMgrChunkFree(node->value);

    node->value = (DOMString)stringDup(value);
}

void domNodeSetValueVariant(DOM_NODE *node, enum VariantType type, void *value, unsigned long valueSize)
{
    variantSet(type, value, valueSize, node, (void (*)(void *, const DOMString ))domNodeSetValue);
}

/*const*/ DOMString domNodeGetValue(DOM_NODE *node)
{
    return (node)?node->value:NULL;
}

void *domNodeGetValueVariant(DOM_NODE *node, enum VariantType type)
{
    return variantGet(type, domNodeGetValue(node));
}

DOM_NODE *domNodeFindNodeByName(DOM_NODE *node, const DOMString name)
{
    DOM_NODE *ret = NULL, *curr;

    if (!node || !name)
        return NULL;

    if ((node->name) && (!stringCompareCaseless(node->name, name)))
        return node;

    for (curr = node->firstChild;
            curr && !ret;
            curr = curr->nextSibling)
        ret = domNodeFindNodeByName(curr, name);

    if (!node->parent && !node->prevSibling)
    {
        for (curr = node->nextSibling;
                curr && !ret;
                curr = curr->nextSibling)
            ret = domNodeFindNodeByName(curr, name);
    }

    return ret;
}

DOM_NODE_LIST *domNodeFindNodesByName(DOM_NODE *node, const DOMString name)
{
    DOM_NODE_LIST *nodeList = domNodeListNew();

    domNodeFindNodesByName_r(nodeList, node, name);

    if (domNodeListGetNumEntries(nodeList) == 0)
    {
        domNodeListDestroy(nodeList);
        return NULL;
    }
    else
        return nodeList;
}

void domNodeFindNodesByName_r(DOM_NODE_LIST *nodeList, DOM_NODE *node, const DOMString name)
{
    DOM_NODE *curr;

    if (!node || !name)
        return;

    if ((node->name) && (!stringCompareCaseless(node->name, name)))
        domNodeListAddNode(nodeList, node);

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        domNodeFindNodesByName_r(nodeList, curr, name);

    if (!node->parent && !node->prevSibling)
    {
        for (curr = node->nextSibling;
                curr;
                curr = curr->nextSibling)
            domNodeFindNodesByName_r(nodeList, curr, name);
    }
}

void domNodeSerializeToFile(DOM_NODE *node, const char *fileName)
{
    FILE *fd = fileOpen(fileName, "w");

    if (fd)
    {
        domNodeSerializeToFd(node, fd);

        fileClose(fd);
    }
}

void domNodeSerializeToFd(DOM_NODE *node, FILE *fd)
{
    unsigned char elementClosed = 0;
    DOM_NODE *curr;

    if (!node)
        return;

    switch (node->type)
    {
        case TEXT_NODE:
            if (node->value)
                filePrintF(fd, "%s", (char *)node->value);
            break;
        case ELEMENT_NODE:        
            if (node->escapeTags)
                filePrintF(fd, "&lt;%s", (char *)node->name);
            else    
                filePrintF(fd, "<%s", (char *)node->name);

            for (curr = node->attributes;
                    curr;
                    curr = curr->nextSibling)
            {
                filePrintF(fd, " %s", (char *)curr->name);

                if (curr->value && *curr->value)
                    filePrintF(fd, "=\"%s\"", (char *)curr->value);
            }

            if (!node->firstChild)
            {
                if (node->escapeTags)
                    filePrintF(fd, "/&gt;");
                else
                    filePrintF(fd, "/>");
                elementClosed = 1;
            }
            else
            {
                if (node->escapeTags)
                    filePrintF(fd, "&gt;");
                else
                    filePrintF(fd, ">");
            }
            
            break;
        case COMMENT_NODE:
            if (node->value)
                filePrintF(fd, "<!--%s-->", (char *)node->value);
        case CDATA_SECTION_NODE:
            if (node->value)
                filePrintF(fd, "<![CDATA[%s]]>", (char *)node->value);
            break;
    }

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        domNodeSerializeToFd(curr, fd);

    if ((node->type == ELEMENT_NODE) && (!elementClosed) && (!node->autoclose))
    {
        if (node->escapeTags)
            filePrintF(fd, "&lt;/%s&gt;", (char *)node->name);
        else
            filePrintF(fd, "</%s>", (char *)node->name);
    }
}

unsigned long domNodeSerializeToString(DOM_NODE *node, char **string)
{
    unsigned long stringLength = 1;
    
    if (string)
    {
        *string = (char *)memMgrChunkNew(stringLength);

        if (*string)
        {
            *string = 0;

            domNodeSerializeToString_r(node, string, &stringLength);
        }
    }

    return stringLength - 1;
}

void domNodeSerializeToString_r(DOM_NODE *node, char **outString, unsigned long *outStringLength)
{
    DOM_NODE *curr;
    unsigned char elementClosed = 0;
    unsigned long newLength;
    char *newString;

    if (!node)
        return;
        
    switch (node->type)
    {
        case TEXT_NODE:
            if (node->value && stringLen(node->value))
            {
                newLength = *outStringLength + stringLen(node->value);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "%s", (char *)node->value);

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
        case ELEMENT_NODE: 
            if (node->escapeTags)
            {
                newLength = *outStringLength + 4 + stringLen(node->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "&lt;%s", (char *)node->name);

                *outString       = newString;
                *outStringLength = newLength;
            }
            else    
            {
                newLength = *outStringLength + 1 + stringLen(node->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "<%s", (char *)node->name);

                *outString       = newString;
                *outStringLength = newLength;
            }

            for (curr = node->attributes;
                    curr;
                    curr = curr->nextSibling)
            {
                newLength = *outStringLength + 1 + stringLen(curr->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, " %s", (char *)curr->name);

                *outString       = newString;
                *outStringLength = newLength;

                if (curr->value && *curr->value)
                {
                    newLength = *outStringLength + 3 + stringLen(curr->value);
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, "=\"%s\"", (char *)curr->value);
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
            }

            if (!node->firstChild)
            {
                if (node->escapeTags)
                {
                    newLength = *outStringLength + 5;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, "/&gt;");
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
                else
                {
                    newLength = *outStringLength + 2;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);

                    stringPrintF(newString + *outStringLength - 1, "/>");
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }

                elementClosed = 1;
            }
            else
            {
                if (node->escapeTags)
                {
                    newLength = *outStringLength + 4;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, "&gt;");
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
                else
                {
                    newLength = *outStringLength + 1;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, ">");
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
            }
            
            break;
        case COMMENT_NODE:
            if (node->value && stringLen(node->value))
            {
                newLength = *outStringLength + 7 + stringLen(node->value);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "<!--%s-->", (char *)node->value);

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
        case CDATA_SECTION_NODE:
            if (node->value && stringLen(node->value))
            {
                newLength = *outStringLength + 12 + stringLen(node->value);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "<![CDATA[%s]]>", (char *)node->value);

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
    }

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        domNodeSerializeToString_r(curr, outString, outStringLength);

        if ((node->type == ELEMENT_NODE) && (!elementClosed) && (!node->autoclose))
        {
            if (node->escapeTags)
            {
                newLength = *outStringLength + 8 + stringLen(node->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "&lt;%s&gt;", (char *)node->name);

                *outString       = newString;
                *outStringLength = newLength;
            }
            else
            {
                newLength = *outStringLength + 3 + stringLen(node->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "</%s>", (char *)node->name);

                *outString       = newString;
                *outStringLength = newLength;
            }
        }
}

unsigned long domNodeSerializeTreeToString(DOM_NODE *node, char **string, int showValues, int safe)
{
    unsigned long stringLength = 1;

    if (string)
    {
        *string = (char *)memMgrChunkNew(stringLength);

        if (*string)
        {
            *string = 0;

            domNodeSerializeTreeToString_r(node, string, &stringLength, "", showValues, safe);
        }
    }

    return stringLength - 1;
}

void domNodeSerializeTreeToString_r(DOM_NODE *node, char **outString,
                                    unsigned long *outStringLength, char *indent, int showValues, int safe)
{
    DOM_NODE *curr;
    unsigned char elementClosed = 0;
    unsigned long newLength;
    char *newString = NULL;
    char *newindent = NULL;
    unsigned long newindentLen = 0;
    
    if (!node)
        return;
        
    // If we exceed the SAFETY_BUFFER_SIZE then return
    if (safe && (*outStringLength > SAFETY_BUFFER_MAX))
        return;

    newindent = memMgrChunkNew(stringLen(indent) + 3);
    if (!newindent)
    {
        return;
    }
    
    stringCopy(newindent, indent);
    newindentLen = stringLen(indent);
    // Some handling to keep from getting any recursion errors
    if (safe)
    {
        if ((newindentLen + 2) < 18)
        {
            stringCopy(newindent + newindentLen, "  ");
            newindentLen += 2;
        }
        else if ((newindentLen + 2) < 20)
        {
            stringCopy(newindent + newindentLen, ">|");
            newindentLen += 2;
        }
    }
    else
    {
        stringCopy(newindent + newindentLen, "  ");
        newindentLen += 2;
    }
    newindent[newindentLen] = '\0';

    switch (node->type)
    {
        case DOCUMENT_NODE:
            newLength = *outStringLength + stringLen(indent) + 10;
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "%s#document\n", indent);

            *outString       = newString;
            *outStringLength = newLength;
            break;
        /*case TEXT_NODE:
            if (node->value && stringLen(node->value))
            {
                if (showValues) {
                    newLength = *outStringLength + stringLen(node->value) + stringLen(indent) + 9;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);

                    stringPrintF(newString + *outStringLength - 1, "%s#text [%s]\n", indent, node->value);
                } else {
                    newLength = *outStringLength + stringLen(indent) + 6;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);

                    stringPrintF(newString + *outStringLength - 1, "%s#text\n", indent);
                }

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;*/
        case ELEMENT_NODE:
            newLength = *outStringLength + stringLen(node->name) + stringLen(indent);
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "%s%s", indent, (char *)node->name);

            *outString       = newString;
            *outStringLength = newLength;

            for (curr = node->attributes;
                    curr && showValues;
                    curr = curr->nextSibling)
            {
                newLength = *outStringLength + 2 + stringLen(curr->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, " %s=", (char *)curr->name);

                *outString       = newString;
                *outStringLength = newLength;

                if (curr->value && *curr->value)
                {
                    newLength = *outStringLength + stringLen(curr->value);
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, "%s", (char *)curr->value);
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
            }
            
            newLength = *outStringLength + 1;
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "\n");

            *outString       = newString;
            *outStringLength = newLength;

            if (!node->firstChild)
            {
                elementClosed = 1;
            }
            break;
        case COMMENT_NODE:
        case CDATA_SECTION_NODE:
        case TEXT_NODE:
            if (node->value && stringLen(node->value))
            {
                if (showValues) {
                    newLength = *outStringLength + stringLen(node->name) + 3 + stringLen(node->value) + stringLen(indent) + 1;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);

                    stringPrintF(newString + *outStringLength - 1, "%s%s [%s]\n", indent, (char *)node->name, (char *)node->value);
                } else {
                    newLength = *outStringLength + stringLen(node->name) + stringLen(indent) + 1;
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);

                    stringPrintF(newString + *outStringLength - 1, "%s%s\n", indent, (char *)node->name);
                }

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
        default:
            break;
    }

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        domNodeSerializeTreeToString_r(curr, outString, outStringLength, newindent, showValues, safe);
    
    memMgrChunkFree(newindent);
}


/*unsigned long domNodeSerializeTreeToString(DOM_NODE *node, char **string)
{
    unsigned long stringLength = 1;

    if (string)
    {
        *string = (char *)memMgrChunkNew(stringLength);

        if (*string)
        {
            *string = 0;

            domNodeSerializeTreeToString_r(node, string, &stringLength);
        }
    }

    return stringLength - 1;
}

void domNodeSerializeTreeToString_r(DOM_NODE *node, char **outString, unsigned long *outStringLength)
{
    DOM_NODE *curr;
    unsigned char elementClosed = 0;
    unsigned long newLength;
    char *newString;

    if (!node)
        return;

    switch (node->type)
    {
        case DOCUMENT_NODE:
            newLength = *outStringLength + 11;
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "#document\n\n");

            *outString       = newString;
            *outStringLength = newLength;
            break;
        case TEXT_NODE:
            if (node->value && stringLen(node->value))
            {
                newLength = *outStringLength + stringLen(node->value) + 9;
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "#text  %s\n\n", node->value);

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
        case ELEMENT_NODE:        
            newLength = *outStringLength + stringLen(node->name) + 1;
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "%s\n", node->name);

            *outString       = newString;
            *outStringLength = newLength;

            for (curr = node->attributes;
                    curr;
                    curr = curr->nextSibling)
            {
                newLength = *outStringLength + 7 + stringLen(curr->name);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, " attr: %s", curr->name);

                *outString       = newString;
                *outStringLength = newLength;

                if (curr->value && *curr->value)
                {
                    newLength = *outStringLength + 7 + stringLen(curr->value);
                    newString = (char *)memMgrChunkRealloc(*outString, newLength);
    
                    stringPrintF(newString + *outStringLength - 1, " val: %s\n", curr->value);
    
                    *outString       = newString;
                    *outStringLength = newLength;
                }
            }
            
            newLength = *outStringLength + 1;
            newString = (char *)memMgrChunkRealloc(*outString, newLength);

            stringPrintF(newString + *outStringLength - 1, "\n");

            *outString       = newString;
            *outStringLength = newLength;

            if (!node->firstChild)
            {
                elementClosed = 1;
            }
            break;
        case COMMENT_NODE:
            if (node->value && stringLen(node->value))
            {
                newLength = *outStringLength + 12 + stringLen(node->value);
                newString = (char *)memMgrChunkRealloc(*outString, newLength);

                stringPrintF(newString + *outStringLength - 1, "#comment  %s\n\n", node->value);

                *outString       = newString;
                *outStringLength = newLength;
            }
            break;
    }

    for (curr = node->firstChild;
            curr;
            curr = curr->nextSibling)
        domNodeSerializeTreeToString_r(curr, outString, outStringLength);

}*/

