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
 
#include "PalmOSConfig.h"
#include "Net.h"

#include "Debug.h"
#include "Error.h"
#include "MemMgr.h"
#include <NetMgr.h>
#include <SslLib.h>
#if 0
#include <NetMgrARM.h>
#include <SslLibARM.h>
#endif


typedef struct {
    unsigned short  number;
    char            *error;
    int             internalErr;
} NetError;

/* TODO: Complete me */
static const NetError errorList[] =
{
    { netErrAlreadyOpen,                    "Network (library) already open"    , networkErrGeneric },
    { netErrNotOpen,                        "Network (library) not open"        , networkErrGeneric },
    { netErrStillOpen,                      "Network (library) still open"      , networkErrGeneric },
    { netErrParamErr,                       "Parameter error"                   , networkErrGeneric },
    { netErrNoMoreSockets,                  "No more available sockets"         , networkErrGeneric },
    { netErrOutOfResources,                 "Out of resources"                  , networkErrGeneric },
    { netErrOutOfMemory,                    "Out of memory"                     , networkErrGeneric },
    { netErrSocketNotOpen,                  "Socket not open"                   , networkErrGeneric },
    { netErrSocketBusy,                     "Socket busy"                       , networkErrGeneric },
    { netErrMessageTooBig,                  "Message too big"                   , networkErrGeneric },
    { netErrSocketNotConnected,             "Socket not connected"              , networkErrGeneric },
    { netErrNoInterfaces,                   "No available interfaces"           , networkErrGeneric },
    { netErrBufTooSmall,                    "Buffer too small"                  , networkErrGeneric },
    { netErrUnimplemented,                  "Unimplemented"                     , networkErrGeneric },
    { netErrPortInUse,                      "Specified port in use"             , networkErrGeneric },
    { netErrQuietTimeNotElapsed,            "Quite time not elapsed"            , networkErrGeneric },
    { netErrInternal,                       "Internal error"                    , networkErrGeneric },
    { netErrTimeout,                        "Transaction timed out"             , networkErrGeneric },
    { netErrSocketAlreadyConnected,         "Socket already connected"          , networkErrGeneric },
    { netErrSocketClosedByRemote,           "Socket closed by remote host"      , networkErrGeneric },
    { netErrOutOfCmdBlocks,                 "Out of command blocks"             , networkErrGeneric },
    { netErrWrongSocketType,                "Wrong socket type"                 , networkErrGeneric },
    { netErrSocketNotListening,             "Socket not listening"              , networkErrGeneric },
    { netErrUnknownSetting,                 "Unknown setting"                   , networkErrGeneric },
    { netErrInvalidSettingSize,             "Invalid setting size"              , networkErrGeneric },
    { netErrPrefNotFound,                   "Preference not found"              , networkErrGeneric },
    { netErrInvalidInterface,               "Invalid interface"                 , networkErrGeneric },
    { netErrInterfaceNotFound,              "Interface not found"               , networkErrGeneric },
    { netErrTooManyInterfaces,              "Too many interfaces"               , networkErrGeneric },
    { netErrBufWrongSize,                   "Wrong buffer size"                 , networkErrGeneric },
    { netErrUserCancel,                     "User canceled"                     , networkErrGeneric },
    { netErrBadScript,                      NULL                                , networkErrGeneric },
    { netErrNoSocket,                       NULL                                , networkErrGeneric },
    { netErrSocketRcvBufFull,               NULL                                , networkErrGeneric },
    { netErrNoPendingConnect,               NULL                                , networkErrGeneric },
    { netErrUnexpectedCmd,                  NULL                                , networkErrGeneric },
    { netErrNoTCB,                          NULL                                , networkErrGeneric },
    { netErrNilRemoteWindowSize,            NULL                                , networkErrGeneric },
    { netErrNoTimerProc,                    NULL                                , networkErrGeneric },
    { netErrSocketInputShutdown,            NULL                                , networkErrGeneric },
    { netErrCmdBlockNotCheckedOut,          NULL                                , networkErrGeneric },
    { netErrCmdNotDone,                     NULL                                , networkErrGeneric },
    { netErrUnknownProtocol,                NULL                                , networkErrGeneric },
    { netErrUnknownService,                 NULL                                , networkErrGeneric },
    { netErrUnreachableDest,                NULL                                , networkErrGeneric },
    { netErrReadOnlySetting,                NULL                                , networkErrGeneric },
    { netErrWouldBlock,                     NULL                                , networkErrGeneric },
    { netErrAlreadyInProgress,              NULL                                , networkErrGeneric },
    { netErrPPPTimeout,                     NULL                                , networkErrGeneric },
    { netErrPPPBroughtDown,                 NULL                                , networkErrGeneric },
    { netErrAuthFailure,                    NULL                                , networkErrGeneric },
    { netErrPPPAddressRefused,              NULL                                , networkErrGeneric },
    { netErrDNSNameTooLong,                 "Host name too long"                , networkErrGeneric },
    { netErrDNSBadName,                     "Bad host name"                     , networkErrGeneric },
    { netErrDNSBadArgs,                     "Bad args (DNS)"                    , networkErrGeneric },
    { netErrDNSLabelTooLong,                NULL                                , networkErrGeneric },
    { netErrDNSAllocationFailure,           NULL                                , networkErrGeneric },
    { netErrDNSTimeout,                     "Host name lookup timeout"          , networkErrGeneric },
    { netErrDNSUnreachable,                 "DNS server unreachable"            , networkErrGeneric },
    { netErrDNSFormat,                      NULL                                , networkErrGeneric },
    { netErrDNSServerFailure,               NULL                                , networkErrGeneric },
    { netErrDNSNonexistantName,             "Host name does not exist"          , networkErrGeneric },
    { netErrDNSNIY,                         NULL                                , networkErrGeneric },
    { netErrDNSRefused,                     NULL                                , networkErrGeneric },
    { netErrDNSImpossible,                  NULL                                , networkErrGeneric },
    { netErrDNSNoRRS,                       NULL                                , networkErrGeneric },
    { netErrDNSAborted,                     "Host name lookup aborted"          , networkErrGeneric },
    { netErrDNSBadProtocol,                 NULL                                , networkErrGeneric },
    { netErrDNSTruncated,                   NULL                                , networkErrGeneric },
    { netErrDNSNoRecursion,                 NULL                                , networkErrGeneric },
    { netErrDNSIrrelevant,                  NULL                                , networkErrGeneric },
    { netErrDNSNotInLocalCache,             NULL                                , networkErrGeneric },
    { netErrDNSNoPort,                      NULL                                , networkErrGeneric },
    { netErrIPCantFragment,                 NULL                                , networkErrGeneric },
    { netErrIPNoRoute,                      NULL                                , networkErrGeneric },
    { netErrIPNoSrc,                        NULL                                , networkErrGeneric },
    { netErrIPNoDst,                        NULL                                , networkErrGeneric },
    { netErrIPktOverflow,                   NULL                                , networkErrGeneric },
    { netErrTooManyTCPConnections,          "Too many connections"              , networkErrGeneric },
    { netErrNoDNSServers,                   "No configured DNS servers"         , networkErrGeneric },
    { netErrInterfaceDown,                  "Interface down"                    , networkErrGeneric },
    { netErrDeviceNotReady,                 NULL                                , networkErrGeneric },
    { sslErrOk,                             NULL                                , networkErrGeneric },
    { sslErrFailed,                         NULL                                , networkErrGeneric },
    { sslErrEof,                            NULL                                , networkErrGeneric },
    { sslErrOutOfMemory,                    NULL                                , networkErrGeneric },
    { sslErrCbAbort,                        NULL                                , networkErrGeneric },
    { sslErrIo,                             NULL                                , networkErrGeneric },
    { sslErrNotFound,                       NULL                                , networkErrGeneric },
    { sslErrDivByZero,                      NULL                                , networkErrGeneric },
    { sslErrNoModInverse,                   NULL                                , networkErrGeneric },
    { sslErrBadDecode,                      NULL                                , networkErrGeneric },
    { sslErrInitNotCalled,                  NULL                                , networkErrGeneric },
    { sslErrBufferTooSmall,                 NULL                                , networkErrGeneric },
    { sslErrNullArg,                        NULL                                , networkErrGeneric },
    { sslErrBadLength,                      NULL                                , networkErrGeneric },
    { sslErrNoDmem,                         NULL                                , networkErrGeneric },
    { sslErrNoMethodSet,                    NULL                                , networkErrGeneric },
    { sslErrNoRandom,                       NULL                                , networkErrGeneric },
    { sslErrBadArgument,                    NULL                                , networkErrGeneric },
    { sslErrBadOption,                      NULL                                , networkErrGeneric },
    { sslErrNotImplemented,                 NULL                                , networkErrGeneric },
    { sslErrReallocStaticData,              NULL                                , networkErrGeneric },
    { sslErrInternalError,                  NULL                                , networkErrGeneric },
    { sslErrRecordError,                    NULL                                , networkErrGeneric },
    { sslErrCsp,                            NULL                                , networkErrGeneric },
    { sslErrCert,                           NULL                                , networkErrGeneric },
    { sslErrHandshakeEncoding,              NULL                                , networkErrGeneric },
    { sslErrMissingProvider,                NULL                                , networkErrGeneric },
    { sslErrHandshakeProtocol,              NULL                                , networkErrGeneric },
    { sslErrExtraHandshakeData,             NULL                                , networkErrGeneric },
    { sslErrWrongMessage,                   NULL                                , networkErrGeneric },
    { sslErrFatalAlert,                     NULL                                , networkErrGeneric },
    { sslErrBadPeerFinished,                NULL                                , networkErrGeneric },
    { sslErrBadSignature,                   NULL                                , networkErrGeneric },
    { sslErrUnexpectedRecord,               NULL                                , networkErrGeneric },
    { sslErrReadAppData,                    NULL                                , networkErrGeneric },
    { sslErrCertDecodeError,                NULL                                , networkErrGeneric },
    { sslErrUnsupportedCertType,            NULL                                , networkErrGeneric },
    { sslErrUnsupportedSignatureType,       NULL                                , networkErrGeneric },
    { sslErrVerifyBadSignature,             NULL                                , networkErrGeneric },
    { sslErrVerifyNoTrustedRoot,            NULL                                , networkErrGeneric },
    { sslErrVerifyNotAfter,                 NULL                                , networkErrGeneric },
    { sslErrVerifyNotBefore,                NULL                                , networkErrGeneric },
    { sslErrVerifyConstraintViolation,      NULL                                , networkErrGeneric },
    { sslErrVerifyUnknownCriticalExtension, NULL                                , networkErrGeneric },
    { sslErrVerifyCallback,                 NULL                                , networkErrGeneric },
    { 0,                                    NULL                                , systemErrNone     }
};

int netErrorTranslate(int err)
{
    int i = 0;

    while (errorList[i].number && (errorList[i].number != err)) {
        i++;
    }
    
    return err; // errorList[i].internalErr;
}

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
        return error;
    
    netInfo->error = error;

    return error;
}

int netGetError(NetSessionPtr netInfo)
{
    return (netInfo)?netInfo->error:0;
}

NetSessionType *netOpenNetwork(NetSessionPtr netInfo)
{
    NetSessionPtr   session     = netInfo;
    Err             error       = errNone;
    Err             errno       = errNone;
    
    if (!session) { // The session does not exist, we need to create it.
        debugOutFunctionFormattedV0(__FUNCTION__, "create session, prev none existant", 0, __FILE__, __LINE__);
        session = (NetSessionType *)memMgrChunkNew(sizeof(NetSessionType));
        if (!session)
            return NULL;
        
        memMgrChunkSet(session, 0, sizeof(NetSessionType));
        
        error = SysLibFind("Net.lib", &session->refNum);
        if (error) {
            netSetError(session, netErrorTranslate(error));
            //errAlert("Could not open the Network library. Maybe it is missing or corrupt? Please try a soft reset.");
            return session;
        }
    }
    
    error = NetLibOpen(session->refNum, &errno);
    if (errno || (error && (error != netErrAlreadyOpen))) {
        debugOutFunctionFormattedV0(__FUNCTION__, "NetLibOpen error", error, __FILE__, __LINE__);
        debugOutFunctionFormattedV0(__FUNCTION__, "NetLibOpen errno", errno, __FILE__, __LINE__);
        if ((errno != netErrUserCancel) && (error != netErrUserCancel))
            netDisplayError("Could not open the network connection.", (errno)?errno:error, __FILE__, __LINE__);

        if (!error && errno)
            error = errno;
        netCloseNetwork(&session, false);
        
        netSetError(session, netErrorTranslate(error));
        
        return session;
    } else if (!error || (error == netErrAlreadyOpen)) {
        debugOutFunctionFormattedV0(__FUNCTION__, "refreshing network", error, __FILE__, __LINE__);
        if ((error = netRefreshNetwork(session))) {
            debugOutFunctionFormattedV0(__FUNCTION__, "refresh failed", error, __FILE__, __LINE__);
            netSetError(session, netErrorTranslate(error));
            return session;
        }
        debugOutFunctionFormattedV0(__FUNCTION__, "network refreshed", 0, __FILE__, __LINE__);

        return session;
    }
    debugOutFunctionFormattedV0(__FUNCTION__, "network open", 0, __FILE__, __LINE__);
        
    return session;
}

int netCloseNetwork(NetSessionPtr *netInfo, boolean disconnect)
{
    Err error = 0;
    
    if (!netInfo) {
        return networkErrNoSession;
    }
    
    if (disconnect) {
        error = NetLibClose((*netInfo)->refNum, true);
    }
    
    memMgrChunkFree(*netInfo);
    *netInfo = NULL;
    
    return netErrorTranslate(error);
}

int netRefreshNetwork(NetSessionPtr netInfo)
{
    UInt16      netIFErr;
    Boolean     allUp;
    Err         err;

    if (!netInfo) {
        return networkErrNoSession;
    }

    err = NetLibConnectionRefresh(netInfo->refNum, true, &allUp, &netIFErr);
    if (!err && !netIFErr && allUp){
        return 0;
    }

    return netErrorTranslate(err);
}

boolean netIsConnected(NetSessionPtr netInfo)
{
    UInt16      openCount, netIFErr;
    Boolean     allUp;
    Err         err;

    if (!netInfo) {
        return false;
    }

    if (NetLibOpenCount(netInfo->refNum, &openCount) == 0){
        if (openCount) {
            err = NetLibConnectionRefresh(netInfo->refNum, false, &allUp, &netIFErr);
            if (!err && !netIFErr && allUp){
                return true;
            }
        }
    }

    return false;
}

int netConnectionEstablish(NetSessionPtr netInfo, char *host, char *service, short port)
{
    NetSocketRef            socket          = 0;
    NetHostInfoBufPtr       AppHostInfo     = NULL;
    NetHostInfoPtr          HostInfo        = NULL;
    UInt32                  AppNetTimeout   = 1 * SysTicksPerSecond();
    UInt32                  AppSysTimeout   = netGetTimeout(netInfo) * SysTicksPerSecond();
    UInt32                  iTime           = 0;
    UInt32                  time            = 0;
    UInt32                  connectTime     = 0;
    Err                     errP, saveErr;
    Int16                   sockConnect     = -1;
    NetIPAddr               address;
    NetIPAddr               *addressPtr     = NULL;
    NetSocketAddrINType     sockAddrP;
    NetSocketLingerType     sockLinger;

    if (!netInfo) {
        return networkErrNoSession;
    }

    //DebugOutComms("NetUTCP2Open", (Char *)host, port, __FILE__, __LINE__, 0);

    address = NetLibAddrAToIN(netInfo->refNum, host);
    if (address != -1)
        goto netConnectionEstablish;

    //DebugOutComms("NetUTCP2Open", "got address", 0, __FILE__, __LINE__, 0);

    AppHostInfo = (NetHostInfoBufPtr)memMgrChunkNew(sizeof(NetHostInfoBufType));
    if (!AppHostInfo) {
        //if (g->isImage != true)
        //    NetDisplayError("Not enough dynamic heap for network communications.", netErrOutOfMemory, __FILE__, __LINE__);
        return netSetError(netInfo, memoryErrNoFreeMemory);
    }

    //DebugOutComms("NetUTCP2Open", "get host by name", 0, __FILE__, __LINE__, 0);

    iTime = TimGetTicks();
    do {
        HostInfo = NetLibGetHostByName(netInfo->refNum, host, AppHostInfo, AppNetTimeout, &errP);
        time = TimGetTicks();
    } while (!HostInfo && (errP == netErrTimeout) && (time < (iTime + AppSysTimeout)));

    // for some reason some NetLib calls don't set errP to errNone
    if (!HostInfo /*&& (errP != errNone)*/) {
        //if (!g->isImage)
        //    NetDisplayError("Host lookup failed. Try again later.", errP, __FILE__, __LINE__);
        if (AppHostInfo)memMgrChunkFree(AppHostInfo);
        //DebugOutComms("NetUTCP2Open", "DNS Error", (Int32)errP, __FILE__, __LINE__, 0);
        /*{
            char    temp[500];

            StrPrintF(temp, "DNS Err stuff: HostInfo: %ld, time: %ld, iTime: %ld, tot: %ld",
                  (Int32)HostInfo, (Int32)time, (Int32)iTime, (Int32)iTime + AppSysTimeout);
            DebugOutComms("NetUTCP2Open", temp, 0, __FILE__, __LINE__, 0);
        }*/
        return netSetError(netInfo, netErrorTranslate(errP));
    }

    //DebugOutComms("NetUTCP2Open", "Got DNS", 0, __FILE__, __LINE__, 0);

    connectTime = TimGetTicks();

    memMgrChunkSet(&sockAddrP, 0, sizeof(sockAddrP));

    //DebugOutComms("NetUTCP2Open", "sockAddrP init\'d", 0, __FILE__, __LINE__, 0);

    if (address == -1) {
        addressPtr = (NetIPAddr*)HostInfo->addrListP[0];
        address = NetNToHL(*addressPtr);
    }

    //DebugOutComms("NetUTCP2Open", "address init\'d", 0, __FILE__, __LINE__, 0);

    //MemMove(&sockAddrP.sin_addr,&AppHostInfo.address[0][0],sizeof(AppHostInfo.address[0]));
    /*MemMove(&sockAddrP.sin_addr,&address,sizeof(address));
       sockAddrP.sin_family = AF_INET;
      sockAddrP.sin_port = htons(port);*/

    sockAddrP.family = netSocketAddrINET;
    sockAddrP.port   = NetHToNS(port);
    memMgrChunkMove(&sockAddrP.addr, &address, sizeof(address));

    // Maybe, if we cannot connect to one address, try for other addresses??
netConnectionEstablish:

    //DebugOutComms("NetUTCP2Open", "sockAddrP setup, open socket", 0, __FILE__, __LINE__, 0);

    socket = NetLibSocketOpen(netInfo->refNum, netSocketAddrINET, netSocketTypeStream, 0, AppNetTimeout, &errP);

    if (socket == -1) {
        //if (!g->isImage)
        //    NetDisplayError("Failed to prepare network communications. Try again later.", errP, __FILE__, __LINE__);
        if (AppHostInfo)
            memMgrChunkFree(AppHostInfo);
        //DebugOutComms("NetUTCP2Open", "Socket error", (Int32)errP, __FILE__, __LINE__, 0);
        return netSetError(netInfo, netErrorTranslate(errP));
    }

    //DebugOutComms("NetUTCP2Open", "setup socket", 0, __FILE__, __LINE__, 0);

    sockLinger.onOff = true;
    sockLinger.time = 0;
    sockConnect = NetLibSocketOptionSet(netInfo->refNum, socket, netSocketOptLevelSocket, netSocketOptSockLinger,
                                        &sockLinger, sizeof(sockLinger), AppNetTimeout, &errP);
    if (sockConnect == -1) {
        saveErr = errP;
        NetLibSocketClose(netInfo->refNum, socket, AppNetTimeout, &errP);
        socket = -1;

        //if (!g->isImage)
        //    NetDisplayError("Failed to connect to the remote host or proxy. Try again later.", saveErr, __FILE__, __LINE__);
        if (AppHostInfo)
            memMgrChunkFree(AppHostInfo);

        //DebugOutComms("NetUTCP2Open", "failed to setup socket", (Int32)saveErr, __FILE__, __LINE__, 0);
        return netSetError(netInfo, netErrorTranslate(errP));
    }

    //DebugOutComms("NetUTCP2Open", "Connect Socket", 0, __FILE__, __LINE__, 0);

    //sockConnect = NetLibSocketConnect(AppNetRefnum, socket, (void*)&sockAddrP, sizeof(NetSocketAddrType), AppNetTimeout, &errP);
    sockConnect = NetLibSocketConnect(netInfo->refNum, socket, (NetSocketAddrType *)&sockAddrP, sizeof(sockAddrP), AppNetTimeout, &errP);

    if (sockConnect == -1) {
        saveErr = errP;
        NetLibSocketClose(netInfo->refNum, socket, AppNetTimeout, &errP);
        socket = -1;

        //DebugOutComms("NetUTCP2Open", "failed to connect", (Int32)saveErr, __FILE__, __LINE__, 0);
        if ((saveErr == netErrTimeout) && (TimGetTicks() < (connectTime + AppSysTimeout))) {
            goto netConnectionEstablish;
        }

        //if (!g->isImage)
        //    NetDisplayError("Failed to connect to the remote host or proxy. Try again later.", saveErr, __FILE__, __LINE__);
        if (AppHostInfo)
            memMgrChunkFree(AppHostInfo);

        //DebugOutComms("NetUTCP2Open", "failed to connect, post timeout", (Int32)saveErr, __FILE__, __LINE__, 0);
        return netSetError(netInfo, netErrorTranslate(errP));
    }

    //DebugOutComms("NetUTCP2Open", "connected!", 0, __FILE__, __LINE__, 0);

    if (AppHostInfo)
        memMgrChunkFree(AppHostInfo);

    netInfo->socket = socket;
    
    return systemErrNone;
}

void netConnectionClose(NetSessionPtr netInfo)
{
    Err     errP = 0;

    if (!netInfo)
        return;

    if (netInfo->socket != -1)
        NetLibSocketClose(netInfo->refNum, netInfo->socket, 2 * SysTicksPerSecond(), &errP);
    
    netInfo->socket = -1;
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
    long    result          = 0;
    UInt32  AppNetTimeout   = 1 * SysTicksPerSecond();

    if (!netInfo) {
        *err = networkErrNoSession;
        
        return -1;
    }
    
    result = NetLibSend(netInfo->refNum, netInfo->socket,     // We use a quick failure, caller should loop for non-blocking
                        data, size, 0, NULL, 0, AppNetTimeout /*netGetTimeout(netInfo)*/, (Err *)err);
    
    *err = netErrorTranslate(*err);
    
    netSetError(netInfo, *err);
    
    return result;
}

long netReceiveData(NetSessionPtr netInfo, void *buffer, unsigned long buffSize, int *err)
{
    long    result          = 0;
    UInt32  AppNetTimeout   = 1 * SysTicksPerSecond();

    if (!netInfo) {
        *err = networkErrNoSession;
        
        return -1;
    }
        
    result = NetLibReceive(netInfo->refNum, netInfo->socket,    // We use a quick failure, caller should loop for non-blocking
                    buffer, buffSize, 0, NULL, 0, AppNetTimeout /*netGetTimeout(netInfo)*/, (Err *)err);
    
    *err = netErrorTranslate(*err);
    
    netSetError(netInfo, *err);
    
    return result;
}

