/*
 * Copyright (C) 1999-2004 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
 * ISO-8859-7
 */

#ifndef iso8859_7_h
#define iso8859_7_h

#include "InternationalConv.h"

extern int iso8859_7_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, int n);
extern int iso8859_7_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, int n);

#endif
