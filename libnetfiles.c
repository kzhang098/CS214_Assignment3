#include "libnetfiles.h"


/*
* This function checks whether the given path leads to a file or directory. It returns 1 if the given path leads
* to a directory, 0 if the given path leads to a file, and -1 otherwise.
*/
int isDir(char * path) {
        struct stat buf;
        stat(path, &buf);

		// Returns 1 if the path leads to a directory
        if(S_ISDIR(buf.st_mode) == 1) {
            return 1;
		// Returns 0 if the path leads to a file
        } else if(S_ISREG(buf.st_mode) == 1) {
            return 0;
		// Returns -1 otherwise
        } else {
            return -1;
        }

	return S_ISDIR(buf.st_mode);
}


char * callServer(int sockfd, char * buffer) {
	int n = write(sockfd, buffer, strlen(buffer));
	if(n < 0) {
		printf("Error writing to socket...");
		exit(1);	
	}

	bzero(buffer,255);
	printf("Reading...\n"); 
	n = read(sockfd,buffer,255); 
	printf("%s\n", buffer);
	printf("Stuck...\n");
	if(n < 0) {
		char * error = malloc(100);
		sprintf(error, "Error on line %d of %s", __LINE__, __FILE__);
		perror(error);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	printf("First loop"); 
	if (buffer[0] == '`') {
		char * error = malloc(100);
		strncpy(error, &buffer[1], strlen(buffer) - 1);
		errno = atoi(error);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	printf("Returned %s\n", buffer); 
	return buffer;
}
	
void createMessage(char * finalMessage, char * func, int fildes, const char * path, int oflags, void * buf, size_t nbyte) {
	finalMessage[0] = '\0';
	finalMessage = strncat(finalMessage, func, strlen(func));
	finalMessage = strncat(finalMessage, ",", 1);
	if (fildes != -1) {
		char * fd = malloc(64);
		sprintf(fd, "%d", fildes);
		finalMessage = strncat(finalMessage, fd, strlen(fd));
	}
	finalMessage = strncat(finalMessage, "^", 1);
	if (path != NULL) {
		finalMessage = strncat(finalMessage, path, strlen(path));
	}
	finalMessage = strncat(finalMessage, ";", 1);
	if (oflags != -1) {
		if (oflags == 0) {
			finalMessage = strncat(finalMessage, "0", 1);
		} else if (oflags == 1) {
			finalMessage = strncat(finalMessage, "1", 1);
		} else {
			finalMessage = strncat(finalMessage, "2", 1);
		}
	}
	finalMessage = strncat(finalMessage, "*", 1);
	if (buf != NULL) {
		if (strcmp(func, "read") == 0) {
			char * buffer = malloc(64);
			sprintf(buffer, "%d", buf);
			finalMessage = strncat(finalMessage, buffer, strlen(buffer));
		} else {
			char * buffer = malloc(strlen(buf) + 1);
			strncpy(buffer, buf, strlen(buf));
			buffer[strlen(buf)] = '\0';
			finalMessage = strncat(finalMessage, buffer, strlen(buffer));
		}
	} 
	finalMessage = strncat(finalMessage, ":", 1);
	if (nbyte != 0) {
		char * nbytes = malloc(64);
		sprintf(nbytes, "%d", nbyte);
		finalMessage = strncat(finalMessage, nbytes, strlen(nbytes));
	}
	finalMessage = strncat(finalMessage, "?", 1);
}
	
void error(char * error_msg) {
	perror(error_msg);
	printf("Error: %s\n", strerror(errno));
	exit(1); 
}
int openSocket(char * hostname) {
	int portNum = 9001;	//Port number the client communicates on. 
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
		char * error = malloc(100);
		sprintf(error, "Error on line %d of %s", __LINE__, __FILE__);
		perror(error);
		printf("Error: %s\n", strerror(errno));
		exit(1);
		return -1;
	} 
	server = gethostbyname(hostname);

	//This above line/function resolves the hostname and will map the hostname to it's IP ADDRESS. If not resolved, then return NULL. Otherwise
	//a struct is returned with server info. 
	
	if(server == NULL) {
		char * error = malloc(100);
		sprintf(error, "Error on line %d of %s", __LINE__, __FILE__);
		perror(error);
		printf("Error: %s\n", strerror(errno));
		exit(1);
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); //YOU HAVE TO ZERO OUT everything in the struct. 

	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNum); //hton? This converts an int to a network readable integer.
	
	int status = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	if(status < 0) {
		char * error = malloc(100);
		sprintf(error, "Error on line %d of %s", __LINE__, __FILE__);
		perror(error);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	} 
	printf("Success\n");	
	return 0;
}

int netserverinit(char * hostname, int mode) {
	
	IPaddress = malloc(strlen(hostname));
	IPaddress = hostname;
	if (openSocket(IPaddress) == -1) {
		printf("Error initializing");
		return -1;
	}
	
	char * response = malloc(strlen("Initializing:") + 3);
	strncat(response,"Initializing:", 13);
	char* modestr = malloc(4);
	sprintf(modestr, "%d", mode);
	strncat(response, modestr, 1);
	
	int n = write(sockfd, response, strlen(response));
	if(n < 0) {
		printf("Error writing to socket...");
		exit(1);	
	}
	serverInitialized = 1;
	close(sockfd);
	return 0;
}

int netopen(const char *path, int oflags) {
	if (serverInitialized == 0) {
		printf("HOST NOT FOUND\n");
		return -1;
	}
	openSocket(IPaddress);
	if (oflags != 0 && oflags != 1 && oflags != 2) {
		printf("Invalid flags.\n");
		return;
	}
	char * finalMessage = malloc(15 + strlen(path));
	createMessage(finalMessage, "open", -1, path, oflags, NULL, 0);
	printf("%s\n", finalMessage);
	int fd = atoi(callServer(sockfd, finalMessage));
	
//	handleError(1,fd); // Will do nothing if nothing is wrong
	
	printf("closing?\n");
	close(sockfd);
	printf("closed\n");
	return fd; 
}

ssize_t netread(int fildes, void *buf, size_t nbyte) {
	printf("Entering netread \n");
	if (serverInitialized == 0) {
		printf("HOST NOT FOUND\n");
		return -1;
	}
	openSocket(IPaddress);
	char * finalMessage = malloc(264);
	createMessage(finalMessage, "read", fildes, NULL, -1, buf, nbyte);
	printf("%s\n", finalMessage);
	char * response = callServer(sockfd, finalMessage);
	char * token = strtok(response, "^");
	char * numbytes = strtok(response, "^");
	strncpy(buf, token, strlen(token));

	//handleError(2,response);
	close(sockfd);
	int i = atoi(numbytes); 
	printf("%d\n",i);
	return i;
}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte) {
	if (serverInitialized == 0) {
		printf("HOST NOT FOUND\n");
		return -1;
	}
	openSocket(IPaddress);
	char * finalMessage = malloc(164 + strlen(buf));
	createMessage(finalMessage, "write", fildes, NULL, -1, buf, nbyte);
	printf("%s\n", finalMessage);
	int byteswritten = atoi(callServer(sockfd, finalMessage));
	//handleError(3, byteswritten); 
	close(sockfd);
	return byteswritten;
}

 int netclose(int fd) {
	if (serverInitialized == 0) {
		printf("HOST NOT FOUND\n");
		return -1;
	}
	openSocket(IPaddress);
	char * finalMessage = malloc(64);
	createMessage(finalMessage, "close", fd, NULL, -1, NULL, 0);
	printf("%s\n", finalMessage);
	int success = atoi(callServer(sockfd, finalMessage));
	//handleError(4, success); 
	close(sockfd);
	return success;
 }