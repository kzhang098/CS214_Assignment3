#include "libnetfiles.h"


int openSocket() {
	int sockfd;	// This is an integer field which will tell us whether the socket creation call fails
	int portNum;	//Port number the client communicates on. 
	int n;	
	struct hostent * server; //For the hostname of the server. Store IP and Hostname. 
	struct sockaddr_in serv_addr; //This is the struct we will use to start the connection. Will contain info from hostent * server
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  

	if(sockfd < 0) {
		return;
	} 
	server = gethostbyname("localhost");

	//This above line/function resolves the hostname and will map the hostname to it's IP ADDRESS. If not resolved, then return NULL. Otherwise
	//a struct is returned with server info. 
	
	if(server == NULL) {
		return;
	}
	
	portNum = atoi("51717"); //Port number

	memset(&serv_addr, 0, sizeof(serv_addr)); //YOU HAVE TO ZERO OUT everything in the struct. 

	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNum); //hton? This converts an int to a network readable integer.
	
	int status = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	if(status < 0) {
		printf("Error connecting...");
	} 
	return sockfd;
}

char * callServer(int sockfd, char * buffer) {
	int n = write(sockfd, buffer, strlen(buffer));
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
	return buffer;
}
	
	
void error(char * error_msg) {
	perror(error_msg);
	exit(1); 
}

int netopen(const char *path, int oflags) {
	int sockfd = openSocket();
	if (oflags != 0 && oflags != 1 && oflags != 2) {
		printf("Invalid flags.\n");
		return;
	}
	char * finalMessage = malloc(15 + strlen(path));
	finalMessage = strncat(finalMessage, "open", 4);
	finalMessage[4] = ',';
	finalMessage[5] = '\0';
	finalMessage = strncat(finalMessage, path, strlen(path));
	finalMessage = strncat(finalMessage, ",", 1);
	if (oflags == 0) {
		finalMessage = strncat(finalMessage, "0", 1);
	} else if (oflags == 1) {
		finalMessage = strncat(finalMessage, "1", 1);
	} else {
		finalMessage = strncat(finalMessage, "2", 1);
	}
	finalMessage[strlen(finalMessage)] = '\0';
	printf("%s\n", finalMessage);
	callServer(sockfd, finalMessage);
	close(sockfd);
	return 0; 
}

ssize_t netread(int fildes, void *buf, size_t nbyte) {
	int sockfd = openSocket();
	char * fd = malloc(64);
	sprintf(fd, "%d", fildes);
	char * finalMessage = malloc(strlen(fd) + 200);
	finalMessage[0] = '\0';
	finalMessage = strncat(finalMessage, "read", 4);
	finalMessage = strncat(finalMessage, ",", 1);
	finalMessage = strncat(finalMessage, fd, strlen(fd));
	finalMessage = strncat(finalMessage, ",", 1);
	char * buffer = malloc(64);
	sprintf(buffer, "%ld", buf);
	finalMessage = strncat(finalMessage, buffer, strlen(buffer));
	finalMessage = strncat(finalMessage, ",", 1);
	char * nbytes = malloc(64);
	sprintf(nbytes, "%d", nbyte);
	finalMessage = strncat(finalMessage, nbytes, strlen(nbytes));
	finalMessage[strlen(finalMessage)] = '\0';
	printf("%s\n", finalMessage);
	callServer(sockfd, finalMessage);
	close(sockfd);
	return 0;
}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte) {
	int sockfd = openSocket();
	char * fd = malloc(64);
	sprintf(fd, "%d", fildes);
	char * finalMessage = malloc(strlen(fd) + 200);
	finalMessage[0] = '\0';
	finalMessage = strncat(finalMessage, "write", 5);
	finalMessage = strncat(finalMessage, ",", 1);
	finalMessage = strncat(finalMessage, fd, strlen(fd));
	finalMessage = strncat(finalMessage, ",", 1);
	char * buffer = malloc(64);
	sprintf(buffer, "%ld", buf);
	finalMessage = strncat(finalMessage, buffer, strlen(buffer));
	finalMessage = strncat(finalMessage, ",", 1);
	char * nbytes = malloc(64);
	sprintf(nbytes, "%d", nbyte);
	finalMessage = strncat(finalMessage, nbytes, strlen(nbytes));
	finalMessage[strlen(finalMessage)] = '\0';
	printf("%s\n", finalMessage);
	callServer(sockfd, finalMessage);
	close(sockfd);
	return 0;
}

 int netclose(int fd) {
	int sockfd = openSocket();
	char * fdes = malloc(64);
	sprintf(fdes, "%d", fd);
	char * finalMessage = malloc(strlen(fdes) + 10);
	finalMessage[0] = '\0';
	finalMessage = strncat(finalMessage, "close", 5);
	finalMessage = strncat(finalMessage, ",", 1);
	finalMessage = strncat(finalMessage, fdes, strlen(fdes));
	printf("%s\n", finalMessage);
	callServer(sockfd, finalMessage);
	close(sockfd);
	return 0;
 }