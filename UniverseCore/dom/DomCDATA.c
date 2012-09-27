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
#include "DomCDATA.h"

DOM_CDATA *domCDATANew(const DOMString data)
{
    return domNodeNew(CDATA_SECTION_NODE, (DOMString)"#cdata-section", data);
}
