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
 
#ifndef WBXML_h
#define WBXML_h

#include "config.h"

#define TMP_BLOCK_SIZE  4096 // temporary block size

#define wbxmlErrMem                         (wbxmlErrorClass |   1)
#define wbxmlErrParamError                  (wbxmlErrorClass |   2)
#define wbxmlErrInvalidTokenLookupArrays    (wbxmlErrorClass |   3)


#define WBXML_HAS_ATTRIBUTES        0x80
#define WBXML_HAS_CONTENT           0x40
#define WBXML_TAG_ID                0x3F

// Global Tokens: wap-192-wbxml-20010725-a.pdf
#define SWITCH_PAGE                 0x00
#define END                         0x01
#define ENTITY                      0x02
#define STR_I                       0x03
#define LITERAL                     0x04
#define EXT_I_0                     0x40
#define EXT_I_1                     0x41
#define EXT_I_2                     0x42
#define PI                          0x43
#define LITERAL_C                   0x44
#define EXT_T_0                     0x80
#define EXT_T_1                     0x81
#define EXT_T_2                     0x82
#define STR_T                       0x83
#define LITERAL_A                   0x84
#define EXT_0                       0xC0
#define EXT_1                       0xC1
#define EXT_2                       0xC2
#define OPAQUE                      0xC3
#define LITERAL_AC                  0xC4

// Charsets: IANACharset
typedef enum _IANACharset {
        other = 1,   // used if the designated
                     // character set is not currently
                     // registered by IANA
        unknown = 2, // used as a default value
        csASCII = 3,
        csISOLatin1 = 4,
        csISOLatin2 = 5,
        csISOLatin3 = 6,
        csISOLatin4 = 7,
        csISOLatinCyrillic = 8,
        csISOLatinArabic = 9,
        csISOLatinGreek = 10,
        csISOLatinHebrew = 11,
        csISOLatin5 = 12,
        csISOLatin6 = 13,
        csISOTextComm = 14,
        csHalfWidthKatakana = 15,
        csJISEncoding = 16,
        csShiftJIS = 17,
        csEUCPkdFmtJapanese = 18,
        csEUCFixWidJapanese = 19,
        csISO4UnitedKingdom = 20,
        csISO11SwedishForNames = 21,
        csISO15Italian = 22,
        csISO17Spanish = 23,
        csISO21German = 24,
        csISO60DanishNorwegian = 25,
        csISO69French = 26,
        csISO10646UTF1 = 27,
        csISO646basic1983 = 28,
        csINVARIANT = 29,
        csISO2IntlRefVersion = 30,
        csNATSSEFI = 31,
        csNATSSEFIADD = 32,
        csNATSDANO = 33,
        csNATSDANOADD = 34,
        csISO10Swedish = 35,
        csKSC56011987 = 36,
        csISO2022KR = 37,
        csEUCKR = 38,
        csISO2022JP = 39,
        csISO2022JP2 = 40,
        csISO13JISC6220jp = 41,
        csISO14JISC6220ro = 42,
        csISO16Portuguese = 43,
        csISO18Greek7Old = 44,
        csISO19LatinGreek = 45,
        csISO25French = 46,
        csISO27LatinGreek1 = 47,
        csISO5427Cyrillic = 48,
        csISO42JISC62261978 = 49,
        csISO47BSViewdata = 50,
        csISO49INIS = 51,
        csISO50INIS8 = 52,
        csISO51INISCyrillic = 53,
        csISO54271981 = 54,
        csISO5428Greek = 55,
        csISO57GB1988 = 56,
        csISO58GB231280 = 57,
        csISO61Norwegian2 = 58,
        csISO70VideotexSupp1 = 59,
        csISO84Portuguese2 = 60,
        csISO85Spanish2 = 61,
        csISO86Hungarian = 62,
        csISO87JISX0208 = 63,
        csISO88Greek7 = 64,
        csISO89ASMO449 = 65,
        csISO90 = 66,
        csISO91JISC62291984a = 67,
        csISO92JISC62991984b = 68,
        csISO93JIS62291984badd = 69,
        csISO94JIS62291984hand = 70,
        csISO95JIS62291984handadd = 71,
        csISO96JISC62291984kana = 72,
        csISO2033 = 73,
        csISO99NAPLPS = 74,
        csISO102T617bit = 75,
        csISO103T618bit = 76,
        csISO111ECMACyrillic = 77,
        csa71 = 78,
        csa72 = 79,
        csISO123CSAZ24341985gr = 80,
        csISO88596E = 81,
        csISO88596I = 82,
        csISO128T101G2 = 83,
        csISO88598E = 84,
        csISO88598I = 85,
        csISO139CSN369103 = 86,
        csISO141JUSIB1002 = 87,
        csISO143IECP271 = 88,
        csISO146Serbian = 89,
        csISO147Macedonian = 90,
        csISO150 = 91,
        csISO151Cuba = 92,
        csISO6937Add = 93,
        csISO153GOST1976874 = 94,
        csISO8859Supp = 95,
        csISO10367Box = 96,
        csISO158Lap = 97,
        csISO159JISX02121990 = 98,
        csISO646Danish = 99,
        csUSDK = 100,
        csDKUS = 101,
        csKSC5636 = 102,
        csUnicode11UTF7 = 103,
        csISO2022CN = 104,
        csISO2022CNEXT = 105,
        csUTF8 = 106,
        csISO885913 = 109,
        csISO885914 = 110,
        csISO885915 = 111,
        csISO885916 = 112,
        csGBK = 113,
        csGB18030 = 114,
        csOSDEBCDICDF0415 = 115,
        csOSDEBCDICDF03IRV = 116,
        csOSDEBCDICDF041 = 117,
        csISO115481 = 118,
        csKZ1048 = 119,
        csUnicode = 1000,
        csUCS4 = 1001,
        csUnicodeASCII = 1002,
        csUnicodeLatin1 = 1003,
        csUnicodeIBM1261 = 1005,
        csUnicodeIBM1268 = 1006,
        csUnicodeIBM1276 = 1007,
        csUnicodeIBM1264 = 1008,
        csUnicodeIBM1265 = 1009,
        csUnicode11 = 1010,
        csSCSU = 1011,
        csUTF7 = 1012,
        csUTF16BE = 1013,
        csUTF16LE = 1014,
        csUTF16 = 1015,
        csCESU8 = 1016,
        csUTF32 = 1017,
        csUTF32BE = 1018,
        csUTF32LE = 1019,
        csBOCU1 = 1020,
        csWindows30Latin1 = 2000,
        csWindows31Latin1 = 2001,
        csWindows31Latin2 = 2002,
        csWindows31Latin5 = 2003,
        csHPRoman8 = 2004,
        csAdobeStandardEncoding = 2005,
        csVenturaUS = 2006,
        csVenturaInternational = 2007,
        csDECMCS = 2008,
        csPC850Multilingual = 2009,
        csPCp852 = 2010,
        csPC8CodePage437 = 2011,
        csPC8DanishNorwegian = 2012,
        csPC862LatinHebrew = 2013,
        csPC8Turkish = 2014,
        csIBMSymbols = 2015,
        csIBMThai = 2016,
        csHPLegal = 2017,
        csHPPiFont = 2018,
        csHPMath8 = 2019,
        csHPPSMath = 2020,
        csHPDesktop = 2021,
        csVenturaMath = 2022,
        csMicrosoftPublishing = 2023,
        csWindows31J = 2024,
        csGB2312 = 2025,
        csBig5 = 2026,
        csMacintosh = 2027,
        csIBM037 = 2028,
        csIBM038 = 2029,
        csIBM273 = 2030,
        csIBM274 = 2031,
        csIBM275 = 2032,
        csIBM277 = 2033,
        csIBM278 = 2034,
        csIBM280 = 2035,
        csIBM281 = 2036,
        csIBM284 = 2037,
        csIBM285 = 2038,
        csIBM290 = 2039,
        csIBM297 = 2040,
        csIBM420 = 2041,
        csIBM423 = 2042,
        csIBM424 = 2043,
        csIBM500 = 2044,
        csIBM851 = 2045,
        csIBM855 = 2046,
        csIBM857 = 2047,
        csIBM860 = 2048,
        csIBM861 = 2049,
        csIBM863 = 2050,
        csIBM864 = 2051,
        csIBM865 = 2052,
        csIBM868 = 2053,
        csIBM869 = 2054,
        csIBM870 = 2055,
        csIBM871 = 2056,
        csIBM880 = 2057,
        csIBM891 = 2058,
        csIBM903 = 2059,
        csIBBM904 = 2060,
        csIBM905 = 2061,
        csIBM918 = 2062,
        csIBM1026 = 2063,
        csIBMEBCDICATDE = 2064,
        csEBCDICATDEA = 2065,
        csEBCDICCAFR = 2066,
        csEBCDICDKNO = 2067,
        csEBCDICDKNOA = 2068,
        csEBCDICFISE = 2069,
        csEBCDICFISEA = 2070,
        csEBCDICFR = 2071,
        csEBCDICIT = 2072,
        csEBCDICPT = 2073,
        csEBCDICES = 2074,
        csEBCDICESA = 2075,
        csEBCDICESS = 2076,
        csEBCDICUK = 2077,
        csEBCDICUS = 2078,
        csUnknown8BiT = 2079,
        csMnemonic = 2080,
        csMnem = 2081,
        csVISCII = 2082,
        csVIQR = 2083,
        csKOI8R = 2084,
        csHZGB2312 = 2085,
        csIBM866 = 2086,
        csPC775Baltic = 2087,
        csKOI8U = 2088,
        csIBM00858 = 2089,
        csIBM00924 = 2090,
        csIBM01140 = 2091,
        csIBM01141 = 2092,
        csIBM01142 = 2093,
        csIBM01143 = 2094,
        csIBM01144 = 2095,
        csIBM01145 = 2096,
        csIBM01146 = 2097,
        csIBM01147 = 2098,
        csIBM01148 = 2099,
        csIBM01149 = 2100,
        csBig5HKSCS = 2101,
        csIBM1047 = 2102,
        csPTCP154 = 2103,
        csAmiga1251 = 2104,
        csKOI7switched = 2105,
        csBRF = 2106,
        csTSCII = 2107,
        cswindows1250 = 2250,
        cswindows1251 = 2251,
        cswindows1252 = 2252,
        cswindows1253 = 2253,
        cswindows1254 = 2254,
        cswindows1255 = 2255,
        cswindows1256 = 2256,
        cswindows1257 = 2257,
        cswindows1258 = 2258,
        csTIS620 = 2259,
        csReserved = 3000
} IANACharset;

struct _WBXMLParserType;

typedef struct _WBXMLHandlersType {

    /**
     * Called prior to parsing the document
     */
    void (*preparse)(struct _WBXMLParserType *parser, void *userContext);
    /**
     * Called after parsing the document.
     */
    void (*postparse)(struct _WBXMLParserType *parser, void *userContext);
    
    /**
     * Called when destroying the parser.
     */
    void (*destroy)(struct _WBXMLParserType *parser, void *userContext);

    /**
     * Called when an element is beginning.
     */
    void (*elementBegin)(struct _WBXMLParserType *parser, void *userContext, const char *elementName);
    /**
     * Called when an element is closed.
     */
    void (*elementEnd)(struct _WBXMLParserType *parser, void *userContext, const char *elementName);

    /**
     * Called when an attribute is done.
     */
    void (*attributeNew)(struct _WBXMLParserType *parser, void *userContext, const char *attributeName, const char *attributeValue);

    /**
     * Called when a text node is done.
     */
    void (*textNew)(struct _WBXMLParserType *parser, void *userContext, const char *text);

} WBXMLHandlersType;

typedef WBXMLHandlersType *WBXMLHandlersPtr;

typedef struct _WBXMLAttrValuePair {
    char    *attr;
    char    *value;
    boolean partialValue;   // Some attr/values will get encoded sort of like:
                            //  href=http://www.google.com/ -> 0x48www.google.com/
} WBXMLAttrValuePair;

typedef enum _WBXMLExtensionType {
    wbxmlCustom,
    wbxmlWMLC,  // Compiled WML
    wbxmlSI,    // Service Indication
    wbxmlSL,    // Service Loading
    wbxmlCO,    // Cache Object
    wbxmlProv   // Provisioning
} WBXMLExtensionType;

typedef struct _WBXMLParserType {
    void                *userContext;
    
    WBXMLHandlersType   handlers;
    
    unsigned char       *data;
    // temp vars, put here to avoid stack overflows.
    char                *attrstr;
    char                *tmpstr;
    char                *strTable;
    char                **stack;
    
    char                **wbxmlTagList;
    WBXMLAttrValuePair  *wbxmlAttrValPairList;
    char                **wbxmlAttrValList;
    
    unsigned long       decksize;
    unsigned long       offset;
    unsigned long       stackDepth;
    unsigned long       maxstack;
    int                 version;
    int                 publicID;
    //int                 charSet;
    IANACharset         charSet;
    
    WBXMLExtensionType  type;
    
    unsigned char       tag;
} WBXMLParserType;

typedef WBXMLParserType *WBXMLParserPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern int wbxmlPushStack(char *tagname, WBXMLParserPtr wbxml);
extern void wbxmlFreeStack(WBXMLParserPtr wbxml);
extern char *wbxmlPopStack(WBXMLParserPtr wbxml);

extern int wbxmlGetAttributeValue(WBXMLParserPtr wbxml, char *attrstr);
extern char *wbxmlGetAttribute(WBXMLParserPtr wbxml, unsigned char attr);
extern void wbxmlGetAttributes(WBXMLParserPtr wbxml);

extern int wbxmlReadElement(WBXMLParserPtr wbxml);

extern WBXMLParserPtr wbxmlNewParser(void);
extern int wbxmlParserInitialize(WBXMLParserPtr parser, WBXMLExtensionType type, WBXMLHandlersPtr handlers, void *userContext);

extern void wbxmlParserAddTokenLookups(WBXMLParserPtr parser, char **wbxmlTagList,
                                WBXMLAttrValuePair *wbxmlAttrValPairList, char **wbxmlAttrValList);

extern WBXMLExtensionType wbxmlGetExtension(WBXMLParserPtr parser);
extern void *wbxmlParserGetContext(WBXMLParserPtr parser);

extern int wbxmlParseHeader(unsigned char *data, unsigned long length, WBXMLParserPtr wbxml);
extern int wbxmlParse(unsigned char *data, unsigned long length, int offset, WBXMLParserPtr wbxml);

extern int wbxmlFree(WBXMLParserPtr wbxml);
extern void wbxmlParserDestroy(WBXMLParserPtr *parser);

#ifdef __cplusplus
}
#endif

#endif
