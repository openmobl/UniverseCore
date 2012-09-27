#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include "NetDef.h"

ssize_t read(int fildes, void *buf, size_t nbyte)
{
    return NetLibReceive(fildes, buf, nbyte, 0, 0, 0, AppNetTimeout, NULL);
}
