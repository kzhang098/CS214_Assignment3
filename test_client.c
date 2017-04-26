
#include "libnetfiles.h"


//First argument will be the hostname (this is resolved using the gethostbyname() function)
//Port number will be the second argument. Make sure you convert this char string to integer using atoi() 
int main(int argc, char ** argv) {
	//Get standard input. The default number of bytes to read is 256.
	netserverinit(argv[1]);
	netopen("FUCK YOU", 0);
	netread(-384, "Yeh", 12);
	netwrite(-384, "Yeh", 12);
	netclose(-384);
	return 0; 
}
