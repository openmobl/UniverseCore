#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include "NetDef.h"

int sethostname(const char *name, size_t len)
{
    Err err;

    // Set the appropriate preference in the Net Library
    err = NetLibSettingSet(netSettingHostName, (void *)name, len + 1);

    errno = err;
    
    return (err) ? -1 : 0;
}
