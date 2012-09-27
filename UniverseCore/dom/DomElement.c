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
#include "DomElement.h"

struct _element_attribute_setter {

    DOM_ELEMENT     *element;
    /*const*/ DOMString name;

};

void _domElementSetter(struct _element_attribute_setter *set, const DOMString string);


void _domElementSetter(struct _element_attribute_setter *set, const DOMString string)
{
    domElementSetAttribute(set->element, set->name, string);
}

DOM_ELEMENT *domElementNew(const DOMString elementName)
{
    return domNodeNew(ELEMENT_NODE, elementName, NULL);
}

void domElementDestroy(DOM_ELEMENT *element)
{
    domNodeDestroy(element);
}

void domElementSetAttribute(DOM_ELEMENT *element, const DOMString name, const DOMString value)
{
    DOM_NODE *curr, *attr = NULL;

    if ((!element) || (!name) || (!value))
        return;

    curr = domNodeFindNodeByName(element->attributes, name);

    if (!curr)
        attr = domNodeNew(ATTRIBUTE_NODE, name, value);
    else
        attr = curr;

    if (!element->attributes)
        element->attributes = attr;
    else
        domNodeAppendSibling(element->attributes, attr);

    domNodeSetValue(attr, value);    
}

void domElementSetAttributeVariant(DOM_ELEMENT *element, const DOMString name, enum VariantType type, void *value, unsigned long valueSize)
{
    struct _element_attribute_setter set;

    set.element = element;
    set.name    = name;

    variantSet(type, value, valueSize, &set, (void (*)(void *, const DOMString ))_domElementSetter);
}

/*const*/ DOMString domElementGetAttribute(DOM_ELEMENT *element, const DOMString name)
{
    DOM_NODE *attr;

    if ((!element) || (!name))
        return NULL;

    attr = domNodeFindNodeByName(element->attributes, name);

    return (attr)?attr->value:NULL;
}

void *domElementGetAttributeVariant(DOM_ELEMENT *element, const DOMString name, enum VariantType type)
{
    return variantGet(type, domElementGetAttribute(element, name));
}

void domElementUnsetAttribute(DOM_ELEMENT *element, const DOMString name)
{
    DOM_NODE *attr;

    if ((!element) || (!name))
        return;

    attr = domNodeFindNodeByName(element->attributes, name);

    if (attr)
    {
        domNodeRemoveChild(NULL, attr);
        domNodeDestroy(attr);
    }
}

/*const*/ DOMString domElementEnumAttributes(DOM_ELEMENT *element, unsigned long index)
{
    DOM_NODE *curr;
    unsigned long cIndex = 0;

    for (curr = element->attributes, cIndex = 0;
            cIndex != index && curr;
            cIndex++, curr = curr->nextSibling);

    if (curr)
        return curr->name;

    return NULL;
}
