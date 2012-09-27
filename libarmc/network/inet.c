#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include "NetDef.h"

in_addr_t inet_addr(const char *cp)
{
    return NetLibAddrAToIN((char *)cp);
}

char *inet_ntoa(struct in_addr in)
{
    return NetLibAddrINToA((NetIPAddr)(in).s_addr, AppINETString);
}

