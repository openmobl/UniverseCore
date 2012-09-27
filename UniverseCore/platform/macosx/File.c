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

#include "MacOSXConfig.h"
#include "File.h"

#include <ftw.h>

FILE *fileOpen(const char *fileName, const char *mode)
{
    return fopen(fileName, mode);
}

int fileSeek(FILE *stream, unsigned long offset, unsigned long whence)
{
    return fseek(stream, offset, whence);
}

long fileTell(FILE *stream)
{
    return ftell(stream);
}

unsigned long fileRead(void *ptr, unsigned long size, unsigned long n, FILE *stream)
{
    return fread(ptr, size, n, stream);
}

unsigned long fileWrite(void *ptr, unsigned long size, unsigned long n, FILE *stream)
{
    return fwrite(ptr, size, n, stream);
}

unsigned long fileSize(FILE *stream, char *name)
{
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
}

int fileDelete(char *name)
{
    return remove(name);
}

static int ftwCallback(const char *name, const struct stat *status, int info)
{
    if (info == FTW_F) {
        fileDelete((char *)name);
    }
    
    return 0;
}

int fileFlushDirectory(char *directory)
{
   return ftw(directory, ftwCallback, 5);
}

void fileClose(FILE *f)
{
    fclose(f);
}
