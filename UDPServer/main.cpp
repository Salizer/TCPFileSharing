/*
    Description: Main to run the UDPServer
    Author: NSR
    Date: 04/04 -11
*/

#include "UDPServer.h"
#include <iostream>

using namespace std;

int main()
{
    // Variables
    UDPServer server(9001);

    // Infinite wait for clients and 'serve' them
    while(1)
    {
        server.waitForClient();
    }

    return 0;
}
