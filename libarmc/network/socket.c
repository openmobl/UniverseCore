#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>

#include <errno.h>

#include "NetDef.h"

// TODO: Map PalmOS Errors onto errno

int accept(int socket, struct sockaddr *address,
       socklen_t *address_len)
{
    return NetLibSocketAccept(socket, (NetSocketAddrType *)address, (Int16 *)address_len, AppNetTimeout, NULL);
}

int bind(int socket, const struct sockaddr *address,
       socklen_t address_len)
{
    return NetLibSocketBind(socket, (NetSocketAddrType*)address, address_len, AppNetTimeout, NULL);
}

int connect(int socket, const struct sockaddr *address,
       socklen_t address_len)
{
    return NetLibSocketConnect(socket, (NetSocketAddrType*)address, address_len, AppNetTimeout, NULL);
}

int getsockopt(int socket, int level, int option_name,
       void *option_value, socklen_t *option_len)
{
    return NetLibSocketOptionGet(socket, level, option_name, option_value, (UInt16 *)option_len, AppNetTimeout, NULL);
}

int listen(int socket, int backlog)
{
    return NetLibSocketListen(socket, backlog, AppNetTimeout, NULL);
}

ssize_t recv(int socket, void *buffer, size_t length, int flags)
{
    return NetLibReceive(socket, buffer, length, flags, 0, 0, AppNetTimeout, NULL);
}

ssize_t recvfrom(int socket, void *buffer, size_t length,
       int flags, struct sockaddr *address,
       socklen_t *address_len)
{
    return NetLibReceive(socket, buffer, length, flags, address, (UInt16 *)address_len, AppNetTimeout, NULL);
}

ssize_t recvmsg(int socket, struct msghdr *message, int flags)
{
    return NetLibReceivePB(socket, (NetIOParamType *)message, flags, AppNetTimeout, NULL);
}

ssize_t send(int socket, const void *buffer, size_t length, int flags)
{
    return NetLibSend(socket, (void *)buffer, length, flags, 0, 0, AppNetTimeout, NULL);
}

ssize_t sendmsg(int socket, const struct msghdr *message, int flags)
{
    return NetLibSendPB(socket, (NetIOParamType*)message, flags, AppNetTimeout, NULL);
}

ssize_t sendto(int socket, const void *message, size_t length,
       int flags, const struct sockaddr *dest_addr,
       socklen_t dest_len)
{
    return NetLibSend(socket, (void *)message, length, flags, (void *)dest_addr, dest_len, AppNetTimeout, NULL);
}

int setsockopt(int socket, int level, int option_name,
       const void *option_value, socklen_t option_len)
{
    return NetLibSocketOptionSet(socket, level, option_name, (void *)option_value, option_len, AppNetTimeout, NULL);
}

int shutdown(int socket, int how)
{
    return NetLibSocketShutdown(socket, how, AppNetTimeout, NULL);
}

int socket(int domain, int type, int protocol)
{
    Err error = errNone;
    
    if ((error = NetLibOpen(NULL))) {
        errno = (int)error;
        return -1;
    }
    
    return NetLibSocketOpen((NetSocketAddrEnum)domain, (NetSocketTypeEnum)type, protocol, AppNetTimeout, NULL);
}

int getpeername(int socket, struct sockaddr *address,
       socklen_t *address_len)
{
    return NetLibSocketAddr(socket, 0, 0, (NetSocketAddrType*)address, (Int16 *)address_len, AppNetTimeout, NULL);
}

int getsockname(int socket, struct sockaddr *address,
       socklen_t *address_len)
{
    return NetLibSocketAddr(socket, (NetSocketAddrType*)address, (Int16 *)address_len, 0, 0, AppNetTimeout, NULL);
}



