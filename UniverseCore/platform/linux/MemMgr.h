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
 
#ifndef MemMgr_h
#define MemMgr_h

#include "LinuxConfig.h"

extern void *memMgrChunkNew(unsigned long size);
extern int memMgrChunkFree(void *ptr);
extern int memMgrChunkMove(void *dest, void *src, unsigned long size);
extern int memMgrChunkSet(void *ptr, int val, unsigned long size);
extern unsigned long memMgrChunkSize(void *ptr);
extern int memMgrChunkResize(void *ptr, unsigned long size);
extern void *memMgrChunkRealloc(void *ptr, unsigned long size);
extern void *memMgrChunkCombine(void *ptr1, unsigned long size1, void *ptr2, unsigned long size2);

extern void memMgrIncramentSizeCount(unsigned long size);

#endif
