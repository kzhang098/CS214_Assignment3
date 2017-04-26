#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/*
*GLOBAL VARIABLES
*/

char * fileNames[10]; 
pthread_t clients[10];

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
	int * socketId;
	int clientId; 
	int portNum; 
	packetData * commands;
} clientInfo;


/*
* THE BELOW FUNCTIONS SHOULD BE MOVED TO LIBNETFILES LATER
*
*/

//Packet Structure: Function,File Descriptor^File Path;o_flags*buffer:nbyte? 

//Tip for delimeters: 
// " , " : Function 
// " ^ " : 
// 

char ** tokenizeMessage(char* message) {
	
	char ** result = (char**)malloc(6*sizeof(char*)); 
		
	//Index values and array to store the indices. 

	int funcIndex, fdIndex, pathIndex, flagIndex, bufferIndex, lengthIndex;
	int * indexArr[7]; 
	
	indexArr[0] = 0;
	funcIndex = strcspn(message, ","); 
	indexArr[1] = funcIndex;
	fdIndex = strcspn(message, "^");
	indexArr[2] = fdIndex; 
	pathIndex = strcspn(message, ";");
	indexArr[3] = pathIndex;
	flagIndex = strcspn(message, "*");
	indexArr[4] = flagIndex; 
	bufferIndex = strcspn(message,":");
	indexArr[5] = bufferIndex;
	lengthIndex = strcspn(message, "?");
	indexArr[6] = lengthIndex; 

	//Argument values

	char * functionName = (char*)malloc(sizeof(funcIndex + 1));
	char * fileDes = (char*)malloc(sizeof(fdIndex + 1)); 
	char * path = (char*)malloc(sizeof(pathIndex + 1)); 
	char * flags = (char*)malloc(sizeof(flagIndex + 1));
	char * buffer = (char*)malloc(sizeof(bufferIndex + 1));
	char * length = (char*)malloc(sizeof(lengthindex + 1)); 
	
	int i
 
	for(i = 0; i < 6; i++) {
		if(indexArr[i+1] - indexArr[i] != 1) {
			switch(i) {
				case 0:
					strncpy(functionName, message[0], indexArr[1] - indexArr[0]);
					strcat(functionName, "\0");  
					break; 		
				case 1:
					strncpy(fileDes, message[indexArr[1] + 1], indexArr[2] - indexArr[1]);
					strcat(fileDes, "\0");
					break;  
				case 2:
					strncpy(path, message[indexArr[2] + 1], indexArr[3] - indexArr[2]);
					strcat(path, "\0");
					break; 
				case 3:
					strncpy(flags, message[indexArr[3] + 1], indexArr[4] - indexArr[3]);
					strcat(flags, "\0");
					break;
				case 4:
					strncpy(buffer, message[indexArr[4] + 1], indexArr[5] - indexArr[4]);
					strcat(buffer, "\0");
					break;
				case 5: 
					strncpy(length, message[indexArr[5] + 1], indexArr[6] - indexArr[5]); 
					strcat(length, "\0");
					break;
			}
		} else {
			
			//Otherwise I need to signal that the argument is empty.
			
			 switch(i) {
                                case 0:
                                        strncpy(functionName, "empty", 5);
                                        break;
                                case 1:
                                        strncpy(fileDes,  "empty", 5);
                                        strcat(fileDes, "\0");
                                        break;
                                case 2:
                                        strncpy(path,  "empty", 5);
                                        strcat(path, "\0");
                                        break;
                                case 3:
                                        strncpy(flags,  "empty", 5);
                                        strcat(flags, "\0");
                                        break;
                                case 4:
                                        strncpy(buffer,  "empty", 5);
                                        strcat(buffer, "\0");
                                        break;
                                case 5:
                                        strncpy(length,  "empty", 5);
                                        strcat(length, "\0");
                                        break;
                        }
		}
	}

	result[0] = functionName;
	result[1] = fileDes;
	result[2] = path;
	result[3] = flags; 
	result[4] = buffer;
	result[5] = length; 

	return result;
}


int readFromSocket(int socket, clientInfo * client) {

	//For testing purposes: 
	printf("This is the buffer received from the client: %s\n", buffer);

	//After which I should be able to parse the data from the packet and store it in the packetData struct. 
	return 0; 
	
}


//void parseBuffer(char * buffer);

int runCommands(clientInfo * client) {
	int * socket = client->socketId; // This is the socket we are using to communicate with the client
	
	readFromSocket(socket, client); 
	
	return 0; 
}

int main(int argc, char ** argv) {

	int sockfd; //Stands for socket found 
	int newsockfd;
	int portNum; 
	int n; 
	int clilen;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr; 

	char buffer[256];

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

	if(sockfd < 0) {
		printf("Error. Could not open socket!"); 
		exit(1); 
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); 
	portNum = atoi(argv[1]); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portNum); 

	int status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if(status < 0) {
		printf("Error binding to client");
		exit(1);
	} 

	listen(sockfd, 1);
		
	int clientNum = 0;
	while (1) {
		while(i < 10) {
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			
			if(newsockfd < 0) {
				printf("Could not accept connection from client"); 
			} else {
				clientInfo * cInfo = (clientInfo*)malloc(sizeof(clientInfo));
				cInfo->socketId = (int*) malloc(sizeof(int));
				*(cData->socketId) = newsockfd; //Stores the socket into the struct. 
				cData->clientId = i; 
				cData->commands = (packetData*)malloc(sizeof(packetData)); 
			}
			
			flag = pthread_create(&clients[i], NULL, (void*)runCommands, cData);  //Starts a new thread with the created struct 
			
			if(flag == 1) {

			} else if (flag == 0) {
				i++; 
			}
		
			memset(buffer,0, 256); 
			n = read(newsockfd, buffer, 255); 

			printf("This is the message: %s\n", buffer);
		
			pthread_join(flag, NULL);
			close(newsockfd);
			close(sockfd); 
		}
	}
	return 0; 
}
