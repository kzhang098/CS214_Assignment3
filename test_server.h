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
typedef struct packetData {
	int functionType; //0: Netopen ; 1: Netclose; 2: Netread; 3: Netwrite 	
	int flag; //The open, read, write commands.
	int size; //The size of the bytes to be read, written. 
	int fileDescriptor; //The server-side file descriptor for the file (This is returned from the netopen() 
	char * buffer; //The buffer containing information that's to be read from or written to the file. 
	char * filePath; //The file that the client will be "touching"
} packetData; 
*/

typedef struct threadInfo{
	clientInfo * client;
	int threadNum;
} threadInfo;

//A struct to hold information about a client. Used to determine what operations are allowed.
typedef struct clientInfo { 
	char * IPAddress; //Unique IP Address of the client.
	char * currentlyOpenFilePath; //Path of the file they currently have open.
	int socketId; // Current socket they are using to communicate with the server.
	int mode; // File mode. Can be unrestricted, exclusive, or transaction.
	struct clientInfo* next; // Next client in the linked list.
	int threadNum;
} clientInfo;

//A struct to hold information about an open file. Used to determine what operations are allowed.
typedef struct fileInfo {
	char * pathname; //Path of the file.
	char * IPAddress; //IP Address that has the file open.
	int filed; //Current file descriptor of this file.
	int mode; //File mode. Can be unrestricted, exclusive, or transaction.
	int o_flags; //The flags used to open this file. Can be Read only, Write only, or Read and Write.
	struct fileInfo * next; //Next file in the linked list.
} fileInfo;


/*
*GLOBAL VARIABLES
*/

fileInfo * fileNames; 
pthread_t* clientThreads[10000000];
clientInfo* clients;
fileInfo* files;
pthread_mutex_t fileLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t userLock = PTHREAD_MUTEX_INITIALIZER;