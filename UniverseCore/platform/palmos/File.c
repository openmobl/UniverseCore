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

#include "PalmOSConfig.h"
#include "File.h"

#include "MemMgr.h"
#include <unix_stdarg.h>

#define DEFAULT_BUF_SIZE    (4096 * 2)

static void _fileCreateDirectory(const char *fileName, unsigned short volRefNum);

FILE *fileOpen(const char *fileName, const char *mode)
{
    Err     error       = errNone;
    FILE    *fileOut    = NULL;
    Boolean rw          = 0;
    UInt32  openMode    = 0;
#ifndef USE_FILE_API
    UInt32  volIterator = vfsIteratorStart;
#endif

    if (!fileName || !mode)
        return NULL;
    
    fileOut = (FILE *)memMgrChunkNew(sizeof(FILE));
    if (!fileOut)
        return NULL;
        
    rw = ((mode[1] == '+') || (mode[1] && (mode[2] == '+')));

#ifdef USE_FILE_API
    switch (mode[0]) {
        case 'r':
            openMode = (rw)?fileModeReadWrite:fileModeReadOnly;
            break;
        case 'w':
            openMode = fileModeReadWrite;
            break;
        default:
            goto fileOpen_Error;
            break;
    }
    
    fileOut->fileHand = FileOpen(0, fileName, 0, 0, openMode, &error);
#else
    switch (mode[0]) {
        case 'r':
            openMode = (rw)?vfsModeReadWrite:vfsModeRead;
            break;
        case 'w':
            openMode = vfsModeReadWrite;
            break;
        default:
            goto fileOpen_Error;
            break;
    }
    
    error = VFSVolumeEnumerate(&fileOut->volRefNum, &volIterator);
    if (error) {
            goto fileOpen_Error;
    }
    
    error = VFSFileOpen(fileOut->volRefNum, fileName, openMode, &fileOut->fileRef);
    if (error == vfsErrFileNotFound) {
        _fileCreateDirectory(fileName, fileOut->volRefNum);
        error = VFSFileCreate(fileOut->volRefNum, fileName);
        if (error) {
            goto fileOpen_Error;
        }
        
        error = VFSFileOpen(fileOut->volRefNum, fileName, openMode, &fileOut->fileRef);
        if (error) {
            goto fileOpen_Error;
        }
    } else if (error) {
        goto fileOpen_Error;
    }
#endif
    
    return fileOut;
    
fileOpen_Error:
    if (fileOut)
        memMgrChunkFree(fileOut);
        
    return NULL;
}

static void _fileCreateDirectory(const char *fileName, unsigned short volRefNum)
{
#ifndef USE_FILE_API
    int i = 0;
    
    for (i = 0; i < stringLen(fileName); i++) {
        if (fileName[i] == '/') {
            char *tempPath = stringDup(fileName);

            if (tempPath) {
                FileRef fileRef;
                
                tempPath[i + 1] = 0;
                
                // Try to open the path, if that fails we need to create it
                if (!VFSFileOpen(volRefNum, tempPath, vfsModeRead, &fileRef)) {
                    VFSFileClose(fileRef);
                } else {
                    VFSDirCreate(volRefNum, tempPath);
                }

                memMgrChunkFree(tempPath);
            } else { // To bad, we'll just fail for now
                break;
            }
        }
    }
#endif
}

int fileSeek(FILE *stream, unsigned long offset, unsigned long whence)
{
    Err     error = errNone;

#ifdef USE_FILE_API
    if (!stream || !stream->fileHand)
        return -1;
        
    error = FileSeek(stream->fileHand, offset, whence);
#else
    if (!stream || !stream->fileRef)
        return -1;
        
    error = VFSFileSeek(stream->fileRef, whence, offset);
#endif
    return (error)?-1:0;
}

long fileTell(FILE *stream)
{
    Err     error   = errNone;
    UInt32  pos     = 0;

#ifdef USE_FILE_API
    if (!stream || !stream->fileHand)
        return -1;
        
    pos = FileTell(stream->fileHand, NULL, &error);
#else
    if (!stream || !stream->fileRef)
        return -1;
        
    error = VFSFileTell(stream->fileRef, &pos);
#endif

    return (error)?-1:pos;
}

unsigned long fileRead(void *ptr, unsigned long size, unsigned long n, FILE *stream)
{
    Err     error       = errNone;
#ifndef USE_FILE_API
    UInt32  bytesRead   = 0;

    if (!stream || !stream->fileRef || !ptr || !size || !n)
        return 0;

    error = VFSFileRead(stream->fileRef, size * n, ptr, &bytesRead);

    return (bytesRead / size);
#else
    if (!stream || !stream->fileHand || !ptr || !size || !n)
        return 0;
        
    return FileRead(stream->fileHand, ptr, size, n, &error);
#endif
}

unsigned long fileWrite(void *ptr, unsigned long size, unsigned long n, FILE *stream)
{
    Err     error           = errNone;
#ifndef USE_FILE_API
    UInt32  bytesWritten    = 0;
    
    if (!stream || !stream->fileRef || !ptr || !size || !n)
        return 0;
    
    error = VFSFileWrite(stream->fileRef, size * n, ptr, &bytesWritten);
    
    return (bytesWritten / n);
#else
    if (!stream || !stream->fileHand || !ptr || !size || !n)
        return 0;
    
    return FileWrite(stream->fileHand, ptr, size, n, &error);
#endif
}

unsigned long fileSize(FILE *stream, char *name)
{
#ifdef USE_FILE_API
    long    n = 0;
    FILE    *f = stream;
    
    if (!f) {
        if (!name)
            return 0;
        
        f = fileOpen(name, "r");
        if (!f)
            return 0;
    }
    
    fileSeek(f, 0, SEEK_END);
    n = fileTell(f);
    
    if (!stream) {
        fileClose(f);
    } else {
        fileSeek(f, 0, SEEK_SET);
    }
    
    return n;
#else
    FILE    *f      = stream;
    Err     error   = errNone;
    UInt32  size    = 0;
    
    if (!f || !f->fileRef)
        return 0;
        
    error = VFSFileSize(f->fileRef, &size);
    
    return (error)?-1:size;
#endif
}

int fileDelete(char *name)
{
#ifdef USE_FILE_API
    Err     error       = errNone;
    
    error = FileDelete(0, name);
#else
    UInt32  volIterator = vfsIteratorStart;
    UInt16  volRefNum   = 0;
    Err     error       = errNone;
    
    error = VFSVolumeEnumerate(&volRefNum, &volIterator);
    if (error) {
            return -1;
    }
    
    error = VFSFileDelete(volRefNum, name);
#endif

    return 0;
}

int fileFlushDirectory(char *directory)
{
#ifndef USE_FILE_API
    FileRef         dirRefNum;
    Err             error           = errNone;
    UInt32          volIterator     = vfsIteratorStart;
    UInt32          dirIterator     = vfsIteratorStart;
    UInt16          volRefNum       = 0;
    FileInfoType    info;
    char            *fileName       = memMgrChunkNew(1024);
    
    if (!fileName || !directory)
        goto fileEmptyDir_Error;
    
    error = VFSVolumeEnumerate(&volRefNum, &volIterator);
    if (error) {
        goto fileEmptyDir_Error;
    }
    
    error = VFSFileOpen(volRefNum, directory, vfsModeRead, &dirRefNum);
    if (error) {
        goto fileEmptyDir_Error;
    }
    
    info.nameP      = fileName;
    info.nameBufLen = 1024;
    
    while ((dirIterator != vfsIteratorStop) && !error) {
        error = VFSDirEntryEnumerate(dirRefNum, &dirIterator, &info);
        if (error)
            goto fileEmptyDir_Error;
        
        if (!(info.attributes & vfsFileAttrDirectory)) {
            error = VFSFileDelete(volRefNum, info.nameP); // Ignore errors
        }
    }
    
    memMgrChunkFree(fileName);
    
    return 0;

fileEmptyDir_Error:
    if (fileName)
        memMgrChunkFree(fileName);
        
    return -1;
#endif
}

void fileClose(FILE *f)
{
    if (!f)
        return;

#ifdef USE_FILE_API
    if (f->fileHand) {
        FileClose(f->fileHand);
        f->fileHand = NULL;
    }
#else
    if (f->fileRef) {
        VFSFileClose(f->fileRef);
    }
#endif
    
    memMgrChunkFree(f);
}

unsigned long filePrintF(FILE *stream, const char *format, ...)
{
    va_list args;
    Char    *text; // [0x100];
    Int16   ret             = -1;
    UInt32  bytesWritten    = 0;
    Err     err             = errNone;
    
#ifdef USE_FILE_API
    if (!stream || !stream->fileHand)
        return 0;
#else
    if (!stream || !stream->fileRef)
        return 0;
#endif
    
    text = memMgrChunkNew(DEFAULT_BUF_SIZE);
    if (!text) {
        return -1;
    }
    
    va_start(args, format);
    
    ret = StrVPrintF(text, format, args);
    
    va_end(args);
    
    if (ret <= 0)
        return ret;
    
    do {
#ifdef USE_FILE_API
        bytesWritten = FileWrite(stream->fileHand, text + bytesWritten,
                                 1, ret - bytesWritten, &err);
#else
        err = VFSFileWrite(stream->fileRef, ret - bytesWritten,
                           text + bytesWritten, &bytesWritten);
#endif
    } while ((bytesWritten < ret) && (err == errNone));
    
    /*
        This gets really ugly. Basically we return -1 if StrVPrintF does so,
        or return bytes written.
    */
    if (ret < 0) {
        ret = -1;
    } else if ((err != errNone) || (bytesWritten != ret)) {
        ret = bytesWritten;
    }
    
    memMgrChunkFree(text);
    
    //MyPutS(text);
    return (Int32)ret; 
}
