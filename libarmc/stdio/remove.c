#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <implementation.h>


int remove(const char *filename)
{
    UInt16 error;
    char realfilename[FILENAME_MAX];

    //---------------------------------------------
    // Create complete filename with path
    //---------------------------------------------
    if(filename[0] == '/') {
        strncpy(realfilename, filename, FILENAME_MAX);
    }
    else
    {
        getcwd(realfilename, FILENAME_MAX);
        strncat(realfilename, "/", FILENAME_MAX);
        strncat(realfilename, filename, FILENAME_MAX);
    }

    //---------------------------------------------
    // Delete file
    //---------------------------------------------
    error = VFSFileDelete(DefaultSDCard, realfilename);
    return error;
}


