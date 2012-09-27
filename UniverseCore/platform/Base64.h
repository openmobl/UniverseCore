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
/*

       Encode or decode file as MIME base64 (RFC 1341)

                by John Walker
               http://www.fourmilab.ch/

        This program is in the public domain.

*/

#ifndef Base64_h
#define Base64_h

struct base64BufferType {
  char  *data;
  int   length;
  char  *ptr;
  int   offset;
};

#ifdef __cplusplus
extern "C" {
#endif

void base64BufferNew(struct base64BufferType *b);
void base64BufferAdd(struct base64BufferType *b, char c);
void base64BufferDelete(struct base64BufferType *b);

void base64Encode(struct base64BufferType *b, const char *source, int length);

#ifdef __cplusplus
}
#endif

/*
void base64_decode(struct base64BufferType *b, const char *source, int length);
*/

/*
#define DEBUG_MALLOC
 */

/*
#ifdef DEBUG_MALLOC
void *_malloc_real(size_t s, char *file, int line);
void _free_real(void *p, char *file, int line);

#define malloc(s)    _malloc_real(s,__FILE__,__LINE__)
#define free(p)        _free_real(p, __FILE__,__LINE__)
#endif
*/

#endif
