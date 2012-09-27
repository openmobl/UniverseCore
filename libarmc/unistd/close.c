#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include "NetDef.h"

int close(int fildes)
{
    return NetLibSocketClose(fildes, AppNetTimeout, NULL);
}
