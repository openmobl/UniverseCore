#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include "NetDef.h"

int gethostname(char *name, size_t namelen)
{
    Err     err;
    size_t  bufLen;

    // Get the appropriate preference in the Net Library
    bufLen = namelen;
    memset(name, 0, namelen);
    err = NetLibSettingGet(netSettingHostName, name, (unsigned short *)&bufLen);

    // Ignore netErrBufTooSmall errors since the sockets API doesn't
    //   return them - it just truncates the result (which NetLibSettingGet will
    //  do).
    if (err == netErrBufTooSmall) err = 0;

    // Return error code
    errno = err;

    return (err) ? -1 : 0;
}
