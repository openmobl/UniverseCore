#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include "NetDef.h"

// TODO: h_errno needs to be set

int h_errno = 0;

struct hostent *gethostbyaddr(const void *addr, socklen_t len,
       int type)
{
    Err error = errNone;
    
    if ((error = NetLibOpen(NULL))) {
        h_errno = (int)error;
        return NULL;
    }
    
    return (struct hostent*)NetLibGetHostByAddr((UInt8 *)addr, len, type, &AppHostInfo, AppNetTimeout, NULL);
}

struct hostent *gethostbyname(const char *name)
{
    Err error = errNone;
    
    if ((error = NetLibOpen(NULL))) {
        h_errno = (int)error;
        return NULL;
    }
    
    return (struct hostent*)NetLibGetHostByName(name, &AppHostInfo, AppNetTimeout, NULL);
}

struct servent *getservbyname(const char *name, const char *proto)
{
    return (struct servent*)NetLibGetServByName(name, proto, &AppServInfo, AppNetTimeout, NULL);
}

