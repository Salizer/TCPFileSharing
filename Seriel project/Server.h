#ifndef SERVER_H
#define SERVER_H
#include "transportlag.h"
#include <ezV24/ezV24.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024

class Server
{
    public:

    Server();
    ~Server();
    void waitForClient();

    private:

	char msgBuffer[BUFFERSIZE+1];
    long getFileLength(FILE* file);
    void sendFile(FILE* file, long fileSize);
};
#endif
