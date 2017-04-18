#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


/*
* This struct is supposed to store client information such as it's unique id and it's socket id (the socket we're using to communicate with it) 
*
*
*/

typedef struct clientData { 
	int * socketId;
	int clientId; 
} clientData;

int runCommands(clientData * client) {

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
		
	pthread_t clients[10];
	int clientNum = 0;
	
	while(i < 10) {
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
		if(newsockfd < 0) {
			printf("Could not accept connection from client"); 
		} else {
			clientData * cData = (clientData*)malloc(sizeof(clientData));
			cData -> socketId = (int*) malloc(sizeof(int));
			*(cData -> socketId) = newsockfd; 
			cData -> clientId = i; 
		}

		flag = pthread_create(&clients[i], NULL, (void*)runCommands, cData);  
		
		if(flag == 1) {

		} else if (flag == 0) {
			i++; 
		}
		
	memset(buffer,0, 256); 
	n = read(newsockfd, buffer, 255); 

	printf("This is the message: %s\n", buffer);
	
	close(newsockfd);
	close(sockfd); 

	return 0; 
}
