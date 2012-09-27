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
 
#include "MacOSXConfig.h"
#include "Net.h"

#include <arpa/inet.h>
#include "Debug.h"
#include "Error.h"
#include "MemMgr.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct {
    unsigned short  number;
    char            *error;
} NetError;

/* TODO: Complete me */
static NetError errorList[] =
{
    { 0,                                    NULL                                }
};


char *netLookupError(int error)
{
    int i = 0;

    while (errorList[i].number && (errorList[i].number != error)) {
        i++;
    }

    return errorList[i].error;
}

void netDisplayError(char *msg, short err, char *file, short line)
{

}

int netSetError(NetSessionPtr netInfo, int error)
{
    if (!netInfo)
        return networkErrNoSession;
    
    netInfo->error = error;

    return error;
}

int netGetError(NetSessionPtr netInfo)
{
    return (netInfo)?netInfo->error:networkErrNoSession;
}

NetSessionType *netOpenNetwork(NetSessionPtr netInfo)
{
    NetSessionPtr   session = netInfo;
    
    if (!session) { // The session does not exist, we need to create it.
        session = (NetSessionType *)memMgrChunkNew(sizeof(NetSessionType));
        if (!session)
            return NULL;
        
        memMgrChunkSet(session, 0, sizeof(NetSessionType));
    }

    return session;
}

int netCloseNetwork(NetSessionPtr *netInfo, boolean disconnect)
{
    int error = 0;
    
    memMgrChunkFree(*netInfo);
    *netInfo = NULL;
    
    return error;
}

int netRefreshNetwork(NetSessionPtr netInfo)
{
    return true;
}

boolean netIsConnected(NetSessionPtr netInfo)
{
    return true;
}

int netConnectionEstablish(NetSessionPtr netInfo, char *host, char *service, short port)
{
    int                 sock    = 0;
    int                 result  = 0;
    struct sockaddr_in  sockAddr;
    struct hostent      *hostInfo = NULL;
    struct in_addr      *addressP = NULL;
    struct in_addr      address;

    if (!netInfo)
        return networkErrNoSession;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        netInfo->socket = -1;
        return netSetError(netInfo, networkErrSocketNotCreated);
    }

    bzero(&sockAddr, sizeof(sockAddr));

    if (inet_aton(host, &address)) {
        goto netConnectionEstablish_haveAddress;
    }

    hostInfo = gethostbyname(host);
    if (hostInfo == NULL) {
        return netSetError(netInfo, networkErrHostLookupFailed);
    }

    addressP = (struct in_addr *)hostInfo->h_addr_list[0];
    //address = ntohl(*addressP); // Needed? Proably, yes. FIXME!!
    
    memMgrChunkMove(&address, addressP, sizeof(address));

netConnectionEstablish_haveAddress:
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port   = htons(port);
    memMgrChunkMove(&sockAddr.sin_addr, &address, sizeof(address));
    
    result = connect(sock, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    if (result == -1) {
        shutdown(sock, 2);
        close(sock);
        
        netInfo->socket = -1;
        
        return netSetError(netInfo, networkErrSocketNotConnected);
    }
    
    netInfo->socket = sock;
    
    return systemErrNone;
}

void netConnectionClose(NetSessionPtr netInfo)
{
    if (!netInfo)
        return;
    
    //shutdown(netInfo->socket, );
    close(netInfo->socket);
}

unsigned long netGetTimeout(NetSessionPtr netInfo)
{
    return (netInfo)?netInfo->timeout:0;
}

void netSetTimeout(NetSessionPtr netInfo, unsigned long timeout)
{
    if (!netInfo)
        return;
    
    netInfo->timeout = timeout;
}

long netSendData(NetSessionPtr netInfo, void *data, unsigned long size, int *err)
{
    long   result = 0;
    
    if (!netInfo) {
        *err = networkErrNoSession;
        
        return -1;
    }

    result = send(netInfo->socket, data, size, 0);
    
    if (result == -1)
        *err = networkErrSendFailed;
    else
        *err = systemErrNone;
    
    netSetError(netInfo, *err);
    
    return result;
}

long netReceiveData(NetSessionPtr netInfo, void *buffer, unsigned long buffSize, int *err)
{
    long   result = 0;
    
    if (!netInfo) {
        *err = networkErrNoSession;
        
        return -1;
    }

    memMgrChunkSet(buffer, 0, buffSize); // Zero out

    result = recv(netInfo->socket, buffer, buffSize, 0);
    
    if (result == -1)
        *err = networkErrReceiveFailed;
    else
        *err = systemErrNone;

    netSetError(netInfo, *err);
    
    return result;
}

