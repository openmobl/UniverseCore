#include <string.h>

size_t strlen(const char *_s)
{
    size_t  len = 0;
    
    while (*(_s++)) {
        len++;
    }
    
    return len;
}

#if 0
#pragma thumb off

asm size_t strlen (const char *_s)
{
    MOV  R1,R0

lbl:
    LDRB R2, [R0], #1
    CMP  R2, #0
    BNE  lbl
    SUB  R0, R0, R1
    BX   LR
}
#endif

#pragma thumb reset

#if 0
/* Copyright (C) 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Code contributed by Matthew Wilcox <willy@odie.barnet.ac.uk>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* size_t strlen(const char *S)
 * entry: r0 -> string
 * exit: r0 = len
 */

asm void strlen()
{
	bic     r1, r0, $3              @ addr of word containing first byte
	ldr     r2, [r1], $4            @ get the first word
	ands    r3, r0, $3              @ how many bytes are duff?
	rsb     r0, r3, $0              @ get - that number into counter.
	beq     Laligned                @ skip into main check routine if no
					@ more
	orr     r2, r2, $0x000000ff     @ set this byte to non-zero
	subs    r3, r3, $1              @ any more to do?
	orrgt   r2, r2, $0x0000ff00     @ if so, set this byte
	subs    r3, r3, $1              @ more?
	orrgt   r2, r2, $0x00ff0000     @ then set.
Laligned:				@ here, we have a word in r2.  Does it
	tst     r2, $0x000000ff         @ contain any zeroes?
	tstne   r2, $0x0000ff00         @
	tstne   r2, $0x00ff0000         @
	tstne   r2, $0xff000000         @
	addne   r0, r0, $4              @ if not, the string is 4 bytes longer
	ldrne   r2, [r1], $4            @ and we continue to the next word
	bne     Laligned                @
Llastword:				@ drop through to here once we find a
	tst     r2, $0x000000ff         @
	addne   r0, r0, $1              @
	tstne   r2, $0x0000ff00         @ and add up to 3 bytes on to it
	addne   r0, r0, $1              @
	tstne   r2, $0x00ff0000         @ (if first three all non-zero, 4th
	addne   r0, r0, $1              @  must be zero)
	mov	pc,lr
}
#endif
