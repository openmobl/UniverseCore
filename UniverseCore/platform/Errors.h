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
#ifndef Errors_h
#define Errors_h

#include "PlatformBytes.h"

/*
 * Error Classes. This is the central point for errors.
 * Please define errClassStart in platform/<platform folder>/include/PalmtformBytes.h
 */
#define systemLevelErrorClass               (0)                     // System is *ALWAYS* 0
#define universeCoreErrorClass              (errClassStart | 0x0100 )
#define loaderErrorClass                    (errClassStart | 0x0200 )
#define httpErrorClass                      (errClassStart | 0x0300 )
#define parserErrorClass                    (errClassStart | 0x0400 )
#define wbxmlErrorClass                     (errClassStart | 0x0500 )
#define memoryErrorClass                    (errClassStart | 0x0600 )
#define networkErrorClass                   (errClassStart | 0x0700 )
#define securityErrorClass                  (errClassStart | 0x0800 )
#define domExceptionClass                   (errClassStart | 0x0900 )
#define urlErrorClass                       (errClassStart | 0x0A00 )
#define fileSysErrorClass                   (errClassStart | 0x0B00 )

#define systemErrNone                       (systemLevelErrorClass |   0)

#define universeCoreErrInvalidPrefField     (unverseCoreErrorClass |   1)

#define loaderErrNoLoader                   (loaderErrorClass |   1)
#define loaderErrInvalidParams              (loaderErrorClass |   2)
#define loaderErrCallbackUndefined          (loaderErrorClass |   3)
#define loaderErrInvalidProtocolState       (loaderErrorClass |   4)
#define loaderErrUnsupportedProtocol        (loaderErrorClass |   5)
#define loaderErrInvalidState               (loaderErrorClass |   6)
#define loaderErrInvalidActionForProtocol   (loaderErrorClass |   7)
#define loaderErrCacheElementExpired        (loaderErrorClass |   8)
#define loaderErrCacheElementDoesNotExist   (loaderErrorClass |   9)

#define httpErrInvalidRequest               (httpErrorClass |   1)
#define httpErrInvalidMethod                (httpErrorClass |   2)
#define httpErrUnsupportedMethod            (httpErrorClass |   3)
#define httpErrParameterError               (httpErrorClass |   4)
#define httpErrGenericRequestError          (httpErrorClass |   5)
#define httpErrNoGlobals                    (httpErrorClass |   6)
#define httpErrStateError                   (httpErrorClass |   7)
#define httpErrInvalidState                 (httpErrorClass |   8)
#define httpErrParseError                   (httpErrorClass |   9)
#define httpErrMalformedHeader              (httpErrorClass |  10)
#define httpErrRemoteClosed                 (httpErrorClass |  11)
#define httpErrRedirectRequested            (httpErrorClass |  12) /* just a "status" */
#define httpErrStatusNotOK                  (httpErrorClass |  13) /* also a "status" */
#define httpErrUserCancel                   (httpErrorClass |  14)
#define httpErrInvalidCallback              (httpErrorClass |  15)
#define httpErrContentHandleError           (httpErrorClass |  16)
#define httpErrMaxRedirectReached           (httpErrorClass |  17) /* uh oh! infinite loop! */
#define httpErrNotOKStatusHandled           (httpErrorClass |  18)
//#define httpErrMalformedURL               (httpErrorClass |  19)

#define parserErrNoParser                   (parserErrorClass |   1)
#define parserErrFormatNotSupported         (parserErrorClass |   2)
#define parserErrGenericParseError          (parserErrorClass |   3)
#define parserErrNoHandler                  (parserErrorClass |   4)
#define parserErrToUTFFailed                (parserErrorClass |   5)

#define memoryErrNoFreeMemory               (memoryErrorClass |   1)
#define memoryErrChunkMoveFailed            (memoryErrorClass |   2)
#define memoryErrChunkSetFailed             (memoryErrorClass |   3)
#define memoryErrInvalidParam               (memoryErrorClass |   4)

#define networkErrSocketNotOpen             (networkErrorClass |   1)
#define networkErrSocketNotCreated          (networkErrorClass |   2)
#define networkErrSocketNotConnected        (networkErrorClass |   3)
#define networkErrHostLookupFailed          (networkErrorClass |   4)
#define networkErrDNSTimedOut               (networkErrorClass |   5)
#define networkErrSendFailed                (networkErrorClass |   6)
#define networkErrReceiveFailed             (networkErrorClass |   7)
#define networkErrSessionNotCreated         (networkErrorClass |   8)
#define networkErrGeneric                   (networkErrorClass |   9)
#define networkErrTimeout                   (networkErrorClass |  10)
#define networkErrNoSession                 (networkErrorClass |  11)

#define urlErrMalformedURL                  (urlErrorClass |   1)
#define urlErrIncompleteURL                 (urlErrorClass |   2) /* This is to indicate that the URL is just a path */
#define urlErrUnsupportedURI                (urlErrorClass |   3)
#define urlErrIsDocumentRelative            (urlErrorClass |   4) /* Sent when a URL that begins with # is parsed */
#define urlErrNullURL                       (urlErrorClass |   5)

#define fileErrFileNotCreated               (fileSysErrorClass |   1)
#define fileErrReadError                    (fileSysErrorClass |   2)

#endif
