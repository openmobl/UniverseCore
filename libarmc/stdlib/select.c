#include <sys/select.h>

#include <PalmOS.h>

// Needs errno

int select(int nfds, fd_set *readfds,
       fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int  ticks;


    // Convert the timeout value to ticks
    if (timeout == 0) {
        ticks = -1;
    } else {
        ticks = (timeout->tv_sec * SysTicksPerSecond()) + 
                     (timeout->tv_usec /  (1000000L / SysTicksPerSecond()));
    }
                     
    // call Net Lib function
    return NetLibSelect(nfds, (NetFDSetType *)readfds,
                        (NetFDSetType *)writefds, (NetFDSetType *)errorfds,
                        ticks, NULL);
}
