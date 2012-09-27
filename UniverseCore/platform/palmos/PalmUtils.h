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

#ifndef PalmUtils_h
#define PalmUtils_h

#include <PalmOS.h>

#define	NUL	'\0'

#define ErrAssert(x,y,z)    \
    if (x) {           \
        goto z;  \
    }

//#if CPU_TYPE != CPU_68K && CPU_TYPE != CPU_x86
#if CPU_TYPE == CPU_ARM

#define _NetSwap16(x) \
	((((x) >> 8) & 0xFF) | \
	 (((x) & 0xFF) << 8))

#define _NetSwap32(x) \
	((((x) >> 24) & 0x00FF) | \
	 (((x) >>  8) & 0xFF00) | \
	 (((x) & 0xFF00) <<  8) | \
	 (((x) & 0x00FF) << 24))

#define NetHToNS(x)	_NetSwap16(x)
#define NetHToNL(x)	_NetSwap32(x)
#define NetNToHS(x)	_NetSwap16(x)
#define NetNToHL(x)	_NetSwap32(x)

#endif

#ifdef __cplusplus
extern "C" {
#endif

int palmIsDigit(int c);
int mathPower(int base, int exp);

/*extern void *GetObjectPtr (UInt16 objectID);
extern void DrawCharsToFitWidth (char* s, RectanglePtr r, char c);
extern FieldPtr GetFocusObjectPtr (void);
extern Boolean FldGetFocus(UInt16 objectID);
extern void SetFieldFromStr(const Char *srcP , UInt16 fieldIndex);
extern char *rindex(char *str, char c);
extern void ExitApp(void);
extern Err SetFieldTextFromStr(FieldType *fieldP, Char *string, Boolean redraw);
extern Boolean isHires(void);
extern UInt32 strToInt(Char* strin, UInt16 base);
extern Char *StrToBase64(Char *ostr, Int32 len);
extern void DrawToFit(char *instr, RectangleType rect, GlobalsType *g);
extern UInt16 HiresFactor(void);

extern Char *StrCompareLower(Char *str, const Char *token);*/
//extern Char *StrDup(Char *string);
//extern Char *StrCombine(Char *string1, Char *string2);
/*extern Boolean StrEquals(Char *string1, Char *string2);
extern Char *StrRChr(Char *string, Char token);
extern Char *StrReverse(Char *str);

extern void FldEmulateEnter(UInt16 index);

extern Boolean SysCheck5Way(void);

extern Err NVFSSyncDatabase(DmOpenRef dbRef);
extern Boolean NVFSIsNVFS(void);
extern UInt32 NVFSGetNVFSFreeSize(void);

extern UInt32 GetFreeMemory(UInt32 *totalMemoryP, UInt32 *dynamicMemoryP, Boolean maxChunkSize );*/

#ifdef __cplusplus
}
#endif

#endif
