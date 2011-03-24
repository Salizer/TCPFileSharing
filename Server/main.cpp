/*
    Description: To start the TCP server.
    Author:
    Date: 15/03 -11
*/

#include <iostream>
#include "Server.h" // The class

using namespace std;

int main(int argc, char *argv[])
{
    printf("Current dir: %s\n", argv[0]);
    Server myServer(9000);
    while(1)
    {
        myServer.waitForClient();
    }

    return(0);
}

