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
#include "DomComment.h"

DOM_COMMENT *domCommentNew(const DOMString comment)
{
    return domNodeNew(COMMENT_NODE, (DOMString)"#comment", comment);
}
