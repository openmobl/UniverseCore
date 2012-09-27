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
#include "MemMgr.h"

#include "Errors.h"
//#include <MemGlue.h>

#define memNewChunkFlagAllowLarge			0x1000	// allow >64K allocations

void *MemPtrNewLarge(Int32 size);
SysAppInfoPtr 	SysGetAppInfo(SysAppInfoPtr *uiAppPP, SysAppInfoPtr *actionCodeAppPP)
							SYS_TRAP(sysTrapSysGetAppInfo);

void *MemPtrNewLarge(Int32 size)
{
	// Allocate a chunk of memory where the size of the memory might exceed
	// 64k.  There are a few potential problems here:
	//  (a) the memory cannot be saved to a database
	//  (b) if you forget to free the large chunk before
	//      you application quits, you will cause a MEMORY LEAK on
	//      Palm OS 5 and 5.1 .
	if (size > 65500) {
        SysAppInfoType      *unusedAppInfoP;
		UInt16 currentOwnerID = SysGetAppInfo(&unusedAppInfoP, &unusedAppInfoP)->memOwnerID;
		return MemChunkNew(0, size, currentOwnerID | memNewChunkFlagNonMovable | memNewChunkFlagAllowLarge);
	}
    
    return MemPtrNew(size);
}

void memMgrIncramentSizeCount(unsigned long size, boolean subtract)
{
    /*GlobalsType     *g;
    Err             error = systemErrNone;
    
    if (!size)
        return;
    
    error = FtrGet(wuAppType, ftrGlobals, (UInt32 *)&g);
    
    if ((error == systemErrNone) && g) {
        g->pageStats.totalMemoryUsed += size;
    }*/
}

void *memMgrChunkNew(unsigned long size)
{
    MemPtr          newPtr = NULL;

    if (!size)
        return NULL;
    
    newPtr = MemPtrNewLarge(size);
    
    if (newPtr) {
        memMgrIncramentSizeCount(size, false);
    }
    
    return newPtr;
}

int memMgrChunkFree(void *ptr)
{
    UInt32          ptrSize = 0;
    Err             result  = systemErrNone;
    
    if (!ptr)
        return memoryErrInvalidParam;
    
    ptrSize = memMgrChunkSize(ptr);
    
    result = MemPtrFree(ptr);
    
    if (result == systemErrNone) {
        memMgrIncramentSizeCount(ptrSize, true);
    }
    
    return result;
}

int memMgrChunkMove(void *dest, void *src, unsigned long size)
{
    Err     result  = systemErrNone;
    
    if (!dest || !src || !size)
        return memoryErrInvalidParam;
    
    result = MemMove(dest, src, size);
    
    return result?memoryErrChunkMoveFailed:systemErrNone;
}

int memMgrChunkSet(void *ptr, int val, unsigned long size)
{
    Err     result  = systemErrNone;
    
    if (!ptr || !size)
        return memoryErrInvalidParam;

    result = MemSet(ptr, size, val);
    
    return result?memoryErrChunkSetFailed:systemErrNone;
}

unsigned long memMgrChunkSize(void *ptr)
{
    return MemPtrSize(ptr);
}

int memMgrChunkResize(void *ptr, unsigned long size)
{
    Err             result  = systemErrNone;
    UInt32          ptrSize = 0;
    
    if (!ptr || !size)
        return memoryErrInvalidParam;
    
    ptrSize = memMgrChunkSize(ptr);
        
    result = MemPtrResize(ptr, size);
    
    if (result == systemErrNone) {
        memMgrIncramentSizeCount(ptrSize, true);
        memMgrIncramentSizeCount(size, false);
    }
    
    return result;
}

void *memMgrChunkRealloc(void *ptr, unsigned long size)
{
    MemPtr  local = NULL;

    if (!size) {
        return NULL;
    }

    if (!ptr) {
        return memMgrChunkNew(size);
    }


    if (memMgrChunkResize((MemPtr)ptr, size)) {
        local = memMgrChunkNew(size);
        if (!local) {
            return NULL;
        }

        memMgrChunkMove(local, ptr, MIN(memMgrChunkSize(ptr), size));

        memMgrChunkFree(ptr);

        return local;
    }

    return ptr;
}

void *memMgrChunkCombine(void *ptr1, unsigned long size1, void *ptr2, unsigned long size2)
{
    MemPtr   out = NULL;

    if (!ptr1 || !size1 || !ptr2 || !size2) {
        return NULL;
    }

    out = memMgrChunkRealloc(ptr1, size1 + size2);
    if (!out) {
        return NULL;
    }

    memMgrChunkMove((void *)((unsigned long)out + size1), ptr2, size2);

    return out;
}
