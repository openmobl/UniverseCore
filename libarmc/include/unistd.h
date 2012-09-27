#ifndef _UNISTD_H
#define _UNISTD_H


#ifdef __cplusplus
extern "C" {
#endif

// For now only supports sockets

ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);
int close(int fildes);

int gethostname(char *name, size_t namelen);
int sethostname(const char *name, size_t len);


#ifdef __cplusplus
}
#endif

#endif
