#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include "NetDef.h"

ssize_t write(int fildes, const void *buf, size_t nbyte)
{
    return NetLibSend(fildes, (void *)buf, nbyte, 0, 0, 0, AppNetTimeout, NULL);
}
