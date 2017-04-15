#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 


int main(int argc, char ** argv) {
	
	char buffer[256]; // This represents buffer that will store messages/packets from the server
	int sockfd;	// This is an integer field which will tell us whether the socket creation call fails
	int portNum;	//Port number the client communicates on. 
	int n;	
	struct hostent * server; //For the hostname of the server. Store IP and Hostname. 
	struct sockaddr_in serv_addr; //This is the struct we will use to start the connection. Will contain info from hostent * server
	
	//Below is the segment which will create the socket
	//	1) Argument 1: You have two options. 
	//		AF_UNIX: This is used if you're trying to create a socket for two processes which share the same filesystem (local)
	//		AD_INET: For two different machines on the same or different networks. (This is what we want)
	//
	//	2) Argument 2: Type of socket. (Two types)
	//		SOCK_STREAM: If you want data to be sent through a continuous stream. 
	//		SOCK_DGRAM: If you want data from the server to be sent in chunks (datagrams) 
	//
	//	3) Argument 3: The protocol
	//		- This value will usually be 0 unless someothing special is needed. If 0, then the OS will determine the best
	//		socket to use. For stream sockets, it uses TCP (to ensure data is complete) and UDP for chunks (for speed but not 
	//		reliability: Meaning your udata may be incomplete) 
	//
	//	In General: This function will return -1 if a socket could not be created. << This should be the value you use to check for errors.
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  

	if(sockfd < 0) {
		// DO SOMETHING
	} 
		
	server = gethostbyname(argv[1]);

	//This above line/function resolves the hostname and will map the hostname to it's IP ADDRESS. If not resolved, then return NULL. Otherwise
	//a struct is returned with server info. 
	
	if(server == NULL) {
		//THROW ERROR
	}
	
	portNum = atoi(argv[2]); //Port number

	memset(&serv_addr, 0, sizeof(serv_addr)); //YOU HAVE TO ZERO OUT everything in the struct. 

	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNum); //hton? This converts an int to a network readable integer.
	
	int status = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	if(status < 0) {
		printf("Error connecting...");
	} 

	//If passed, then a connection to the server was successful. 
	printf("Please enter the message: ");	
	memset(buffer,0,256);
	fgets(buffer, 255, stdin);
		
	n = write(sockfd, buffer, strlen(buffer));
	
	if(n < 0) {
		printf("Error writing to socket...");
		exit(1);	
	}

	bzero(buffer,256);
	n = read(sockfd,buffer,255); 
	
	if(n < 0) {
		printf("Error reading from socket");
		exit(1); 
	}
	
	printf("%s\n", buffer); 
	
	close(sockfd);
	return 0; 
}


void error(char * error_msg) {
	perror(error_msg);
	exit(1); 
}

