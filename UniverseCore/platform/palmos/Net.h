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
#ifndef Net_h
#define Net_h

#include "PalmOSConfig.h"

typedef struct NetSessionType {
    UInt32          timeout;
    UInt16          refNum;
    UInt16          error;
    NetSocketRef    socket;
} NetSessionType;

typedef NetSessionType *NetSessionPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern char *netLookupError(int error);
extern void netDisplayError(char *msg, short err, char *file, short line);

extern int netErrorTranslate(int err);

extern int netSetError(NetSessionPtr netInfo, int error);
extern int netGetError(NetSessionPtr netInfo);

extern NetSessionType *netOpenNetwork(NetSessionPtr netInfo);
extern int netCloseNetwork(NetSessionPtr *netInfo, boolean disconnect);
extern int netRefreshNetwork(NetSessionPtr netInfo);
extern boolean netIsConnected(NetSessionPtr netInfo);
extern int netConnectionEstablish(NetSessionPtr netInfo, char *host, char *service, short port);
extern void netConnectionClose(NetSessionPtr netInfo);
extern unsigned long netGetTimeout(NetSessionPtr netInfo);
extern void netSetTimeout(NetSessionPtr netInfo, unsigned long timeout);
extern long netSendData(NetSessionPtr netInfo, void *data, unsigned long size, int *err);
extern long netReceiveData(NetSessionPtr netInfo, void *buffer, unsigned long buffSize, int *err);

#ifdef __cplusplus
}
#endif

#endif
