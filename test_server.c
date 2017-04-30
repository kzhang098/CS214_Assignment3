#include "test_server.h"

/*
* THE BELOW FUNCTIONS SHOULD BE MOVED TO LIBNETFILES LATER
*/

//Packet Structure: Function,File Descriptor^File Path;o_flags*buffer:nbyte? 

//Tip for delimeters: 
// " , " : Function 
// " ^ " : 




/*
Just a note: 

MODES

* 0 : Unrestricted
* 1 : Exclusive
* 2 : Transaction

FLAGS

* 0 : Read Only
* 1 : Write Only
* 2 : Read/Write

*/

int checkLogic(fileInfo * entry, char * filePath, int mode, int flag) {
		fileInfo * ptr = files;

		pthread_mutex_lock(&userLock); 
		
		int found = 0;
		
		while(ptr != NULL) {
			//Depending on the mode of the current request, I have to follow different logic
			switch(mode) {
				case 0: //If mode from client request is Unrestricted.
					if(strcmp(files->pathname,filePath) == 0) { //If the names match. 
						found = 1; 
						if(files->mode == 2) { //Is the file in transaction mode?
							//Can't do anything
							found = 0; // I set this back to zero since if 1, it would add to the end.
						} else {
							if(files->mode == 1 && (files->o_flags == 1 || files->o_flags == 2)) { // Check if in exclusive and in write OR read/write mode.
								//Client can only read then. Since with exclusive only ONE client can have read permissions at a time
							}
						}
					} 
					
					break;
				case 1: //Exclusive
					if(strcmp(files->pathname,filePath) == 0) { //Open?
						found = 1;
						if(files->mode == 2) { //Transaction Mode?
							//Can't do anything.
							found == 0;
						} else { //
							if(files->o_flags == 1 || files->o_flags == 2) { //Check if in write mode. If yes, then you cannot write. 
							
							}
						}
					}
				
					break;
				case 3:
			
				break;
			
			}
			
			ptr = ptr->next; 	
		}
		
		if(found) {
				//Then add to ptr since ptr is already at the end.
		}
		
		pthread_mutex_unlock(&userLock); 
		
		return 0; // Just temporary
}		

int parseMode(char * initMessage) {
	int mode; 
	char * char_mode = (char*)malloc(1); 
	memcpy(char_mode, &initMessage[13], 1);
	mode = atoi(char_mode);
	
	return mode; 
}

char ** tokenizeMessage(char* message) {
	char ** result = (char**)malloc(6*sizeof(char*)); 
		
	//Index values and array to store the indices. 

	int funcIndex, fdIndex, pathIndex, flagIndex, bufferIndex, lengthIndex;
	int indexArr[7]; 
	
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

	char * functionName = (char*)malloc(funcIndex + 1);
	char * fileDes = (char*)malloc(fdIndex + 1); 
	char * path = (char*)malloc(pathIndex + 1); 
	char * flags = (char*)malloc(flagIndex + 1);
	char * buffer = (char*)malloc(bufferIndex + 1);
	char * length = (char*)malloc(lengthIndex + 1); 
	
	int i;
	int value;  

	for(i = 0; i < 6; i++) {
		if(indexArr[i+1] - indexArr[i] != 1) {
			switch(i) {
				case 0:
					strncpy(functionName, &message[0], indexArr[1] - indexArr[0]);
					strcat(functionName, "\0");  
					break; 		
				case 1:
					value = indexArr[1] + 1;
					strncpy(fileDes, &message[value], indexArr[2] - indexArr[1] - 1);
					strcat(fileDes, "\0");
					break;  
				case 2:
					value = indexArr[2] + 1;
					strncpy(path, &message[value], indexArr[3] - indexArr[2] - 1);
					strcat(path, "\0");
					break; 
				case 3:
					value = indexArr[3] + 1;
					strncpy(flags, &message[value], indexArr[4] - indexArr[3] - 1);
					strcat(flags, "\0");
					break;
				case 4:
					value = indexArr[4] + 1;
					strncpy(buffer, &message[value], indexArr[5] - indexArr[4] - 1);
					strcat(buffer, "\0");
					break;
				case 5: 
					value = indexArr[5] + 1;
					strncpy(length, &message[value], indexArr[6] - indexArr[5] - 1); 
					strcat(length, "\0");
					break;
			}
		} else {
			
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

	printf("Yes?\n"); 	
	
	result[0] = functionName;
	result[1] = fileDes;
	result[2] = path;
	result[3] = flags; 
	result[4] = buffer;
	result[5] = length; 

	printf("About to free!!!?\n");
	
	return result;
}


int runCommands(clientInfo * client) {
	
	int socket = client->socketId; // This is the socket we are using to communicate with the client
	char buffer[256]; 
	char * error = (char*)malloc(64);
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER; 	

	int n; 
	
	memset(buffer,0, 256); 
    n = read(socket, buffer, 255); 
	printf("This is the message: %s\n", buffer);
			
	if (strncmp(buffer, "Initializing", 12) != 0) {
		char ** tokenizedBuffer = tokenizeMessage(buffer);
			if (strncmp(tokenizedBuffer[0], "open", 4) == 0) {
				
				//Check file existence. 
				
				/*
				FILE * file;
				file = fopen(tokenizedBuffer[0],"r");
				if(file == NULL) {
					sprintf(error, "%d", 4);
					write(*socket, error, 255,0); 
				} 
				*/
				//Check if path is a directory. If so, then write 3 which represents EISDIR error
				
				/*				
				if(isDir(tokenizedBuffer[2]) == 1) {
					printf("Error: %s\n", strerror(errno)); 
					char * response = (char*)malloc(64);
					char* error = malloc(64);
					sprintf(error, "%d", errno);
                    			strncat(response, "^", 1);
                    			strncat(response, error, strlen(error));
                    			write(*socket, response, strlen(response),0);
					return -1;
				}*/
			
				//if(access(tokenizedBuffer[2], R_OK)) {
					printf("Opening\n");
								
					int fd = -2 * open(tokenizedBuffer[2], atoi(tokenizedBuffer[3]));
					
					if(fd == 1) {
						printf("Error: %s\n", strerror(errno)); 
						char * response = (char*)malloc(15);
						char* error = malloc(10);
						sprintf(error, "%d", errno);
                        strncat(response, "`", 1);
                        strncat(response, error, strlen(error));
                        write(socket, response, strlen(response));
						
						free(response);
						free(error);
						
						return -1;
					}

					//THIS IS THE PART I'M ADDING FOR EXTENSION A
					
					// -----------------------------------------------------------------------
					
					//I should probably check the logic here.
					
					//Creating struct for EXTENSION A
					
					/*
					
					fileInfo * myfile = (fileInfo*)malloc(sizeof(fileInfo));
					memcpy(myfile->pathname, tokenizedBuffer[0], strlen(tokenizedBuffer[0]));
					myfile->mode = client->mode;
					myfile->o_flags = atoi(tokenizedBuffer[3]);
					myfile->next = NULL;
					
					*/
					
					// -----------------------------------------------------------------------
					
					char * strfd = malloc(10);
					sprintf(strfd, "%d", fd);
					printf("writing\n");
					n = write(socket, strfd, 255); 
					printf("Sent\n");
					
					free(strfd); 
					 
				/*
				} else {
				
					//The file does not have read permission. Throw error. 
					sprintf(error,"%d", 1); 
					write(*socket, error, 255,0); 
				} 
				*/
			} else if (strncmp(tokenizedBuffer[0], "read", 4) == 0) {
				printf("Reading\n");
				lseek(atoi(tokenizedBuffer[1]) / -2, 0, SEEK_SET);
				char * readBuffer = malloc(atoi(tokenizedBuffer[5]) + 5);
				
				//pthread_rwlock_rdlock(&rwlock);
				int bytesread = read(atoi(tokenizedBuffer[1]) / -2, readBuffer, atoi(tokenizedBuffer[5]));
				
				//THIS IS EBADF ERROR

				if(bytesread < 0) {
					printf("Error: %s\n", strerror(errno)); 
						char * response = (char*)malloc(15);
						char* error = malloc(10);
						sprintf(error, "%d", errno);
                        strncat(response, "`", 1);
                        strncat(response, error, strlen(error));
                        write(socket, response, strlen(response));
						
						free(response);
						free(error); 
						return -1;
				}

				//pthread_rwlock_unlock(&rwlock); 
				char * strread = malloc(10);
				sprintf(strread, "%d", bytesread);
				char * returnMessage = malloc(strlen(readBuffer) + strlen(tokenizedBuffer[4]));
				sprintf(returnMessage, "%s^%s", readBuffer, strread);
				printf("PRINTING %s\n", returnMessage);  
				n = write(socket, returnMessage, strlen(returnMessage)); 
				
				free(readBuffer); 
				free(strread); 
				free(returnMessage); 
			} else if (strncmp(tokenizedBuffer[0], "write", 5) == 0) {
				printf("Writing\n");
				lseek(atoi(tokenizedBuffer[1]) / -2, 0, SEEK_END);
				//pthread_rwlock_rdlock(&rwlock);
				int written = write(atoi(tokenizedBuffer[1]) / -2, tokenizedBuffer[4], atoi(tokenizedBuffer[5]));
				printf("Done writing\n");
				if(written < 0) {
					printf("Error: %s\n", strerror(errno)); 
						char * response = (char*)malloc(15);
						char* error = malloc(10);
						sprintf(error, "%d", errno);
                        strncat(response, "`", 1);
                        strncat(response, error, strlen(error));
                        write(socket, response, strlen(response));
						
						free(response);
						free(error); 
						
						return -1;
				} 

				//pthread_rwlock_unlock(&rwlock); 
				//printf("%s\n", tokenizedBuffer[4]); 
				char * strwritten = malloc(10);
				sprintf(strwritten, "%d", written);
				n = write(socket, strwritten, strlen(strwritten)); 
			} else if (strncmp(tokenizedBuffer[0], "close", 5) == 0) {
				printf("Closing\n");
				int success = close(atoi(tokenizedBuffer[1]) / -2);
				
				//ERROR HANDLING
				
				if(success < 0) {
					printf("Error: %s\n", strerror(errno)); 
						char * response = (char*)malloc(15);
						char* error = malloc(10);
						sprintf(error, "%d", errno);
                        strncat(response, "`", 1);
                        strncat(response, error, strlen(error));
                        write(socket, response, strlen(response));
						
						free(response);
						free(error); 
						return -1;
				}
		
				char * strsuccess = malloc(5);
				sprintf(strsuccess, "%d", success);
				n = write(socket, strsuccess, 255); 
				
				free(strsuccess); 
			}
			printf("These are the tokens: %s %s %s %s %s %s\n", tokenizedBuffer[0], tokenizedBuffer[1], tokenizedBuffer[2], tokenizedBuffer[3], tokenizedBuffer[4], tokenizedBuffer[5]);
			free(tokenizedBuffer); 
		} else { //set mode for this client. 
			int mode;
			mode = parseMode(buffer);
			client->mode = mode;
			printf("THIS IS THE MODE!!!!!! %d\n", client->mode);
			
		}
	return 0; 
}

void appendClient(clientInfo* client) {

	printf("Testing\n");
	clientInfo* ptr = clients;
	clientInfo* prev = NULL;
	if (clients == NULL) {
		printf("Put at front\n");
		client->next = NULL;
		clients = client;
		return;
	}
	printf("Not empty\n");
	while (ptr != NULL) {
		if (strcmp(ptr->IPAddress, client->IPAddress) == 0) {
			printf("Already exists\n");
			ptr->socketId = client->socketId;
			return;
		}
		printf("%s\n", ptr->IPAddress);
		prev = ptr;
		ptr = ptr->next;
	}
	printf("Appending something\n");
	client->next = NULL;
	prev->next = client;
	

}

int main(int argc, char ** argv) {

	int sockfd; //Stands for socket file descriptor
	int newsockfd;
	int portNum = 9001; 
	int n; 
	int clilen;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr; 
	//clients = (clientInfo *)malloc(sizeof(clientInfo)); 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

	if(sockfd < 0) {
		printf("Error. Could not open socket!"); 
		exit(1); 
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portNum); 

	int status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if(status < 0) {
		printf("Error binding to client");
		close(sockfd);
		exit(1);
	} 
	
	int i = 0;
	int clientNum = 0;
	int flag = 1;
	
	listen(sockfd, 1);
	 
	while (1) {	
			
			flag = 1;
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			
			printf("%d\n", newsockfd);
			
			if(newsockfd < 0) {
				printf("Could not accept connection from client"); 
			} else {
				clientInfo * cInfo = (clientInfo*)malloc(sizeof(clientInfo));
				cInfo->socketId = newsockfd; //Stores the socket into the struct. 
				cInfo->IPAddress = (char * )malloc(strlen(inet_ntoa(cli_addr.sin_addr)) + 2);
				sprintf(cInfo->IPAddress, "%s\0",inet_ntoa(cli_addr.sin_addr));
				cInfo->next = NULL; 
				printf("%s\n", cInfo->IPAddress);
				appendClient(cInfo);
				flag = pthread_create(&clientThreads[i], NULL, (void*)runCommands, cInfo); //Starts a new thread with the created struct  
				pthread_join(clientThreads[i], NULL);
				printf("%d\n", newsockfd); 
			}
			
			if(flag == 0) {
				i++; 
			} else  {
				printf("Hi.../");
			} 
	}
	return 0; 
}
