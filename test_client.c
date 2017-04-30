#include "libnetfiles.h"


//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 
int main(int argc, char ** argv) {
	//Get standard input. The default number of bytes to read is 256.
	
	int i;
	netserverinit(argv[1], 0);
	for (i = 0; i < 50; i++) {
		char * test = malloc(50);
		int fd = netopen("./Hints.txt", 2);
		netread(fd, test, 12);
		test[12] = '\0';
		printf("%s\n", test);
		netwrite(fd, "This is a test\n", 15);
		netclose(fd);
		fd = netopen("./Hints.txt", 2);
		netread(fd, test, 12);
		test[12] = '\0';
		printf("%s\n", test);
		netwrite(fd, "This is a test\n", 14);
		netclose(fd);
	}
	return 0; 
}