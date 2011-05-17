#include "Server.h" // The class

using namespace std;

int main(int argc, char *argv[])
{
    printf("Current dir: %s\n", argv[0]);
    Server myServer;
    while(1)
    {
        myServer.waitForClient();
    }

    return(0);
}

