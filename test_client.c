
#include "libnetfiles.h"


//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 
int main(int argc, char ** argv) {
	//Get standard input. The default number of bytes to read is 256.
	netserverinit(argv[1]);
	int fd = netopen("./Hints.txt", 2);
	netread(fd, "Yeh", 12);
	netwrite(fd, "This is a test", 14);
	netclose(fd);
	return 0; 
}
