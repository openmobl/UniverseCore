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
#ifndef InternationalConv_h
#define InternationalConv_h

#include "config.h"

typedef unsigned int ucs4_t;
typedef unsigned int state_t;
typedef struct conv_type {
    state_t istate;
    state_t ostate;
} conv_type;
typedef conv_type *conv_t;

typedef struct {
  unsigned short indx; /* index into big table */
  unsigned short used; /* bitmask of used entries */
} Summary16;

/* This is where all "too few" errors start */
#define RET_TOO_FEW_OFF 10
/* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ      -1
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)  (-10-(n))
/* Return code if invalid. (xxx_wctomb) */
#define RET_ILUNI      -1
/* Return code if output buffer is too small. (xxx_wctomb, xxx_reset) */
#define RET_TOOSMALL   -2
/* Replaces abort() for systems that do not support it. */
#define SIG_ABRT        -5

#endif
