#include <sys/stat.h>
#include <PalmTypes.h>
#include <VFSMgr.h>
#include "implementation.h"

/* This is a representation of seconds from 1970 (UNIX epoch)
   to 1904 (PalmOS/Mac OS epoch) */
#define secondsFrom1970     (-2082844800L)

int stat(const char *path, struct stat *buf)
{
    if (path && buf) {
        UInt16  error;
        FileRef fileRef;
        
        error = VFSFileOpen(DefaultSDCard, path, vfsModeRead, &fileRef);
        if (!error) {
            UInt32  attribs;
            UInt16  error;
            
            if (!VFSFileGetAttributes(fileRef, &attribs)) {
                
            }
            
            VFSFileSize(fileRef, (UInt32 *)&buf->st_size);
            
            if (!(error = VFSFileGetDate(fileRef, vfsFileDateAccessed, (UInt32 *)&buf->st_atime))) {
                buf->st_atime += secondsFrom1970;
            } else {
                goto stat_close;
            }
            if (!(error = VFSFileGetDate(fileRef, vfsFileDateModified, (UInt32 *)&buf->st_mtime))) {
                buf->st_mtime += secondsFrom1970;
            } else {
                goto stat_close;
            }
            if (!(error = VFSFileGetDate(fileRef, vfsFileDateCreated, (UInt32 *)&buf->st_ctime))) {
                buf->st_ctime += secondsFrom1970;
            } else {
                goto stat_close;
            }

            // Unsupported
            buf->st_dev     = 0;
            buf->st_ino     = 0;
            buf->st_nlink   = 0;
            buf->st_uid     = 0;
            buf->st_gid     = 0;
            buf->st_rdev    = 0;
            // We could probably create these values based on the FS
            // and file size
            buf->st_blksize = 0;
            buf->st_blocks  = 0;

stat_close:
            VFSFileClose(fileRef);
        } else {
            // errno = ;
            return -1;
        }
        
        return 0;
    }

    // errno = ;
    return -1;
}
