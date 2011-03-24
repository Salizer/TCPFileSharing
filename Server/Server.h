#ifndef SERVER_H
#define SERVER_H
/** \file Server.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFERSIZE 1000

/**
    \brief The server. Port: 9000.
    \author NSR
    \date 15/03 -11
*/
class Server
{
    // Fields
    private:
    unsigned int portNumber;
    sockaddr_in serverInfo; // Settings for the server
    sockaddr_in clientInfo; // Info about the client
    int socketListener; // Used to listen for connection
    int currentConnection; // ID of the current connection
    int socketSize; // Size of socket.(Needs more description why)
    char msgBuffer[BUFFERSIZE];

    // Functions
    public:
    // -Constructors and deconstructor
    Server(unsigned int port);
    ~Server(); // Close sockets

    // -Server functions
    void waitForClient(); // Wait for connection and serv the client

    // -Prototypes for helper functions
    private:
    /**
        \brief Returns length of file
        \pre 'file' is pointing to an opened file.
        \post The length of the file is returned.
    */
    long getFileLength(FILE* file);
    /**
        \brief Sends file to the client in the end of the 'socket'.
        \pre 'file' is pointing to an opened file and 'fileSize' > 0.
        \post File is sent to the client in packages of 1 kB.
    */
    void sendFile(FILE* file, int socket, long fileSize);
};
#endif