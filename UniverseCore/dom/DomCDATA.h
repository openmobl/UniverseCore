/*
 * libsgml -- SGML state machine parsing library.
 *                                                                  
 * Copyright (c) 2002 Uninformed Research (http://www.uninformed.org)
 * All rights reserved.
 *
 * skape
 * mmiller@hick.org
 */
/**
 * @defgroup DOMText Text
 * @ingroup DOMNode
 *
 * DOM Text manipulation.
 *
 * @{
 */
#ifndef _LIBSGML_DOMCDATA_H
#define _LIBSGML_DOMCDATA_H

#include "DomNode.h"

/**
 * @def DOM_CDATA
 *
 * Defines DOM_CDATA to DOM_NODE.  This gives us "inheritance" in C.
 */
#define DOM_CDATA DOM_NODE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Constructs a new DOM CDATA node with the provided text.
 *
 * @param  cdata [in] The null terminated string to set the CDATA node to.
 * @return On success, an initialized DOM text node will be returned.  Otherwise, NULL will be returned.
 */
DOM_CDATA *domCDATANew(const DOMString data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
