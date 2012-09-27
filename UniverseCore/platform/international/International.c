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
#include "config.h"
#include "International.h"

#include "InternationalConv.h"
#include "MemMgr.h"
#include "Strings.h"
#include "WBXML.h"

int intlLookupConverterByEncodingString(char *encoding);

typedef struct EncodingConverterLookupTableType {
    char            *name;
    int             (*mbtowc)(conv_t conv, ucs4_t *pwc, const unsigned char *s, int n);
    int             (*wctomb)(conv_t conv, unsigned char *r, ucs4_t wc, int n);
    int             (*reset)(conv_t conv, unsigned char *r, int n);
    int             (*flush)(conv_t conv, ucs4_t *pwc);
    IANACharset     wapCharset;
} EncodingConverterLookupTableType;

// Names from:
//  http://developer.apple.com/documentation/Carbon/Conceptual/ProgWithTECM/tecmgr_encnames/chapter_4_section_4.html#//apple_ref/doc/uid/TP40000932-CH204-CJBHCICD
const EncodingConverterLookupTableType conversionTable[] = {
// International standards and some latin/western-europe languages are supported by default
// INTL_UNICODE_SUPPORT
    { "UTF-32", utf32_mbtowc, utf32_wctomb, NULL, NULL, csUTF32 },
    { "UTF-16", utf16_mbtowc, utf16_wctomb, NULL, NULL, csUTF16 },
    { "UTF-8", utf8_mbtowc, utf8_wctomb, NULL, NULL, csUTF8 },
    { "UTF-7", utf7_mbtowc, utf7_wctomb, NULL, NULL, csUTF7 },
    { "Unicode", utf16_mbtowc, utf16_wctomb, NULL, NULL, csUTF16 },
    
    { "UCS-4", ucs4_mbtowc, ucs4_wctomb, NULL, NULL, csUCS4 },
    { "UCS-2", ucs2_mbtowc, ucs2_wctomb, NULL, NULL, csUTF8 }, // csUTF8 wrong here? does not really matter...
//

// INTL_WESTERNEURO_SUPPORT
    { "US-ASCII", ascii_mbtowc, ascii_wctomb, NULL, NULL, csASCII },
    { "US-ASCII", ascii_mbtowc, ascii_wctomb, NULL, NULL, other }, // this is to trap any WBXML encodings that are set to other
    { "ISO-8859-1", iso8859_1_mbtowc, iso8859_1_wctomb, NULL, NULL, csISOLatin1 }, { "latin1", iso8859_1_mbtowc, iso8859_1_wctomb, NULL, NULL, csISOLatin1 },
    { "ISO-8859-3", iso8859_3_mbtowc, iso8859_3_wctomb, NULL, NULL, csISOLatin3 }, { "latin3", iso8859_3_mbtowc, iso8859_3_wctomb, NULL, NULL, csISOLatin3 },
    { "ISO-8859-15", iso8859_15_mbtowc, iso8859_15_wctomb, NULL, NULL, other }, { "latin9", iso8859_15_mbtowc, iso8859_15_wctomb, NULL, NULL, other },
    { "windows-1252", cp1252_mbtowc, cp1252_wctomb, NULL, NULL, cswindows1252 }, { "cp1252", cp1252_mbtowc, cp1252_wctomb, NULL, NULL, cswindows1252 },
    { "cp437", cp437_mbtowc, cp437_wctomb, NULL, NULL, csPC8CodePage437 },
    { "cp850", cp850_mbtowc, cp850_wctomb, NULL, NULL, csPC850Multilingual },
    { "mac", mac_roman_mbtowc, mac_roman_wctomb, NULL, NULL, csMacintosh }, { "macintosh", mac_roman_mbtowc, mac_roman_wctomb, NULL, NULL, csMacintosh }, { "x-mac-roman", mac_roman_mbtowc, mac_roman_wctomb, NULL, NULL, csMacintosh },
    { "x-mac-icelandic", mac_iceland_mbtowc, mac_iceland_wctomb, NULL, NULL, other },
//    { "x-mac-latin1", , , NULL, NULL, other },
//


#ifdef INTL_NEXTSTEPLATIN_SUPPORT
    { "cp037", nextstep_mbtowc, nextstep_wctomb, NULL, NULL, csIBM037 },
#endif


#ifdef INTL_ARABIC_SUPPORT
    { "ISO-8859-6", iso8859_6_mbtowc, iso8859_6_wctomb, NULL, NULL, csISOLatinArabic }, { "arabic", iso8859_6_mbtowc, iso8859_6_wctomb, NULL, NULL, csISOLatinArabic },
    { "windows-1256", cp1256_mbtowc, cp1256_wctomb, NULL, NULL, cswindows1256 }, { "cp1256", cp1256_mbtowc, cp1256_wctomb, NULL, NULL, cswindows1256 },
    { "cp864", cp864_mbtowc, cp864_wctomb, NULL, NULL, csIBM864 },
    { "x-mac-arabic", mac_arabic_mbtowc, mac_arabic_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_ROMAN_SUPPORT
    { "ISO-8859-2", iso8859_2_mbtowc, iso8859_2_wctomb, NULL, NULL, csISOLatin2 }, { "latin2", iso8859_2_mbtowc, iso8859_2_wctomb, NULL, NULL, csISOLatin2 },
    { "ISO-8859-4", iso8859_4_mbtowc, iso8859_4_wctomb, NULL, NULL, csISOLatin4 }, { "latin4", iso8859_4_mbtowc, iso8859_4_wctomb, NULL, NULL, csISOLatin4 },
    { "windows-1250", cp1250_mbtowc, cp1250_wctomb, NULL, NULL, cswindows1250 }, { "cp 1250", cp1250_mbtowc, cp1250_wctomb, NULL, NULL, cswindows1250 }, { "cp1250", cp1250_mbtowc, cp1250_wctomb, NULL, NULL, cswindows1250 },
    { "windows-1257", cp1257_mbtowc, cp1257_wctomb, NULL, NULL, cswindows1257 }, { "cp 1257", cp1257_mbtowc, cp1257_wctomb, NULL, NULL, cswindows1257 }, { "cp1257", cp1257_mbtowc, cp1257_wctomb, NULL, NULL, cswindows1257 },
    { "x-mac-centraleurroman", mac_centraleurope_mbtowc, mac_centraleurope_wctomb, NULL, NULL, other },
    { "x-mac-croatian", mac_croatian_mbtowc, mac_croatian_wctomb, NULL, NULL, other },
    { "x-mac-romanian", mac_romania_mbtowc, mac_romania_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_CHINESE_SUPPORT
    { "GB2312", gb2312_mbtowc, gb2312_wctomb, NULL, NULL, csGB2312 },
    { "X-EUC-CN", euc_cn_mbtowc, euc_cn_wctomb, NULL, NULL, csGB2312 },
    { "GBK", gbk_mbtowc, gbk_wctomb, NULL, NULL, csGBK },
//    { "ISO-2022-CN", iso2022_cn_mbtowc, iso2022_cn_wctomb, NULL, NULL, csISO2022CN },
    { "HZ-GB-2312", hz_mbtowc, hz_wctomb, NULL, NULL, csHZGB2312 },
//    { "x-cns11643-1", cns11643_1_mbtowc, cns11643_1_wctomb, NULL, NULL, other },
//    { "x-cns11643-2", cns11643_2_mbtowc, cns11643_2_wctomb, NULL, NULL, other },
//    { "X-EUC-TW", euc_tw_mbtowc, euc_tw_wctomb, NULL, NULL, other },
    { "Big5", big5_mbtowc, big5_wctomb, NULL, NULL, csBig5 },
#endif

#ifdef INTL_CYRILLIC_SUPPORT
    { "ISO-8859-5", iso8859_5_mbtowc, iso8859_5_wctomb, NULL, NULL, csISOLatinCyrillic }, { "cyrillic", iso8859_5_mbtowc, iso8859_5_wctomb, NULL, NULL, csISOLatinCyrillic },
    { "KOI8-R", koi8_r_mbtowc, koi8_r_wctomb, NULL, NULL, csKOI8R },
    { "windows-1251", cp1251_mbtowc, cp1251_wctomb, NULL, NULL, cswindows1251 }, { "cp1251", cp1251_mbtowc, cp1251_wctomb, NULL, NULL, cswindows1251 },
    { "cp866", cp866_mbtowc, cp866_wctomb, NULL, NULL, csIBM866 },
    { "x-mac-cyrillic", mac_cyrillic_mbtowc, mac_cyrillic_wctomb, NULL, NULL, other },
    { "x-mac-ukrainian", mac_ukraine_mbtowc, mac_ukraine_wctomb, NULL, NULL, other },
    { "ISO-8859-7", iso8859_7_mbtowc, iso8859_7_wctomb, NULL, NULL, csISOLatinGreek }, { "greek", iso8859_7_mbtowc, iso8859_7_wctomb, NULL, NULL, csISOLatinGreek  },
    { "windows-1253", cp1253_mbtowc, cp1253_wctomb, NULL, NULL, cswindows1253 }, { "cp1253", cp1253_mbtowc, cp1253_wctomb, NULL, NULL, cswindows1253 },
    { "x-mac-greek", mac_greek_mbtowc, mac_greek_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_HEBREW_SUPPORT
    { "ISO-8859-8", iso8859_8_mbtowc, iso8859_8_wctomb, NULL, NULL, csISOLatinHebrew }, { "hebrew", iso8859_8_mbtowc, iso8859_8_wctomb, NULL, NULL, csISOLatinHebrew },
    { "windows-1255", cp1255_mbtowc, cp1255_wctomb, NULL, NULL, cswindows1255 }, { "cp1255", cp1255_mbtowc, cp1255_wctomb, NULL, NULL, cswindows1255 },
    { "x-mac-hebrew", mac_hebrew_mbtowc, mac_hebrew_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_JAPANESE_SUPPORT
    { "EUC-JP", euc_jp_mbtowc, euc_jp_wctomb, NULL, NULL, other }, { "X-EUC-JP", euc_jp_mbtowc, euc_jp_wctomb, NULL, NULL, other }, // other ??
    { "ISO-2022-JP", iso2022_jp_mbtowc, iso2022_jp_wctomb, NULL, NULL, csISO2022JP },
    { "Shift_JIS", sjis_mbtowc, sjis_wctomb, NULL, NULL, csShiftJIS }, { "x-sjis", sjis_mbtowc, sjis_wctomb, NULL, NULL, csShiftJIS }, { "x-shift-jis", sjis_mbtowc, sjis_wctomb, NULL, NULL, csShiftJIS },
#endif

#ifdef INTL_KOREAN_SUPPORT
    { "EUC-KR", euc_kr_mbtowc, euc_kr_wctomb, NULL, NULL, csEUCKR },
    { "ISO-2022-KR", iso2022_kr_mbtowc, iso2022_kr_wctomb, NULL, NULL, csISO2022KR },
#endif

#ifdef INTL_THAI_SUPPORT
    { "cp874", cp874_mbtowc, cp874_wctomb, NULL, NULL, other },
    { "x-mac-thai", mac_thai_mbtowc, mac_thai_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_TURKISH_SUPPORT
    { "ISO-8859", iso8859_9_mbtowc, iso8859_9_wctomb, NULL, NULL, csISOLatin5 }, { "latin5", iso8859_9_mbtowc, iso8859_9_wctomb, NULL, NULL, csISOLatin5 },
    { "windows-1254", cp1254_mbtowc, cp1254_wctomb, NULL, NULL, cswindows1254 }, { "cp1254", cp1254_mbtowc, cp1254_wctomb, NULL, NULL, cswindows1254 },
    { "x-mac-turkish", mac_turkish_mbtowc, mac_turkish_wctomb, NULL, NULL, other },
#endif

#ifdef INTL_VIETNAMESE_SUPPORT
    { "VISCII", viscii_mbtowc, viscii_wctomb, NULL, NULL, csVISCII },
    { "windows-1258", cp1258_mbtowc, cp1258_wctomb, NULL, NULL, cswindows1258 }, { "cp1258", cp1258_mbtowc, cp1258_wctomb, NULL, NULL, cswindows1258 },
#endif
    
    { NULL, NULL, NULL, NULL, NULL, other } // end of list
};

int intlLookupConverterByEncodingString(char *encoding)
{
    int i = 0;
    
    for (i = 0; conversionTable[i].name != NULL; i++) {
        if (!stringCompareCaseless(encoding, conversionTable[i].name))
            return i;
    }
    
    return -1;
}


unsigned long intlConvertStrings(unsigned char *in, char *inEncoding,
                        unsigned long inLen, unsigned char **out, char *outEncoding)
{
    int             to              = -1;
    int             from            = -1;
    int             convFromCharLen = 0;    // The length of the multi-byte character that we are converting from
    int             convToCharLen   = 0;    // The length of the multi-byte character that we are converting to
    unsigned long   fromLeft        = inLen;
    //unsigned long   toLeft          = 0;
    unsigned long   i               = 0;
    unsigned long   j               = 0;
    conv_type       conv;
    
    if (!in || !inEncoding || !outEncoding)
        return 0;
    
    to      = intlLookupConverterByEncodingString(outEncoding);
    from    = intlLookupConverterByEncodingString(inEncoding);
    
    conv.istate = 0;
    conv.ostate = 0;
    
    i = 0;
    
    //toLeft = intlDestLen();

    if ((to < 0) || (from < 0) || !conversionTable[to].wctomb || !conversionTable[from].mbtowc)
        goto intlConvert_Invalid;

    do {
        ucs4_t          c = 0;
        unsigned char   buf[10];
        unsigned char   *newOut;
        //state_t         lastState = conv.istate;
        
        convFromCharLen = conversionTable[from].mbtowc(&conv, &c, in + i, fromLeft);
        if (convFromCharLen < 0) { // TODO: Check the error code
            goto intlConvert_Invalid;
        }
        
        convToCharLen = conversionTable[to].wctomb(&conv, buf, c, 10/*toLeft*/);
        if (convToCharLen < 0) { // TODO: Check the error code
            goto intlConvert_Invalid;
        }
        
        newOut = memMgrChunkRealloc(*out, j + convToCharLen);
        if (!newOut) {
            memMgrChunkFree(*out);
            *out = NULL;
            
            goto intlConvert_Invalid;
        }
        
        memMgrChunkMove(newOut + j, buf, convToCharLen);
        
        i += convFromCharLen;
        j += convToCharLen;
        fromLeft -= convFromCharLen;
        //toLeft -= convToCharLen;
        
        *out = newOut;
    } while (fromLeft > 0);
    
    return j;
    
intlConvert_Invalid:

    return 0;
}

unsigned long intlEncodingToUTF8(unsigned char *in, char *inEncoding,
                                unsigned long inLen, unsigned char **out)
{
    return intlConvertStrings(in, inEncoding, inLen, out, "UTF-8");
}
