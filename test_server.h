#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h> 
#include <arpa/inet.h>

#include "libnetfiles.c"
/*
* This struct is supposed to store client information such as it's unique id and it's socket id (the socket we're using to communicate with it) 
*
*
*/

typedef struct packetData {
	int functionType; //0: Netopen ; 1: Netclose; 2: Netread; 3: Netwrite 	
	int flag; //The open, read, write commands.
	int size; //The size of the bytes to be read, written. 
	int fileDescriptor; //The server-side file descriptor for the file (This is returned from the netopen() 
	char * buffer; //The buffer containing information that's to be read from or written to the file. 
	char * filePath; //The file that the client will be "touching"
} packetData; 

typedef struct clientInfo { 
	char * IPAddress;
	int socketId;
	int mode;
	struct clientInfo* next;
} clientInfo;

typedef struct fileInfo {
	char * pathname;
	int mode;
	int o_flags;
	struct fileInfo * next;
} fileInfo;


/*
*GLOBAL VARIABLES
*/

char * fileNames[10]; 
pthread_t clientThreads[5000];
clientInfo* clients;
fileInfo* files;
//pthread_mutex_t userLock;
pthread_mutex_t userLock = PTHREAD_MUTEX_INITIALIZER;