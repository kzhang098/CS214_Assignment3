
#include "libnetfiles.h"

// A function to read from standard input into a buffer. Includes how many bytes to read from standard input.
int getStdin(char * buffer, int bytes) {
	//First, free the buffer, malloc the specified number of bytes, and zero it out.
	free(buffer);
	buffer = malloc(bytes);
	memset(buffer,0,bytes);
	//Get the specified number of bytes from standard input and store it in the buffer.
	fgets(buffer, bytes, stdin);
	int i = 0;
	//Terminate the string and remove the newline character appended to the end by standard input.
	while (buffer[i] != '\n' && i < strlen(buffer)) {
		i++;
	}
	buffer[i] = '\0';
	printf("%s\n", buffer);
}

// A function to concatenate the buffer and finalMessage, with a carat ^ separating them.
int concatFinalMessage(char * finalMessage, char * buffer) {
	//Realloc the size of finalMessage.
	finalMessage = realloc(finalMessage, strlen(finalMessage) + strlen(buffer) + 2);
	//Separate arguments with ^
	strncat(finalMessage, "^", 1);
	strncat(finalMessage, buffer, strlen(buffer));
	//Terminate the string.
	finalMessage[strlen(finalMessage)] = '\0';
	printf("%s\n", finalMessage);
}

//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 
int main(int argc, char ** argv) {
	
	char * buffer = malloc(256); // This represents buffer that will store messages/packets from the server
	char * finalMessage; // This represents the final message that will be sent to the server
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
	printf("Please enter what operation you are performing (open, read, write, or close):\n");
	//Get standard input. The default number of bytes to read is 256.
	getStdin(buffer, 256);
	finalMessage = malloc(strlen(buffer) + 2);
	strncat(finalMessage, buffer, strlen(buffer));
	finalMessage[strlen(finalMessage)] = '\0';
	printf("%s", finalMessage);
	//Ask the user to specify the operation. If an invalid operation is given, repeatedly prompt the user.
	while (strcmp(buffer, "open") != 0 && strcmp(buffer, "read") != 0 && strcmp(buffer, "write") != 0 && strcmp(buffer, "close") != 0) {
		printf("Invalid operation. Only open, read, write, and close are allowed.\nPlease enter what operation you are performing (open, read, write, or close):\n");
		getStdin(buffer, 256);
	}
	//If open is selected, get the pathname and flags.
	if (strcmp(buffer, "open") == 0) {
		printf("Please enter the pathname you want to open:\n");
		getStdin(buffer, 256);
		concatFinalMessage(finalMessage, buffer);
		//If an invalid flag is given, repeatedly prompt the user. Otherwise, convert them to their int form and concatenate them.
		while (strcmp(buffer, "O_RDONLY") != 0 && strcmp(buffer, "O_WRONLY") != 0 && strcmp(buffer, "O_RDWR") != 0) {
			printf("Please enter the flags for the file you want to open:\n");
			getStdin(buffer, 256);
			//RD_ONLY is 0, O_WRONLY is 1, and O_RDWR is 2.
			if (strcmp(buffer, "O_RDONLY") == 0) {
				concatFinalMessage(finalMessage, "0");
			} else if (strcmp(buffer, "O_WRONLY") == 0) {
				concatFinalMessage(finalMessage, "1");
			} else if (strcmp(buffer, "O_RDWR") == 0) {
				concatFinalMessage(finalMessage, "2");
			} else {
				printf("Invalid flags. Please try again.\n");
			}
		}
	//If write is selected, get the file descriptor and string to write to the file.
	} else if (strcmp(buffer, "write") == 0) {
		printf("Please enter the file descriptor of the file you want to write to:\n");
		getStdin(buffer, 256);
		concatFinalMessage(finalMessage, buffer);
		//Get number of bytes to write to file.
		while (atoi(buffer) < 1){
			printf("Please enter the number of bytes you wish to write to file:\n");
			getStdin(buffer, 256);
			if (atoi(buffer) < 1) {
				printf("Please enter a positive whole number.\n");
			}
		}
		int nbytes = atoi(buffer);
		printf("%d\n", nbytes);
		//Get string to write to file.
		printf("Please enter the string you wish to write to file:\n");
		getStdin(buffer, nbytes);
		concatFinalMessage(finalMessage, buffer);	
		memset(buffer,0,strlen(buffer));
		sprintf(buffer, "%d", nbytes);
		concatFinalMessage(finalMessage, buffer);
	} else if (strcmp(buffer, "read") == 0) {
		printf("Please enter the file descriptor of the file you want to read from:\n");
		getStdin(buffer, 256);
		concatFinalMessage(finalMessage, buffer);
		//Get number of bytes to read from file.
		while (atoi(buffer) < 1){
			printf("Please enter the number of bytes you wish to read from file:\n");
			getStdin(buffer, 256);
			if (atoi(buffer) < 1) {
				printf("Please enter a positive whole number.\n");
			}
		}
		int nbytes = atoi(buffer);
		//Get string to write to file.
		printf("Please enter the variable you wish to read from file into:\n");
		getStdin(buffer, nbytes);
		concatFinalMessage(finalMessage, buffer);
		memset(buffer,0,strlen(buffer));
		sprintf(buffer, "%d", nbytes);
		concatFinalMessage(finalMessage, buffer);
	} else if (strcmp(buffer, "close") == 0) {
		printf("Please enter the file descriptor of the file you want to close:\n");
		getStdin(buffer, 256);
		concatFinalMessage(finalMessage, buffer);
	}
	netopen("FUCK YOU", 0);
	netread(-384, "Yeh", 12);
	netwrite(-384, "Yeh", 12);
	netclose(-384);
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
