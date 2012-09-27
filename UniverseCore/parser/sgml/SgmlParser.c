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

#include "International.h"
#include "SgmlParser.h"
#include "SgmlExtensionHtml.h"
#include "SgmlExtensionXml.h"

#define SGML_PARSER_STATEINDEX_INTEXT                 0x0000
#define SGML_PARSER_STATEINDEX_INELEMENTNAME          0x0001
#define SGML_PARSER_STATEINDEX_INATTRIBUTENAME        0x0002
#define SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS    0x0003
#define SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S     0x0004
#define SGML_PARSER_STATEINDEX_INCOMMENT              0x0005
#define SGML_PARSER_STATEINDEX_INCDATA                0x0006
#define SGML_PARSER_STATEINDEX_INENTITY               0x0007
#define SGML_PARSER_STATEINDEX_INPI                   0x0008
#define SGML_PARSER_STATEINDEX_INDOCTYPE              0x0009


#ifdef PALMOS

/*
 * All the possible logical states for an SGML document.
 */
const SGML_STATE_TABLE sgmlStateTable[] = {
    {    // Text
        SGML_PARSER_STATEINDEX_INTEXT,
        NULL,
        0
    },
    {    // ElementName
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        NULL,
        0
    },
    {    // AttributeName
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        NULL,
        0
    },
    {    // AttributeValue_NoSpaces
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        NULL,
        0
    },
    {    // AttributeValue_Spaces
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        NULL,
        0
    },
    {    // Comment
        SGML_PARSER_STATEINDEX_INCOMMENT,
        NULL,
        0
    },
    {    // CDATA
        SGML_PARSER_STATEINDEX_INCDATA,
        NULL,
        0
    },
    /*{    // DOCTYPE
        SGML_PARSER_STATEINDEX_INDOCTYPE,
        NULL,
        0
    },
    {    // Entity
        SGML_PARSER_STATEINDEX_INENTITY,
        NULL,
        0
    },
    {    // PI
        SGML_PARSER_STATEINDEX_INPI,
        NULL,
        0
    },*/

};

#define cdataStr        10
#define endCdataStr     20
#define endCommentStr   30

const SGML_STATE_TABLE_RULE sgmlStateTableRules[] = {
    /*
     * SGML_PARSER_STATEINDEX_INTEXT
     * Text mode rules
     *
     * Specific changes:
     *        <                    (divert SGML_PARSER_STATEINDEX_INELEMENTNAME)
     */
    {
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '<',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        0,
        0,
        SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTNAME,
        SGML_PARSER_STATE_INTEXT
    },
    /*
     * SGML_PARSER_STATEINDEX_INELEMENTNAME
     * ElementName mode rules
     *
     * Specific changes:
     *      /         (flag as closure element)
     *      >         (divert SGML_PARSER_STATEIDNEX_INTEXT)
     *      !         (if not actual, update state setting SGML_PARSER_STATE_INCOMMENTEXCLAMATION)
     *      [         (divert to CDATA)
     *      -         (if state EXCLA, update state setting SGML_PARSER_STATE_INCOMMENTDASH1)
     *      -         (if state DASH1, divert SGML_PARSER_STATEINDEX_INCOMMENT)
     *      not ' '   (if not actual, start with actual name of element)
     *      ' '       (if actual, divert SGML_PARSER_STATE_INATTRIBUTENAME)
     */
    {    // If we see a / in the name anywhere, flag this is as a closure element.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '/',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        0
    },
    {    // If we hit a close element tag, we've reached the end, divert back to text mode.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '!',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0,
        SGML_PARSER_STATE_INCOMMENT,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1 
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCDATA,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION
    },
    {    // If we hit another space after the name, now we divert to the attribute name section.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        0,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0
    },
    {    // Once we get our first non-space character we safely start getting the name itself, as long as the state isn't ACTUAL.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_NOTWS,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0
    },
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTENAME
     * AttributeName mode rules
     *
     * State changers:
     *        /                               (update set state in closure element)
     *        >                               (divert text)
     *        =                               (divert attrval)
     *        first nonspace                  (update set state in actual attrname)
     *        first space after nonspace set  (divert attrname)
     */    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '/',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        0        
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTENAME | SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL | SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTCLOSURE
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '=',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS,
        SGML_PARSER_STATE_INATTRIBUTENAME | SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL
    },
    {    // Ignore unwanted spaces.
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        0,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0,
        0
    },
    {    // Referential
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL
    },    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_NOTWS,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0
    },    
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS
     * AttributeValue_NoSpaces mode rules
     *
     * Specific changes:
     *      >       (divert SGML_PARSER_STATEINDEX_INTEXT)
     *      ' '     (divert SGML_PARSER_STATEINDEX_INATTRIBUTENAME)
     *      "       (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     *      '       (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     *     any      (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     */    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS | SGML_PARSER_STATE_INELEMENT
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '"',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '\'',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    }, // We got something like attr=attrvalue
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_ANY,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_IS_BUFFER,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S
     * AttributeValue_NoSpaces mode rules
     *
     * Specific changes:
     *      "   (divert attrname)
     *      '   (divert attrname)
     *      ws  (divert attrname if NOQUOTE)
     *      >   (divert intext)
     *
     */
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '"',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE    
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '\'',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S    
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS | SGML_PARSER_STATE_INELEMENT
    },
    /*
     * SGML_PARSER_STATEINDEX_INCOMMENT
     * Comment mode rules
     *
     * Specific changes:
     *    >       if dash1 and dash2, divert text
     *    not -   unset SGML_PARSER_STATE_INCOMMENTGOTDASH1
     *    -       set SGML_PARSER_STATE_INCOMMENTGOTDASH1
     *    -       if dash1, set dash2
     */    
    /*{
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCOMMENT | SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_NOT,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        0
    },*/
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)endCommentStr, //(DOMString)"-->",
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCOMMENTVALUE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCOMMENT | SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2 | SGML_PARSER_STATE_INCOMMENTVALUE
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_ANY,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTVALUE,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    /*
     * SGML_PARSER_STATEINDEX_INCDATA
     * Comment mode rules
     *
     * Specific changes:
     *    
     */
    /*{
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAGOTBRACKET,
        0
    },*/
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)cdataStr, // (DOMString)"CDATA",
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0, //SGML_PARSER_STATE_INCDATAGOTBRACKET,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAGOTCDATA,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        SGML_PARSER_STATE_INCDATAGOTCDATA,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAVALUE,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)endCdataStr, // (DOMString)"]]>",
        
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCDATAVALUE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCDATA | SGML_PARSER_STATE_INCDATAGOTBRACKET | SGML_PARSER_STATE_INCDATAGOTCDATA | SGML_PARSER_STATE_INCDATAVALUE
    },
};

#else

/*
 * All the possible logical states for an SGML document.
 */
SGML_STATE_TABLE sgmlStateTable[] = {
    {    // Text
        SGML_PARSER_STATEINDEX_INTEXT,
        NULL,
        0
    },
    {    // ElementName
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        NULL,
        0
    },
    {    // AttributeName
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        NULL,
        0
    },
    {    // AttributeValue_NoSpaces
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        NULL,
        0
    },
    {    // AttributeValue_Spaces
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        NULL,
        0
    },
    {    // Comment
        SGML_PARSER_STATEINDEX_INCOMMENT,
        NULL,
        0
    },
    {    // CDATA
        SGML_PARSER_STATEINDEX_INCDATA,
        NULL,
        0
    },
    /*{    // DOCTYPE
        SGML_PARSER_STATEINDEX_INDOCTYPE,
        NULL,
        0
    },
    {    // Entity
        SGML_PARSER_STATEINDEX_INENTITY,
        NULL,
        0
    },
    {    // PI
        SGML_PARSER_STATEINDEX_INPI,
        NULL,
        0
    },*/

};

SGML_STATE_TABLE_RULE sgmlStateTableRules[] = {
    /*
     * SGML_PARSER_STATEINDEX_INTEXT
     * Text mode rules
     *
     * Specific changes:
     *        <                    (divert SGML_PARSER_STATEINDEX_INELEMENTNAME)
     */
    {
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '<',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        0,
        0,
        SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTNAME,
        SGML_PARSER_STATE_INTEXT
    },
    /*
     * SGML_PARSER_STATEINDEX_INELEMENTNAME
     * ElementName mode rules
     *
     * Specific changes:
     *      /         (flag as closure element)
     *      >         (divert SGML_PARSER_STATEIDNEX_INTEXT)
     *      !         (if not actual, update state setting SGML_PARSER_STATE_INCOMMENTEXCLAMATION)
     *      [         (divert to CDATA)
     *      -         (if state EXCLA, update state setting SGML_PARSER_STATE_INCOMMENTDASH1)
     *      -         (if state DASH1, divert SGML_PARSER_STATEINDEX_INCOMMENT)
     *      not ' '   (if not actual, start with actual name of element)
     *      ' '       (if actual, divert SGML_PARSER_STATE_INATTRIBUTENAME)
     */
    {    // If we see a / in the name anywhere, flag this is as a closure element.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '/',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        0
    },
    {    // If we hit a close element tag, we've reached the end, divert back to text mode.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '!',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0,
        SGML_PARSER_STATE_INCOMMENT,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1 
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCDATA,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION
    },
    {    // If we hit another space after the name, now we divert to the attribute name section.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INELEMENTNAME | SGML_PARSER_STATE_INELEMENTNAME_ACTUAL | SGML_PARSER_STATE_INELEMENTCLOSURE | SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION | SGML_PARSER_STATE_INCOMMENTGOTDASH1
    },
    {
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        0,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0
    },
    {    // Once we get our first non-space character we safely start getting the name itself, as long as the state isn't ACTUAL.
        SGML_PARSER_STATEINDEX_INELEMENTNAME,
        SGML_STC_LETTER_TYPE_NOTWS,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        SGML_PARSER_STATE_INELEMENTNAME_ACTUAL,
        0
    },
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTENAME
     * AttributeName mode rules
     *
     * State changers:
     *        /                               (update set state in closure element)
     *        >                               (divert text)
     *        =                               (divert attrval)
     *        first nonspace                  (update set state in actual attrname)
     *        first space after nonspace set  (divert attrname)
     */    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '/',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        SGML_PARSER_STATE_INELEMENTCLOSURE,
        0        
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTENAME | SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL | SGML_PARSER_STATE_INELEMENT | SGML_PARSER_STATE_INELEMENTCLOSURE
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '=',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS,
        SGML_PARSER_STATE_INATTRIBUTENAME | SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL
    },
    {    // Ignore unwanted spaces.
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        0,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0,
        0
    },
    {    // Referential
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL
    },    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_STC_LETTER_TYPE_NOTWS,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL,
        0
    },    
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS
     * AttributeValue_NoSpaces mode rules
     *
     * Specific changes:
     *      >       (divert SGML_PARSER_STATEINDEX_INTEXT)
     *      ' '     (divert SGML_PARSER_STATEINDEX_INATTRIBUTENAME)
     *      "       (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     *      '       (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     *     any      (divert SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S)
     */    
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        0,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS | SGML_PARSER_STATE_INELEMENT
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '"',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '\'',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    }, // We got something like attr=attrvalue
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS,
        SGML_STC_LETTER_TYPE_ANY,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_IS_BUFFER,
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        0,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS
    },
    /*
     * SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S
     * AttributeValue_NoSpaces mode rules
     *
     * Specific changes:
     *      "   (divert attrname)
     *      '   (divert attrname)
     *      ws  (divert attrname if NOQUOTE)
     *      >   (divert intext)
     *
     */
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '"',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE    
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '\'',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        0,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S    
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFICWS,
        0,
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        0,
        SGML_PARSER_STATE_INATTRIBUTENAME,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_S | SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE
    },
    {
        SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_S,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INATTRIBUTEVALUE_NS | SGML_PARSER_STATE_INELEMENT
    },
    /*
     * SGML_PARSER_STATEINDEX_INCOMMENT
     * Comment mode rules
     *
     * Specific changes:
     *    >       if dash1 and dash2, divert text
     *    not -   unset SGML_PARSER_STATE_INCOMMENTGOTDASH1
     *    -       set SGML_PARSER_STATE_INCOMMENTGOTDASH1
     *    -       if dash1, set dash2
     */    
    /*{
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '>',
        NULL,
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCOMMENT | SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_NOT,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '-',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        0
    },*/
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)"-->",
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCOMMENTVALUE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCOMMENT | SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2 | SGML_PARSER_STATE_INCOMMENTVALUE
    },
    {
        SGML_PARSER_STATEINDEX_INCOMMENT,
        SGML_STC_LETTER_TYPE_ANY,
        0,
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        0,
        0,
        SGML_PARSER_STATE_INCOMMENTVALUE,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2
    },
    /*
     * SGML_PARSER_STATEINDEX_INCDATA
     * Comment mode rules
     *
     * Specific changes:
     *    
     */
    /*{
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE | SGML_STC_FLAG_INCL_IN_BUFFER,
        0,
        SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAGOTBRACKET,
        0
    },*/
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)"CDATA",
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        0, //SGML_PARSER_STATE_INCDATAGOTBRACKET,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAGOTCDATA,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_SPECIFIC,
        '[',
        NULL,
        SGML_STC_FLAG_UPDATE_STATE,
        0,
        SGML_PARSER_STATE_INCDATAGOTCDATA,
        SGML_PARSER_STATE_INCOMMENTGOTDASH1 | SGML_PARSER_STATE_INCOMMENTGOTDASH2,
        SGML_PARSER_STATE_INCDATAVALUE,
        0
    },
    {
        SGML_PARSER_STATEINDEX_INCDATA,
        SGML_STC_LETTER_TYPE_STRINGC,
        0,
        (DOMString)"]]>",
        
        SGML_STC_FLAG_DIVERT | SGML_STC_FLAG_UPDATE_STATE,
        SGML_PARSER_STATEINDEX_INTEXT,
        SGML_PARSER_STATE_INCDATAVALUE,
        0,
        SGML_PARSER_STATE_INTEXT,
        SGML_PARSER_STATE_INCDATA | SGML_PARSER_STATE_INCDATAGOTBRACKET | SGML_PARSER_STATE_INCDATAGOTCDATA | SGML_PARSER_STATE_INCDATAVALUE
    },
};

#endif

SGML_PARSER *sgmlParserNew()
{
    SGML_PARSER *ret = (SGML_PARSER *)memMgrChunkNew(sizeof(SGML_PARSER));

    if (!ret)
        return NULL;

    memMgrChunkSet(ret, 0, sizeof(SGML_PARSER));

    ret->type = SGML_EXTENSION_TYPE_CUSTOM;

    return ret;
}

unsigned long sgmlParserInitialize(SGML_PARSER *parser, enum SgmlExtensionType type, SGML_HANDLERS *handlers, void *userContext)
{
    unsigned long   ret = 1;
    
    memMgrChunkSet(parser, 0, sizeof(SGML_PARSER));

    parser->type                 = type;
    parser->internal.userContext = userContext;
    parser->internal.state       = SGML_PARSER_STATE_INTEXT;
    
    parser->internal.passthroughTags = NULL;

    ret = _sgmlParserInitializeStateTable(parser);
    if (ret != 1)
        return ret;

    if (handlers)
        memMgrChunkMove(&parser->handlers, handlers, sizeof(SGML_HANDLERS));

    switch (parser->type)
    {
        case SGML_EXTENSION_TYPE_HTML:
            parser->internal.extensionContext = (void *)sgmlExtensionHtmlNew(parser);
            break;
        case SGML_EXTENSION_TYPE_XML:
            parser->internal.extensionContext = (void *)sgmlExtensionXmlNew(parser);
            break;
        default:
            break;
    }    

    _sgmlParserInitializeStateTableRules(parser);

    return ret;
}

int _sgmlParserInitializeStateTable(SGML_PARSER *parser)
{
#ifdef PALMOS
    int     i = 0;
    
    parser->stateTableElements         = sizeof(sgmlStateTable) / sizeof(SGML_STATE_TABLE);
    parser->stateTable                 = (SGML_STATE_TABLE *)memMgrChunkNew(sizeof(sgmlStateTable)); //sgmlStateTable;
    parser->stateTableRuleElements     = sizeof(sgmlStateTableRules) / sizeof(SGML_STATE_TABLE_RULE);
    parser->stateTableRules            = (SGML_STATE_TABLE_RULE *)memMgrChunkNew(sizeof(sgmlStateTableRules)); //sgmlStateTableRules;

    if (!parser->stateTable || !parser->stateTableRules) {
        if (parser->stateTable)
            memMgrChunkFree(parser->stateTable);
        if (parser->stateTableRules)
            memMgrChunkFree(parser->stateTableRules);
        
        return 0;
    }

    for (i = 0; i < parser->stateTableElements; i++) {
        parser->stateTable[i].stateIndexId  = sgmlStateTable[i].stateIndexId;
        parser->stateTable[i].rules         = sgmlStateTable[i].rules;
        parser->stateTable[i].ruleSize      = sgmlStateTable[i].ruleSize;
    }
    
    for (i = 0; i < parser->stateTableRuleElements; i++) {
        parser->stateTableRules[i].stateIndexId     = sgmlStateTableRules[i].stateIndexId;
        parser->stateTableRules[i].letterType       = sgmlStateTableRules[i].letterType;
        parser->stateTableRules[i].letter           = sgmlStateTableRules[i].letter;

        if (sgmlStateTableRules[i].string == (DOMString)cdataStr) {
            parser->stateTableRules[i].string = stringDup("CDATA");
        } else if (sgmlStateTableRules[i].string == (DOMString)endCdataStr) {
            parser->stateTableRules[i].string = stringDup("]]>");
        } else if (sgmlStateTableRules[i].string == (DOMString)endCommentStr) {
            parser->stateTableRules[i].string = stringDup("-->");
        } else {
            parser->stateTableRules[i].string = NULL;
        }
        if (sgmlStateTableRules[i].string)
            WinDrawChars(parser->stateTableRules[i].string, StrLen(parser->stateTableRules[i].string), 0, 80);
        parser->stateTableRules[i].flags            = sgmlStateTableRules[i].flags;
        parser->stateTableRules[i].divertTableId    = sgmlStateTableRules[i].divertTableId;
        parser->stateTableRules[i].isState          = sgmlStateTableRules[i].isState;
        parser->stateTableRules[i].notState         = sgmlStateTableRules[i].notState;
        parser->stateTableRules[i].addState         = sgmlStateTableRules[i].addState;
        parser->stateTableRules[i].remState         = sgmlStateTableRules[i].remState;
    }

    parser->internal.onStateChange     = _sgmlOnStateChange;
    parser->internal.onDivert          = _sgmlOnDivert;

    parser->internal.extensionContext  = NULL;

    parser->internal.currentState      = &parser->stateTable[0];
    parser->internal.currentBuffer     = NULL;
    parser->internal.currentBufferSize = 0;
#else
    parser->stateTableElements         = sizeof(sgmlStateTable) / sizeof(SGML_STATE_TABLE);
    parser->stateTable                 = sgmlStateTable;
    parser->stateTableRuleElements     = sizeof(sgmlStateTableRules) / sizeof(SGML_STATE_TABLE_RULE);
    parser->stateTableRules            = sgmlStateTableRules;

    parser->internal.onStateChange     = _sgmlOnStateChange;
    parser->internal.onDivert          = _sgmlOnDivert;

    parser->internal.extensionContext  = NULL;

    parser->internal.currentState      = &sgmlStateTable[0];
    parser->internal.currentBuffer     = NULL;
    parser->internal.currentBufferSize = 0;
#endif
    
    return 1;
}

void _sgmlParserInitializeStateTableRules(SGML_PARSER *parser)
{
    unsigned long lastTableIndex = -1;
    unsigned long x = 0, lastChange = 0;

    for (lastTableIndex = SGML_PARSER_STATEINDEX_INTEXT,
            x = 0;
            x <= parser->stateTableRuleElements;
            x++)
    {
        /*
         * If the index changed.
         */
        if ((x == parser->stateTableRuleElements) || (parser->stateTableRules[x].stateIndexId != lastTableIndex))
        {
            parser->stateTable[lastTableIndex].ruleSize = x - lastChange; 
            parser->stateTable[lastTableIndex].rules    = &(parser->stateTableRules[0]) + lastChange;

            lastTableIndex = parser->stateTableRules[x].stateIndexId;
            lastChange = x;
        }
    }
}

void sgmlParserAddPassthroughTags(SGML_PARSER *parser, DOMString *tags)
{
    parser->internal.passthroughTags = tags;
}

void sgmlParserDestroy(SGML_PARSER *parser, unsigned char destroyParser)
{
#ifdef PALMOS
    int     i = 0;
#endif
    
    switch (parser->type)
    {
        case SGML_EXTENSION_TYPE_HTML:
            sgmlExtensionHtmlDestroy(parser, (SGML_EXTENSION_HTML *)parser->internal.extensionContext);
            break;
        case SGML_EXTENSION_TYPE_XML:
            sgmlExtensionXmlDestroy(parser, (SGML_EXTENSION_XML *)parser->internal.extensionContext);
            break;
        default:
            break;
    }

#ifdef PALMOS
    for (i = 0; i < parser->stateTableRuleElements; i++) {
        if (parser->stateTableRules[i].string)
            memMgrChunkFree(parser->stateTableRules[i].string);
    }
    
    memMgrChunkFree(parser->stateTable);
    memMgrChunkFree(parser->stateTableRules);
#endif

    if (destroyParser)
        memMgrChunkFree(parser);
}

unsigned long sgmlParserParseString(SGML_PARSER *parser, const char *string, const unsigned long stringLength)
{
    unsigned long ret;

    if (parser->handlers.preparse)
        parser->handlers.preparse(parser, parser->internal.userContext);

    ret = _sgmlParseChunk(parser, string, stringLength);
    
    if (parser->handlers.postparse)
        parser->handlers.postparse(parser, parser->internal.userContext);

    if (parser->internal.lastAttributeName)
        memMgrChunkFree(parser->internal.lastAttributeName);
    if (parser->internal.lastElementName)
        memMgrChunkFree(parser->internal.lastElementName);
    if (parser->internal.currentBuffer)
        memMgrChunkFree(parser->internal.currentBuffer);

    return ret;
}

unsigned long sgmlParserParseFile(SGML_PARSER *parser, const char *file)
{
    unsigned long chunkSize, ret = 1;
    char chunk[4096];
    FILE *fd;

    if ((fd = fileOpen(file, "r")) == NULL)
        return 0;

    if (parser->handlers.preparse)
        parser->handlers.preparse(parser, parser->internal.userContext);

    while ((ret) && ((chunkSize = fileRead(chunk, 1, sizeof(chunk), fd)) > 0))
        ret = _sgmlParseChunk(parser, chunk, chunkSize);

    if (parser->handlers.postparse)
        parser->handlers.postparse(parser, parser->internal.userContext);

    fileClose(fd);

    if (parser->internal.lastAttributeName)
        memMgrChunkFree(parser->internal.lastAttributeName);
    if (parser->internal.lastElementName)
        memMgrChunkFree(parser->internal.lastElementName);
    if (parser->internal.currentBuffer)
        memMgrChunkFree(parser->internal.currentBuffer);

    return ret;
}

void sgmlParserExtensionSetParam(SGML_PARSER *parser, unsigned long param, void *value)
{
    if (parser->internal.setExtensionParam)
        parser->internal.setExtensionParam(parser, parser->internal.extensionContext, param, value);
}

void sgmlParserExtensionGetParam(SGML_PARSER *parser, unsigned long param, void *value)
{
    if (parser->internal.getExtensionParam)
        parser->internal.getExtensionParam(parser, parser->internal.extensionContext, param, value);
}

int _sgmlIsWhitespace(char tok)
{
    return ((tok == ' ') || (tok == '\r') || (tok == '\n') || (tok == '\t'));
}

int _sgmlParserIsPassthroughTag(SGML_PARSER *parser, DOMString tag)
{
    int i = 0;
    
    if (!parser->internal.passthroughTags)
        return 0;
    
    while (parser->internal.passthroughTags[i])
    {
        if (!stringCompareCaseless(parser->internal.passthroughTags[i], tag))
            return 1;

        i++;
    }

    return 0;
}

DOMString _sgmlParserPassthroughTag(SGML_PARSER *parser, DOMString tag)
{
    int i = 0;
    
    if (!parser->internal.passthroughTags)
        return NULL;
    
    while (parser->internal.passthroughTags[i])
    {
        if (!stringCompareCaseless(parser->internal.passthroughTags[i], tag))
            return parser->internal.passthroughTags[i];
        
        i++;
    }
    
    return NULL;
}

int sgmlConvertToUTF8(const char *chunk, const unsigned long chunkSize, char *encoding, 
                                        char **convertedChunk, unsigned long *convertedSize)
{
    *convertedSize = intlEncodingToUTF8((unsigned char *)chunk, encoding, chunkSize, (unsigned char **)convertedChunk);
    
    return (*convertedSize) ? 1 : 0;
}

unsigned long _sgmlParseChunk(SGML_PARSER *parser, const char *chunk, const unsigned long chunkSize)
{
    unsigned long ret = 1, includeInBuffer = 0, divert = 0, oldState = 0, newIndex = 0, startPosition = 0, addPosition, isBuffer = 0;
    unsigned long increaseX = 0;
    unsigned long passthroughWatch = 0, passthrough = 0;
    unsigned long passBufStart = 0, passBufEnd = 0;
    char *passTag = NULL;
    //register int x = 0, ruleEnum = 0;
    // force ulong regardless of system
    register unsigned long x = 0;
    register int ruleEnum = 0;
    

    for (; x < chunkSize; x++)
    {
        if (x < increaseX)
            goto _sgmlParseChunk_handle;
            
        includeInBuffer = 1;
        isBuffer = 0;
        increaseX = 0;
        
        if (passthrough && !passBufStart)
        {
            passBufStart = x;
        }
        
        // Some poeple do something like "attrname = "value"" so wee need to catch this, only make
        // this exception for html.
        if (!passthrough && (parser->internal.state & SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL) &&
            (parser->type == SGML_EXTENSION_TYPE_HTML) && _sgmlIsWhitespace(chunk[x]))
        {
            unsigned long i = x;
            
            while (_sgmlIsWhitespace(chunk[i]) && (i <= chunkSize))
                i++;
            
            if (chunk[i] == '=')
                x = i;
        }
        else if (!passthrough && (parser->internal.state & SGML_PARSER_STATE_INATTRIBUTEVALUE_NS) &&
                 (parser->type == SGML_EXTENSION_TYPE_HTML) && _sgmlIsWhitespace(chunk[x]))
        {
            unsigned long i = x;
            
            while (_sgmlIsWhitespace(chunk[i]) && (i <= chunkSize))
                i++;
            
            if ((chunk[i] == '\'') || (chunk[i] == '\"'))
                x = i;
        }

        for (ruleEnum = 0;
                ruleEnum < parser->internal.currentState->ruleSize;
                ruleEnum++)
        {
            unsigned char matchLetter = 0;

            switch (parser->internal.currentState->rules[ruleEnum].letterType)
            {
                case SGML_STC_LETTER_TYPE_SPECIFIC:
                    if (parser->internal.currentState->rules[ruleEnum].letter == chunk[x])
                        matchLetter = 1;
                    break;
                case SGML_STC_LETTER_TYPE_NOT:
                    if (parser->internal.currentState->rules[ruleEnum].letter != chunk[x])
                        matchLetter = 1;
                    break;
                case SGML_STC_LETTER_TYPE_SPECIFICWS:
                    switch (chunk[x])
                    {
                        case ' ':
                        case '\t':
                        case '\r':
                        case '\n':
                            matchLetter = 1;
                            break;
                    }
                    break;
                case SGML_STC_LETTER_TYPE_NOTWS:
                    switch (chunk[x])
                    {
                        case ' ':
                        case '\t':
                        case '\r':
                        case '\n':
                            matchLetter = 0;
                            break;
                        default:
                            matchLetter = 1;
                            break;
                    }
                    break;
                case SGML_STC_LETTER_TYPE_ANY:
                    matchLetter = 1;
                    break;
                case SGML_STC_LETTER_TYPE_STRINGC:
                    if (!stringNCompare(parser->internal.currentState->rules[ruleEnum].string, chunk + x,
                                        stringLen(parser->internal.currentState->rules[ruleEnum].string)))
                    {
                        matchLetter = 1;
                        increaseX   = x + stringLen(parser->internal.currentState->rules[ruleEnum].string);
                    }
                    else
                    {
                        matchLetter = 0;
                    }
                    break;
                case SGML_STC_LETTER_TYPE_STRINGCL:
                    if (!stringNCompareCaseless(parser->internal.currentState->rules[ruleEnum].string, chunk + x,
                                        stringLen(parser->internal.currentState->rules[ruleEnum].string)))
                    {
                        matchLetter = 1;
                        increaseX   = x + stringLen(parser->internal.currentState->rules[ruleEnum].string);
                    }
                    else
                    {
                        matchLetter = 0;
                    }
                    break;
                default:
                    break;
            }

            if ((matchLetter)
                 && ((!parser->internal.currentState->rules[ruleEnum].isState) || (parser->internal.state & parser->internal.currentState->rules[ruleEnum].isState))
                 && ((!parser->internal.currentState->rules[ruleEnum].notState) || (!(parser->internal.state & parser->internal.currentState->rules[ruleEnum].notState))))
            {
                /*
                 * Update state?
                 */

                if (parser->internal.currentState->rules[ruleEnum].flags & SGML_STC_FLAG_UPDATE_STATE)
                {
                    unsigned long newState = (parser->internal.state | parser->internal.currentState->rules[ruleEnum].addState);

                    if (parser->internal.currentState->rules[ruleEnum].remState)
                        newState &= ~(parser->internal.currentState->rules[ruleEnum].remState);

                    oldState = parser->internal.state;
                    parser->internal.state = newState;

                    if (parser->internal.onStateChange)
                        parser->internal.onStateChange(parser, oldState, newState);
                }

                /*
                 * Include in buffer?
                 */
                if (!(parser->internal.currentState->rules[ruleEnum].flags & SGML_STC_FLAG_INCL_IN_BUFFER))
                    includeInBuffer = 0;
                    
                if ((parser->internal.currentState->rules[ruleEnum].flags & SGML_STC_FLAG_IS_BUFFER) && !includeInBuffer)
                    isBuffer = 1;

                /*
                 * Divert?
                 */
                if (parser->internal.currentState->rules[ruleEnum].flags & SGML_STC_FLAG_DIVERT)
                {
                    parser->internal.currentState = &parser->stateTable[(newIndex = parser->internal.currentState->rules[ruleEnum].divertTableId)];
                    
                    divert = 1;
                }

                break;
            }
        }

_sgmlParseChunk_handle:
        if (divert)
        {
            if (!includeInBuffer)
            {
                _sgmlParserAppendBuffer(parser, chunk, startPosition, x - isBuffer);
                addPosition  = (x - isBuffer) - startPosition;
            }
            else
            {
                _sgmlParserAppendBuffer(parser, chunk, startPosition, x + 1);
                addPosition = (x+1) - startPosition;
            }

            if (parser->internal.currentBuffer)
                parser->internal.currentBuffer[parser->internal.currentBufferSize] = 0;

            if (passthrough && (oldState & SGML_PARSER_STATE_INELEMENTCLOSURE) &&
                    _sgmlParserIsPassthroughTag(parser, parser->internal.currentBuffer + 1))
            {
                char *buffer = NULL;
                unsigned long bufferSize = 0;
                
                passthrough = 0;
                passthroughWatch = 0;
                
                passBufEnd = x - (stringLen(passTag) + 2);
                
                if (passBufEnd - passBufStart)
                {
                    _sgmlParserCopyBytesToBuffer(&buffer, &bufferSize, chunk, passBufStart, passBufEnd);
                    buffer[bufferSize] = 0;
                
                    if (parser->handlers.textNew && buffer)
                        parser->handlers.textNew(parser, parser->internal.userContext, buffer);

                    if (buffer)
                        memMgrChunkFree(buffer);
                    bufferSize = 0;
                }
                
                passBufStart = 0;
                passBufEnd = 0;
                passTag = NULL;
            }
            if (passthroughWatch && (parser->internal.state & SGML_PARSER_STATE_INTEXT))
            {
                passthrough = 1;
                passthroughWatch = 0;
            }
            else if (passthroughWatch && (parser->internal.state & SGML_PARSER_STATE_INELEMENTCLOSURE))
            {
                passthroughWatch = 0;
                passBufStart = 0;
                passBufEnd = 0;
                passTag = NULL;
            }
            
            if (!passthrough/* || (parser->internal.strictXML && (oldstate & SGML_PARSER_STATE_INCDATA))*/) {
                if (parser->internal.onDivert)
                    parser->internal.onDivert(parser, newIndex, oldState, parser->internal.state,
                                                parser->internal.currentBuffer, parser->internal.currentBufferSize);
            }

            if ((oldState & SGML_PARSER_STATE_INELEMENTNAME) &&
                _sgmlParserIsPassthroughTag(parser, parser->internal.currentBuffer))
            {
                passthroughWatch = 1; // watch out for a passthrough
                passTag = _sgmlParserPassthroughTag(parser, parser->internal.currentBuffer);
            }

            _sgmlParserResetBuffer(parser);

            startPosition += addPosition + 1;

            divert = 0;
        }
        else if (!includeInBuffer) 
        {
            _sgmlParserAppendBuffer(parser, chunk, startPosition, x - isBuffer);
            addPosition = (x - isBuffer) - startPosition;

            startPosition += addPosition + 1;
        }
    }
    
    if (x != 0)
        _sgmlParserAppendBuffer(parser, chunk, startPosition, x-1);

    return ret;
}

void _sgmlParserCopyBytesToBuffer(char **buffer, unsigned long *size, const char *chunk,
                                    unsigned long startOffset, unsigned long endOffset)
{
    unsigned long copyLength = endOffset - startOffset;
    char *saveBuf = NULL;

    if (!buffer)
        return;

    if (endOffset <= startOffset)
        return;
    
    if (!*buffer)
        *buffer = (char *)memMgrChunkNew(copyLength + 1);
    else
        *buffer = (char *)memMgrChunkRealloc((saveBuf = *buffer), *size + copyLength + 1);

    if (!*buffer)
    {
        if (saveBuf)
            memMgrChunkFree(saveBuf);
    }    
    else
        memMgrChunkMove(*buffer + *size, (void *)(chunk + startOffset), copyLength);

    *size += copyLength;
    
    /*unsigned long newSize = (*size)?(*size + 1):2;
    
    if (!buffer)
        return;
    
    *buffer = (char *)memMgrChunkRealloc(*buffer, newSize);
    if (!*buffer)
        return;
    
    *buffer[newSize - 2] = byte;
    *buffer[newSize - 1] = '\0';
    
    *size = newSize;*/
}

void _sgmlParserAppendBuffer(SGML_PARSER *parser, const char *chunk, unsigned long startOffset, unsigned long endOffset)
{
    unsigned long copyLength = endOffset - startOffset;
    char *saveBuf = NULL;

    if (endOffset <= startOffset)
        return;
    
    if (!parser->internal.currentBuffer)
        parser->internal.currentBuffer = (char *)memMgrChunkNew(copyLength + 1);
    else
        parser->internal.currentBuffer = (char *)memMgrChunkRealloc((saveBuf = parser->internal.currentBuffer), parser->internal.currentBufferSize + copyLength + 1);

    if (!parser->internal.currentBuffer)
    {
        if (saveBuf)
            memMgrChunkFree(saveBuf);
    }    
    else
        memMgrChunkMove(parser->internal.currentBuffer + parser->internal.currentBufferSize, (void *)(chunk + startOffset), copyLength);

    parser->internal.currentBufferSize += copyLength;
}

void _sgmlParserResetBuffer(SGML_PARSER *parser)
{
    if (parser->internal.currentBuffer)
        memMgrChunkFree(parser->internal.currentBuffer);

    parser->internal.currentBuffer     = NULL;
    parser->internal.currentBufferSize = 0;
}

void _sgmlOnStateChange(SGML_PARSER *parser, unsigned long oldState, unsigned long newState)
{
}

void _sgmlOnDivert(SGML_PARSER *parser, unsigned long newIndex, unsigned long oldState, unsigned long newState, const DOMString lastBuffer, unsigned long lastBufferSize)
{
    DOMString buffer = (lastBuffer)?(DOMString)lastBuffer:(DOMString)"";

    switch (newIndex)
    {
        case SGML_PARSER_STATEINDEX_INELEMENTNAME:
            if (oldState & SGML_PARSER_STATE_INTEXT)
            {
                if (parser->handlers.textNew)
                    parser->handlers.textNew(parser, parser->internal.userContext, buffer);
            }
            break;
        case SGML_PARSER_STATEINDEX_INATTRIBUTENAME:
            if (oldState & SGML_PARSER_STATE_INELEMENTNAME)
            {
                if (parser->handlers.elementBegin)
                    parser->handlers.elementBegin(parser, parser->internal.userContext, buffer);
                        
                if (parser->internal.lastElementName)
                    memMgrChunkFree(parser->internal.lastElementName);
                
                parser->internal.lastElementName = (char *)stringDup(buffer);
            }
            if (oldState & SGML_PARSER_STATE_INATTRIBUTENAME)
            {
                if (parser->handlers.attributeNew)
                    parser->handlers.attributeNew(parser, parser->internal.userContext, buffer, (DOMString)"");
            }
            if ((oldState & SGML_PARSER_STATE_INATTRIBUTEVALUE_NS) 
                    || (oldState & SGML_PARSER_STATE_INATTRIBUTEVALUE_S))
            {
                if (parser->handlers.attributeNew)
                    parser->handlers.attributeNew(parser, parser->internal.userContext, parser->internal.lastAttributeName, buffer);
            }
            break;
        case SGML_PARSER_STATEINDEX_INATTRIBUTEVALUE_NS:
            if (parser->handlers.attributeNew)
            {
                if (parser->internal.lastAttributeName)
                    memMgrChunkFree(parser->internal.lastAttributeName);

                parser->internal.lastAttributeName = (char *)stringDup(buffer);
            }
            break;
        case SGML_PARSER_STATEINDEX_INTEXT:
            if (oldState & SGML_PARSER_STATE_INELEMENTCLOSURE)
            {
                char *slashPos = 0;
            
                if (!(oldState & SGML_PARSER_STATE_INELEMENTNAME))    
                    buffer = parser->internal.lastElementName;

                if ((buffer) && ((slashPos = (char *)stringRChr(buffer, '/'))) && (slashPos != buffer))
                {
                    *slashPos = 0;

                    if (parser->handlers.elementBegin)
                    {
                        parser->handlers.elementBegin(parser, parser->internal.userContext, buffer);
                    
                        if (parser->internal.lastElementName)
                            memMgrChunkFree(parser->internal.lastElementName);

                        parser->internal.lastElementName = (char *)stringDup(buffer);
                    }
                }
                else if (oldState & SGML_PARSER_STATE_INELEMENTNAME)
                    buffer++;
                    
                if (parser->handlers.elementEnd)
                    parser->handlers.elementEnd(parser, parser->internal.userContext, buffer);
            }
            else if (oldState & SGML_PARSER_STATE_INELEMENTNAME)
            {    
                if (parser->handlers.elementBegin)
                    parser->handlers.elementBegin(parser, parser->internal.userContext, buffer);
                        
                if (parser->internal.lastElementName)
                    memMgrChunkFree(parser->internal.lastElementName);

                parser->internal.lastElementName = (char *)stringDup(buffer);
            }
            else if (oldState & SGML_PARSER_STATE_INATTRIBUTENAME && buffer && *buffer)
            {
                if (parser->handlers.attributeNew)
                    parser->handlers.attributeNew(parser, parser->internal.userContext, buffer, (DOMString)"");
            }
            else if (oldState & SGML_PARSER_STATE_INATTRIBUTEVALUE_NS)
            {
                if (parser->handlers.attributeNew)
                    parser->handlers.attributeNew(parser, parser->internal.userContext, parser->internal.lastAttributeName, lastBuffer);
            }
            else if (oldState & SGML_PARSER_STATE_INCOMMENT)
            {
                if (parser->handlers.commentNew)
                    parser->handlers.commentNew(parser, parser->internal.userContext, buffer);
            }
            else if (oldState & SGML_PARSER_STATE_INCDATA)
            {
                if (parser->handlers.cdataNew)
                    parser->handlers.cdataNew(parser, parser->internal.userContext, buffer);
            }
            break;
    }
}

