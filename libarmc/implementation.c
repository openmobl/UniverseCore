/*
 *  implementation.c
 *  
 *
 *  Created by Donald C. Kirker on 1/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ArmNetMgr.h>
#include <M68kCalls.h>
#include "NetDef.h"

//------------------------------------------------------
// Default SD card (checked and set by using functions)
//------------------------------------------------------
unsigned short DefaultSDCard = 0;

// Default NetLib timeout
int AppNetTimeout = sysTicksPerSecond * 2;

// Storage for inet_* calls
char AppINETString[32];

// Storage for gethost* calls
NetHostInfoBufType AppHostInfo;

// Storage for getserv* calls
NetServInfoBufType AppServInfo;

FILE *fstdout = NULL;
FILE *fstderr = NULL;

extern void _clearenv(void);

jmp_buf _exit_jmp;
void (*_at_exit_fptr)(void) = 0;

void PrintConsole(const char *strText)
{
    fwrite(strText, strlen(strText), 1, fstdout);
}

static UInt16 SetDefaultCardNo(void)
{
    UInt32 error;
    UInt16 iCardNo = 0xffff;
    UInt16 iFirstCardNo = 0xffff;
    LocalID dbID;
    UInt16 volRefNum;
    UInt32 volIterator;

    //---------------------------------------------
    // Get card no of current app
    //---------------------------------------------
    SysCurAppDatabase(&iCardNo, &dbID);

    //---------------------------------------------
    // Loop all SD cards
    //---------------------------------------------
    volIterator = vfsIteratorStart;
    while(volIterator != vfsIteratorStop) 
    {
        error = VFSVolumeEnumerate(&volRefNum, &volIterator);
        if (!error) 
        {
          if(iFirstCardNo == 0xffff)
            iFirstCardNo = volRefNum;
            if(volRefNum == iCardNo)
              return iCardNo;
        }
    }
    
    return iFirstCardNo;
}

int setupLibC(void)
{
    int     result      = 0;
    FILE    *pEnv       = NULL;
    char    *envBuffer  = NULL;

    AppNetTimeout = 10;

    // Setup the memory card that we are using
    DefaultSDCard = SetDefaultCardNo();

    // Open up stdout
    fstdout = fopen("/PALM/stdout", "ab");
    // open up stderr
    fstderr = fopen("/PALM/stderr", "ab");

    fprintf(stdout, "-------------\n");
    fprintf(stderr, "-------------\n");

    //-------------------------------------
    // Parse file 'environment' for predefined environ
    //-------------------------------------
    pEnv = fopen("environment", "rb");
    if(pEnv)
    {
        envBuffer = (char *)malloc(256);
        if(envBuffer)
        {
            while(fgets(envBuffer, 256, pEnv))
            {
                if(strlen(envBuffer) > 0)
                {
                    envBuffer[strlen(envBuffer) - 1] = 0; // remove newline
                    putenv(envBuffer);
                }
            }
            free(envBuffer);
        }
        fclose(pEnv);
    }

    NetLibLoad();

    result = setjmp(_exit_jmp);

    return result;
}

int destroyLibC(void)
{
    if(_at_exit_fptr)
        _at_exit_fptr();
    
    NetLibUnload();
    
    if (fstdout) {
        fclose(fstdout);
        fstdout = NULL;
    }
    if (fstderr) {
        fclose(fstderr);
        fstderr = NULL;
    }
    
    _clearenv();
    
    return 0;
}

