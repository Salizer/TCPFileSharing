/** \file Server.cpp
    \brief Implementation of the 'Server' class.
    \author NSR
    \date 22/03 -11
*/
#include "Server.h"

// Functions
// -Constructors and deconstructor
Server::Server(unsigned int port)
{
    // Initialization
    portNumber = port;

    // -Server socket
    memset(&serverInfo, 0, sizeof(serverInfo)); // 'Clean' the info. (Maybe unnecessary)
    serverInfo.sin_family = AF_INET; // Type of connection TCP/IP. (Needs more research)
    serverInfo.sin_addr.s_addr = INADDR_ANY; // Address interface. (Needs more research)
    serverInfo.sin_port = htons(portNumber); // Server port number

    // -Listening
    socketListener = socket(AF_INET, SOCK_STREAM, 0); // Prepare
    bind(socketListener, (sockaddr *)&serverInfo, sizeof(serverInfo)); // Bind listener to server socket

    // -Etc
    socketSize = sizeof(sockaddr_in);
}
Server::~Server()
{
    // Close sockets in case of malfunction.(Not sure what happens if already closed)
    close(socketListener);
    close(currentConnection);

    printf("Server closed\n");
}

// -Server functions
void Server::waitForClient()
{
    // Set to wait for '1' client
    listen(socketListener, 1);

    // Wait and connect
    printf("Waiting for client.\n");
    currentConnection = accept(socketListener, (sockaddr *)&clientInfo, (socklen_t*)&socketSize);

    // Do stuff
    // -Test
    printf("Someone connected from: %s\n", inet_ntoa(clientInfo.sin_addr)); // Someone connected
    //send(currentConnection, "Hello", 5, 0);

    // Receive file request
    recv(currentConnection, msgBuffer, BUFFERSIZE, 0); // Note: Assume is null terminated


    // Open and send file
    FILE* stream;

    if(stream = fopen(msgBuffer, "r"))
    {
        // Send info to client about file found and its size and ask if want it
        long lengthOfFile = getFileLength(stream);

        printf("User requested: %s with a size of %ld bytes\n", msgBuffer, lengthOfFile);

        // -Sending file size
        sprintf(msgBuffer, "%ld", lengthOfFile);
        send(currentConnection, msgBuffer, strlen(msgBuffer)+1, 0); // + 1 since want null terminator

        // -Get response
        recv(currentConnection, msgBuffer, BUFFERSIZE, 0); // Note: Assume is null terminated

        if(!strcmp(msgBuffer, "Ok")) // Returns '0' if equal
        {
            sendFile(stream, currentConnection, lengthOfFile);
        }

        // Cleaning
        fclose(stream);
    }
    else
    {
        // Can here send msg to client about file not found
        printf("Client tried to get: %s\n", msgBuffer);
        send(currentConnection, "0", 2, 0); // 2 in size since null terminated
    }

    // Close
    close(currentConnection);
    printf("Client connection closed\n");
}

// -Helper functions
long Server::getFileLength(FILE* file)
{
    fseek(file, 0L, SEEK_END); // Search for end of files
    long length = ftell(file);
    rewind(file); // Back to start of file

    return(length);
}
void Server::sendFile(FILE* file, int socket, long fileSize)
{
    puts("Sending file");
    size_t readFromFile = 0;

    // Start reading
    while(1)
    {
        readFromFile  = fread(msgBuffer, 1, BUFFERSIZE, file);

        // Send info
        send(socket, msgBuffer, readFromFile , 0);


        recv(currentConnection, msgBuffer, BUFFERSIZE, 0); // Note: Assume is null terminated

        if(!strcmp(msgBuffer, "Ok")) // Returns '0' if equal
        {
            puts("User received the package");
        }

        if(readFromFile  != BUFFERSIZE) // ALl read
            break;
    }
}