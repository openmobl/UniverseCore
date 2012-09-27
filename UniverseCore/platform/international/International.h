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
#ifndef International_h
#define International_h

#ifdef __cplusplus
extern "C" {
#endif

#include "InternationalConv.h"

extern unsigned long intlConvertStrings(unsigned char *in, char *inEncoding,
                            unsigned long inLen, unsigned char **out, char *outEncoding);
extern unsigned long intlEncodingToUTF8(unsigned char *in, char *inEncoding,
                            unsigned long inLen, unsigned char **out);

/*
 * Include all the converters.
 */

#include "utf7.h"
#include "utf8.h"
#include "utf16.h"
#include "utf32.h"

#include "ucs2.h"
#include "ucs4.h"

#include "ascii.h"
#include "iso8859_1.h"
#include "iso8859_3.h"
#include "iso8859_15.h"
#include "cp1252.h"
#include "cp437.h"
#include "cp850.h"
#include "mac_iceland.h"
#include "mac_roman.h"

#ifdef INTL_NEXTSTEPLATIN_SUPPORT
#   include "nextstep.h"
#endif

#ifdef INTL_ARABIC_SUPPORT
#   include "iso8859_6.h"
#   include "cp1256.h"
#   include "cp864.h"
#   include "mac_arabic.h"
#endif

#ifdef INTL_ROMAN_SUPPORT
#   include "iso8859_2.h"
#   include "iso8859_4.h"
#   include "cp1250.h"
#   include "cp1257.h"
#   include "mac_centraleurope.h"
#   include "mac_croatian.h"
#   include "mac_romania.h"
#endif

#ifdef INTL_CHINESE_SUPPORT
#   include "gb2312.h"
#   include "euc_cn.h"
#   include "gbk.h"
//#   include "iso2022_cn.h"
#   include "hz.h"
//#   include "cns11643_1.h"
//#   include "cns11643_2.h"
//#   include "euc_tw.h"
#   include "big5.h"
#   include "big5_2003.h"
#endif

#ifdef INTL_CYRILLIC_SUPPORT
#   include "iso8859_5.h"
#   include "koi8_r.h"
#   include "cp1251.h"
#   include "cp866.h"
#   include "mac_cyrillic.h"
#   include "mac_ukraine.h"
#   include "iso8859_7.h"
#   include "cp1253.h"
#   include "mac_greek.h"
#endif

#ifdef INTL_HEBREW_SUPPORT
#   include "iso8859_8.h"
#   include "cp1255.h"
#   include "mac_hebrew.h"
#endif

#ifdef INTL_JAPANESE_SUPPORT
#   include "jisx0201.h"
#   include "jisx0208.h"
#   include "jisx0212.h"
#   include "euc_jp.h"
#   include "iso2022_jp.h"
#   include "sjis.h"
#endif

#ifdef INTL_KOREAN_SUPPORT
#   include "ksc5601.h"
#   include "euc_kr.h"
#   include "iso2022_kr.h"
#endif

#ifdef INTL_THAI_SUPPORT
#   include "cp874.h"
#   include "mac_thai.h"
#endif

#ifdef INTL_TURKISH_SUPPORT
#   include "iso8859_9.h"
#   include "cp1254.h"
#   include "mac_turkish.h"
#endif

#ifdef INTL_VIETNAMESE_SUPPORT
#   include "viscii.h"
#   include "cp1258.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
