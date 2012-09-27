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
#include "DomText.h"

DOM_TEXT *domTextNew(const DOMString text)
{
    return domNodeNew(TEXT_NODE, (DOMString)"#text", text);
}
