#include <string.h>

int strncmp(const char *_s1, const char *_s2, size_t _n)
{
    if (_n == 0) {
        return 0;
    }
    
    while (_n-- && (*_s1 == *_s2)) {
        if (!_n || !*_s1) {
            break;
        }
        
        *_s1++;
        *_s2++;
    }
    
    return ((*(unsigned char *)_s1) - (*(unsigned char *)_s2));
}

#if 0
/*
 * Copyright (c) 2002 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Adapted for uClibc from NetBSD strncmp.S, version 1.2 2003/04/05
 * by Erik Andersen <andersen@codepoet.org>
 */

asm void strncmp()
{
	/* if (len == 0) return 0 */
	cmp	r2, #0
	moveq	r0, #0
	moveq	pc, lr
	subs	r2, r2, #1

	/* ip == last src address to compare */
	add	ip, r0, r2
1:
	ldrb	r2, [r0], #1
	ldrb	r3, [r1], #1
	cmp	ip, r0
	cmpcs	r2, #1
	cmpcs	r2, r3
	beq	1b
	sub	r0, r2, r3
	mov	pc, lr
}
#endif
