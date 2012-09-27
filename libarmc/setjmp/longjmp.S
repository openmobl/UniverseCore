/* longjmp for ARM.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

.global longjmp
.type longjmp,%function
.align 2
longjmp:
	mov	ip, r0		/* save jmp_buf pointer */
	
	movs	r0, r1		/* get the return value in place */
	moveq	r0, #1		/* can't let setjmp() return zero! */

	ldmia     ip!,  {v1-v6, sl, fp, sp, lr}

	mov pc, lr

.size longjmp,.-longjmp

