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
 
#ifndef WMLC_h
#define WMLC_h

#include "config.h"

#include "DomDocument.h"
#include "DomElement.h"
#include "DomText.h"
#include "DomComment.h"
#include "WBXML.h"

typedef struct WMLCExtContextType {
    DOM_DOCUMENT *document;
    DOM_ELEMENT  *currElement;
} WMLCExtContextType;

typedef WMLCExtContextType *WMLCExtContextPtr;

////
// WML TAG Tokens, according to WAP WML 16-Jun-1999 V1.1 (SPEC-WML-19990616.PDF Ch.14.3.2)
//  
#define WML_a                       0x1C
#define WML_anchor                  0x22
#define WML_access                  0x23
#define WML_b                       0x24
#define WML_big                     0x25
#define WML_br                      0x26
#define WML_card                    0x27
#define WML_do                      0x28
#define WML_em                      0x29
#define WML_fieldset                0x2A
#define WML_go                      0x2B
#define WML_head                    0x2C
#define WML_i                       0x2D
#define WML_img                     0x2E
#define WML_input                   0x2F
#define WML_meta                    0x30
#define WML_noop                    0x31
#define WML_p                       0x20
#define WML_postfield               0x21
#define WML_prev                    0x32
#define WML_onevent                 0x33
#define WML_optgroup                0x34
#define WML_option                  0x35
#define WML_refresh                 0x36
#define WML_select                  0x37
#define WML_setvar                  0x3E
#define WML_small                   0x38
#define WML_strong                  0x39
#define WML_pre                     0x1B
#define WML_table                   0x1F
#define WML_td                      0x1D
#define WML_template                0x3B
#define WML_timer                   0x3C
#define WML_tr                      0x1E
#define WML_u                       0x3d
#define WML_wml                     0x3F

//
// WML Attribute Start Tokens, according to WAP WML 16-Jun-1999 V1.1 (SPEC-WML-19990616.PDF Ch.14.3.3)
//  
#define WML_accept_charset          0x05
#define WML_align                   0x52
#define WML_align_bottom            0x06
#define WML_align_center            0x07
#define WML_align_left              0x08
#define WML_align_middle            0x09
#define WML_align_right             0x0A
#define WML_align_top               0x0B
#define WML_alt                     0x0C
#define WML_accesskey               0x5E
#define WML_class                   0x54
#define WML_columns                 0x53
#define WML_content                 0x0D
#define WML_content_wmlc            0x5C
#define WML_domain                  0x0F
#define WML_emptyok_false           0x10
#define WML_emptyok_true            0x11
#define WML_format                  0x12
#define WML_forua_false             0x56
#define WML_forua_true              0x57
#define WML_height                  0x13
#define WML_href                    0x4A
#define WML_href_http               0x4B	
#define WML_href_https              0x4C
#define WML_hspace                  0x14
#define WML_http_equiv              0x5A
#define WML_http_equiv_content	    0x5B
#define WML_http_equiv_expires	    0x5D
#define WML_id                      0x55
#define WML_ivalue                  0x15
#define WML_iname                   0x16
#define WML_label                   0x18
#define WML_localsrc                0x19
#define WML_maxlength               0x1C
#define WML_method_get              0x1B
#define WML_method_post             0x1C
#define WML_mode_nowrap             0x1D
#define WML_mode_wrap               0x1E
#define WML_multiple_false          0x1F
#define WML_multiple_true           0x20
#define WML_name                    0x21
#define WML_newcontect_false	    0x22
#define WML_newcontext_true         0x23
#define WML_onenterbackward         0x25
#define WML_onenterforward          0x26
#define WML_onpick                  0x24
#define WML_ontimer                 0x27
#define WML_optional_false          0x28
#define WML_optional_true           0x29
#define WML_path                    0x2A
#define WML_scheme                  0x2E
#define WML_sendreferer_false	    0x2F
#define WML_sendreferer_true	    0x30
#define WML_size                    0x31
#define WML_src                     0x32
#define WML_src_http                0x58
#define WML_src_https               0x59
#define WML_ordered_true            0x33
#define WML_ordered_false           0x34
#define WML_tabindex                0x35
#define WML_title                   0x36
#define WML_type                    0x37
#define WML_type_accept             0x38
#define WML_type_delete             0x39
#define WML_type_help               0x3A
#define WML_type_password           0x3B
#define WML_type_onpick             0x3C
#define WML_type_onenterback	    0x3D
#define WML_type_onenterforward	    0x3E
#define WML_type_ontimer            0x3F
#define WML_type_options            0x45
#define WML_type_prev               0x46
#define WML_type_reset              0x47
#define WML_type_text               0x48
#define WML_type_vnd                0x49
#define WML_value                   0x4D
#define WML_vspace                  0x4E
#define WML_width                   0x4F
#define WML_xml_lang                0x50


//
// WML Attribute Value Tokens, according to WAP WML 16-Jun-1999 V1.1 (SPEC-WML-19990616.PDF Ch.14.3.4)
//  
#define WML_com                     0x85
#define WML_edu                     0x86
#define WML_net                     0x87
#define WML_org                     0x88
#define WML_accept                  0x89
#define WML_bottom                  0x8A
#define WML_clear                   0x8B
#define WML_delete                  0x8C
#define WML_help                    0x8D
#define WML_http                    0x8E
#define WML_http_www		        0x8F
#define WML_https                   0x90
#define WML_https_www		        0x91
#define WML_middle                  0x93
#define WML_nowrap                  0x94
#define WML_val_onenterbackward     0x96
#define WML_val_onenterforward      0x97
#define WML_val_onpick		        0x95
#define WML_val_ontimer		        0x98
#define WML_options                 0x99
#define WML_password		        0x9A
#define WML_reset                   0x9B
#define WML_text                    0x9D
#define WML_top                     0x9E
#define WML_unknown                 0x9F
#define WML_wrap                    0xA0
#define WML_www                     0xA1


#ifdef __cplusplus
extern "C" {
#endif

extern int wbxmlExtensionWMLCNew(WBXMLParserPtr parser);
extern void wbxmlExtensionWMLCDestroy(WBXMLParserPtr parser, void *userContext);
extern void wbxmlExtensionWMLCInitialize(WBXMLParserPtr parser, void *userContext);
extern void wbxmlExtensionWMLCElementBegin(WBXMLParserPtr parser, void *userContext, const char *elementName);
extern void wbxmlExtensionWMLCElementEnd(WBXMLParserPtr parser, void *userContext, const char *elementName);
extern void wbxmlExtensionWMLCAttributeNew(WBXMLParserPtr parser, void *userContext,
                                            const char *attributeName, const char *attributeValue);
extern void wbxmlExtensionWMLCTextNew(WBXMLParserPtr parser, void *userContext, const char *textValue);
extern void wbxmlExtensionWMLCCommentNew(WBXMLParserPtr parser, void *userContext, const char *comment);
extern void wbxmlExtensionWMLCDeinitialize(WBXMLParserPtr parser, void *userContext);
extern DOMDocumentType *wbxmlExtensionWMLCGetDocument(WBXMLParserPtr parser);

#ifdef __cplusplus
}
#endif

#endif
