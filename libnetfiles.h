#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

static int serverInitialized = 0;
static char * IPaddress;
static int sockfd;

int openSocket(char * hostname);
char * callServer(int sockfd, char * buffer);
void error(char * error_msg);

int netserverinit(char * hostname, int mode);
int netopen(const char *path, int oflags);
ssize_t netread(int fildes, void *buf, size_t nbyte);
ssize_t netwrite(int fildes, const void *buf, size_t nbyte);
int netclose(int fd);