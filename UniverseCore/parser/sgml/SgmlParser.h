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
 * @ingroup SGMLParser
 */
/**
 * @{
 */
/**
 * @example test.c
 * Example for using user defined callbacks for parsing SGML.
 */
#ifndef _LIBHTTP_SGMLPARSER_H
#define _LIBHTTP_SGMLPARSER_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

enum SgmlExtensionType {
    SGML_EXTENSION_TYPE_XML = 0,
    SGML_EXTENSION_TYPE_HTML,

    SGML_EXTENSION_TYPE_CUSTOM = 255
};

struct _sgml_parser;

/**
 *     Basic handlers of SGML parsing.
 *
 * @short Basic handlers for SGML parsing.
 */
typedef struct _sgml_handlers {

    /**
     * Called prior to parsing the document
     */
    void (*preparse)(struct _sgml_parser *parser, void *userContext);
    /**
     * Called after parsing the document.
     */
    void (*postparse)(struct _sgml_parser *parser, void *userContext);

    /**
     * Called when an element is beginning.
     */
    void (*elementBegin)(struct _sgml_parser *parser, void *userContext, const DOMString elementName);
    /**
     * Called when an element is closed.
     */
    void (*elementEnd)(struct _sgml_parser *parser, void *userContext, const DOMString elementName);

    /**
     * Called when an attribute is done.
     */
    void (*attributeNew)(struct _sgml_parser *parser, void *userContext, const DOMString attributeName, const DOMString attributeValue);

    /**
     * Called when a text node is done.
     */
    void (*textNew)(struct _sgml_parser *parser, void *userContext, const DOMString text);

    /**
     * Called when a comment is done.
     */
    void (*commentNew)(struct _sgml_parser *parser, void *userContext, const DOMString comment);
    
    /**
     * Called when a cdata is done.
     */
    void (*cdataNew)(struct _sgml_parser *parser, void *userContext, const DOMString data);

} SGML_HANDLERS;

/**
 * @}
 */

#define SGML_STC_LETTER_TYPE_SPECIFIC   0x00
#define SGML_STC_LETTER_TYPE_SPECIFICWS 0x01
#define SGML_STC_LETTER_TYPE_NOT        0x02
#define SGML_STC_LETTER_TYPE_NOTWS      0x03
#define SGML_STC_LETTER_TYPE_ANY        0x04
#define SGML_STC_LETTER_TYPE_STRINGC    0x05 // case string compare
#define SGML_STC_LETTER_TYPE_STRINGCL   0x06 // caseless string compare

#define SGML_STC_FLAG_DIVERT           (1 << 0)
#define SGML_STC_FLAG_UPDATE_STATE     (1 << 1)
#define SGML_STC_FLAG_INCL_IN_BUFFER   (1 << 2)
#define SGML_STC_FLAG_IS_BUFFER        (1 << 3)


#define SGML_PARSER_STATE_INTEXT                      (1ul << 0)

#define SGML_PARSER_STATE_INELEMENT                   (1ul << 1)
#define SGML_PARSER_STATE_INELEMENTNAME               (1ul << 2)
#define SGML_PARSER_STATE_INELEMENTNAME_ACTUAL        (1ul << 3)
#define SGML_PARSER_STATE_INELEMENTCLOSURE            (1ul << 4)
    
#define SGML_PARSER_STATE_INATTRIBUTENAME             (1ul << 5)
#define SGML_PARSER_STATE_INATTRIBUTENAME_ACTUAL      (1ul << 6)

#define SGML_PARSER_STATE_INATTRIBUTEVALUE_NS         (1ul << 10)

#define SGML_PARSER_STATE_INATTRIBUTEVALUE_S          (1ul << 12) 

#define SGML_PARSER_STATE_INATTRIBUTEVALUE_NOQUOTE    (1ul << 13)

#define SGML_PARSER_STATE_INCOMMENT                   (1ul << 15)
#define SGML_PARSER_STATE_INCOMMENTGOTEXCLAMATION     (1ul << 16)
#define SGML_PARSER_STATE_INCOMMENTGOTDASH1           (1ul << 17)
#define SGML_PARSER_STATE_INCOMMENTGOTDASH2           (1ul << 18)
#define SGML_PARSER_STATE_INCOMMENTVALUE              (1ul << 19)

#define SGML_PARSER_STATE_INATTRIBUTEVALUE_DBLQUOTE   (1ul << 20)

#define SGML_PARSER_STATE_INDOCTYPE                   (1ul << 21)

#define SGML_PARSER_STATE_INCDATA                     (1ul << 22)
#define SGML_PARSER_STATE_INCDATAGOTBRACKET           (1ul << 23)
#define SGML_PARSER_STATE_INCDATAGOTCDATA             (1ul << 24)
#define SGML_PARSER_STATE_INCDATAVALUE                (1ul << 25)


typedef struct _sgml_state_table_rule {

    unsigned long stateIndexId;

    unsigned char letterType;
    unsigned char letter;
    char          *string;

    unsigned long flags;

    unsigned long divertTableId;

    unsigned long isState;
    unsigned long notState;

    unsigned long addState;    
    unsigned long remState;

} SGML_STATE_TABLE_RULE;

typedef struct _sgml_state_table {

    unsigned long          stateIndexId;

    SGML_STATE_TABLE_RULE  *rules;
    unsigned long          ruleSize;

} SGML_STATE_TABLE;

typedef struct _sgml_parser {

    enum SgmlExtensionType type;

    SGML_HANDLERS               handlers;    

    SGML_STATE_TABLE            *stateTable;
    unsigned long               stateTableElements;
    SGML_STATE_TABLE_RULE       *stateTableRules;
    unsigned long               stateTableRuleElements;

    struct {

        SGML_STATE_TABLE        *currentState;

        char                    **passthroughTags;

        char                    *lastElementName;
        char                    *lastAttributeName;

        char                    *currentBuffer;
        unsigned long           currentBufferSize;

        unsigned long           state;

        void                    *extensionContext;
        void                    *userContext;

        void                    (*onStateChange)(struct _sgml_parser *parser, unsigned long oldState, unsigned long newState);
        void                    (*onDivert)(struct _sgml_parser *parser, unsigned long newIndex, unsigned long oldState, unsigned long newState, const DOMString lastBuffer, unsigned long lastBufferSize);

        void                    (*setExtensionParam)(struct _sgml_parser *parser, void *extensionContext, unsigned long param, void *value);
        void                    (*getExtensionParam)(struct _sgml_parser *parser, void *extensionContext, unsigned long param, void *value);

    } internal;

} SGML_PARSER;

typedef SGML_PARSER SGMLParserType;

typedef SGMLParserType *SGMLParserPtr;

/**
 * @addtogroup SGMLParser
 *
 * @{
 */

/**
 * Constructs and initializes a new SGML_PARSER instance.
 *
 * @return On success, an initialized SGML_PARSER instance is returned.  Otherwise, NULL is returned.
 */
SGML_PARSER *sgmlParserNew();
/**
 * Initializes an SGML_PARSER.
 *
 * The type parameter can be one of the following:
 *
 * @li SGML_EXTENSION_TYPE_XML
 *         Use the XML parser.
 * @li SGML_EXTENSION_TYPE_HTML
 *         Use the HTML parser.
 * @li SGML_EXTENSION_TYPE_CUSTOM
 *         Use a custom parser.
 *
 * @param  parser      [in] The parser context.
 * @param  type        [in] The type of parser to use.
 * @param  handlers    [in] User defined handlers.  These should only be set if the type is SGML_EXTENSION_TYPE_CUSTOM.
 * @param  userContext [in] The arbitrary user context that is passed into the handlers.
 * @return On success, one is returned.
 */
unsigned long sgmlParserInitialize(SGML_PARSER *parser, enum SgmlExtensionType type, SGML_HANDLERS *handlers, void *userContext);

/**
 * Adds an array of character pointers that point to tags to be "passed through" when parsing.
 *
 * @param  parser        [in] The parser context.
 * @param  tags          [in] The array of tags to passthrough. Last pointer must be NULL.
 */
void sgmlParserAddPassthroughTags(SGML_PARSER *parser, DOMString *tags);

/**
 * Destroys and deinitializes an SGML_PARSER.  If the destroyParser parameter is 1, the pointer passed in is deallocated by free().
 *
 * @param  parser        [in] The parser context.
 * @param  destroyParser [in] 1 if the parser should be destroyed by free(), 0 if not.
 */
void sgmlParserDestroy(SGML_PARSER *parser, unsigned char destroyParser);

/**
 * Convert a given input encoded in type encoding to UTF-8 for parsing, storing and handling.
 *
 * @param  chunk          [in]  The chunk to convert.
 * @param  chunkSize      [in]  The size of the chunk to convert.
 * @param  encoding       [in]  The encoding of the chunk to convert.
 * @param  convertedChunk [out] A pointer to the newly created and converted chunk.
 * @param  convertedSize  [out] The size of the newly converted chunk.
 * @return On success, one is returned.  Otherwise, 0 is returned.
 */
int sgmlConvertToUTF8(const char *chunk, const unsigned long chunkSize, char *encoding, 
                                        char **convertedChunk, unsigned long *convertedSize);

/**
 * Parses a given string.
 *
 * @param  parser       [in] The parser context.
 * @param  string       [in] The string to parser.
 * @param  stringLength [in] The length of the string to be parsed.
 * @return On success, one is returned.  Otherwise, 0 is returned.
 */
unsigned long sgmlParserParseString(SGML_PARSER *parser, const char *string, const unsigned long stringLength);
/**
 * Parser a given file.
 *
 * @param  parser [in] The parser context.
 * @param  file   [in] The name of the file to parse.
 * @return On success, one is returned.  Otherwise, 0 is returned.
 */
unsigned long sgmlParserParseFile(SGML_PARSER *parser, const char *file);

/**
 * Sets a parameter on a given SGML parser extension.
 *
 * If the type passed to initialize was...
 *
 * @li SGML_PARSER_TYPE_XML
 *         Look at sgmlExtensionXmlSetParam
 * @li SGML_PARSER_TYPE_HTML
 *         Look at sgmlExtensionHtmlSetParam
 * @li SGML_PARSER_TYPE_CUSTOM
 *         This method has no use.
 */
void sgmlParserExtensionSetParam(SGML_PARSER *parser, unsigned long param, void *value);
/**
 * Gets a parameter on a given SGML parser extension.
 *
 * If the type passed to initialize was...
 *
 * @li SGML_PARSER_TYPE_XML
 *         Look at sgmlExtensionXmlSetParam
 * @li SGML_PARSER_TYPE_HTML
 *         Look at sgmlExtensionHtmlSetParam
 * @li SGML_PARSER_TYPE_CUSTOM
 *         This method has no use.
 */
void sgmlParserExtensionGetParam(SGML_PARSER *parser, unsigned long param, void *value);

#define sgmlParserGetExtensionContext(parser) parser->internal.extensionContext
#define sgmlParserGetUserContext(parser) parser->internal.userContext

/**
 * @}
 */

int _sgmlParserInitializeStateTable(SGML_PARSER *parser);
void _sgmlParserInitializeStateTableRules(SGML_PARSER *parser);

int _sgmlParserIsPassthroughTag(SGML_PARSER *parser, DOMString tag);
DOMString _sgmlParserPassthroughTag(SGML_PARSER *parser, DOMString tag);

int _sgmlIsWhitespace(char tok);

unsigned long _sgmlParseChunk(SGML_PARSER *parser, const char *chunk, const unsigned long chunkSize);
void _sgmlParserAppendBuffer(SGML_PARSER *parser, const char *chunk, unsigned long startOffset, unsigned long length);
void _sgmlParserResetBuffer(SGML_PARSER *parser);
void _sgmlParserCopyBytesToBuffer(char **buffer, unsigned long *size, const char *chunk,
                                    unsigned long startOffset, unsigned long endOffset);

void _sgmlOnStateChange(SGML_PARSER *parser, unsigned long oldState, unsigned long newState);
void _sgmlOnDivert(SGML_PARSER *parser, unsigned long newIndex, unsigned long oldState, unsigned long newState, const DOMString lastBuffer, unsigned long lastBufferSize);

#ifdef __cplusplus
}
#endif

#endif
