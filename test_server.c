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

//Packet Structure: Function Type, Flags, path length 

int readFromSocket(int socket, clientInfo * client) {
	int buffer[4];
	memset(buffer,0, sizeof(buffer)); 

	read(socket, buffer, sizeof(buffer)); 
	client->commands->functionType = buffer[0];
	client->commands->flag = buffer[1];
	client->commands->size = buffer[2]; 

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