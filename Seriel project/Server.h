#ifndef SERVER_H
#define SERVER_H
/*
	Description: Server class for file transfer over serial connection.
	Author: NSRD
	Date: 17/05 -11
*/
#include "transportlag.h"
#include <ezV24/ezV24.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024

class Server
{
    public:

    Server(char* port, int baudRate); // Note: Baud rate values can be found in 'ezV24' enum.
    ~Server();
    void waitForClient();

    private:

	char msgBuffer[BUFFERSIZE+1];
    long getFileLength(FILE* file);
    void sendFile(FILE* file, long fileSize);
};
#endif
