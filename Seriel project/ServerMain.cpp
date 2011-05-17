#include "Server.h" // The class
/*
	Description: Main for using the Server class.
	Author: NSRD
	Date: 17/05 -11
*/
using namespace std;

int main(int argc, char *argv[])
{
    printf("Current dir: %s\n", argv[0]);
    Server myServer("/dev/ttyS1", V24_B9600); // VMWare port and baud rate of 9600
    while(1)
    {
        myServer.waitForClient();
    }

    return(0);
}

