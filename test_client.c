#include "libnetfiles.h"


//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 
int main(int argc, char ** argv) {
	//Get standard input. The default number of bytes to read is 256.
	
	int i;
	netserverinit(argv[1]);
	for (i = 0; i < 100; i++) {
		
		int fd = netopen("./Hints.txt", 2);
		netread(fd, "Yeh", 12);
		netwrite(fd, "This is a test\n", 14);
		netclose(34214123);
		fd = netopen("./Hints.txt", 2);
		netread(fd, "Yeh", 12);
		netwrite(fd, "This is a test\n", 14);
		netclose(fd);
	}
	return 0; 
}