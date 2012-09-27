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
 
#ifndef File_h
#define File_h

#include "PalmOSConfig.h"
#include <VFSMgr.h>

#ifdef USE_FILE_API
#define SEEK_SET    fileOriginBeginning
#define SEEK_CUR    fileOriginCurrent
#define SEEK_END    fileOriginEnd
#else
#define SEEK_SET    vfsOriginBeginning
#define SEEK_CUR    vfsOriginCurrent
#define SEEK_END    vfsOriginEnd
#endif

typedef struct FILE {
#ifdef USE_FILE_API
    FileHand    fileHand;
#else
    UInt16      volRefNum;
    FileRef     fileRef;
#endif
} FILE;

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *fileOpen(const char *fileName, const char *mode);
extern int fileSeek(FILE *stream, unsigned long offset, unsigned long whence);
extern long fileTell(FILE *stream);
extern unsigned long fileRead(void *ptr, unsigned long size, unsigned long n, FILE *stream);
extern unsigned long fileWrite(void *ptr, unsigned long size, unsigned long n, FILE *stream);
extern unsigned long fileSize(FILE *stream, char *name);
extern int fileDelete(char *name);
extern int fileFlushDirectory(char *directory);
extern void fileClose(FILE *f);
extern unsigned long filePrintF(FILE *stream, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
