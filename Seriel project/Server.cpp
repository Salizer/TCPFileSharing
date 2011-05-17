#include "Server.h"

Server::Server()
{
    serialConnect( , );
}
Server::~Server()
{
    serialClose();
    printf("Server closed\n");
}

void Server::waitForClient()
{

    printf("Waiting for client.\n");

	recieve(msgBuffer);

    FILE* stream;

    if(stream = fopen(msgBuffer, "r"))
    {
        long lengthOfFile = getFileLength(stream);

        printf("User requested: %s with a size of %ld bytes\n", msgBuffer, lengthOfFile);

        sprintf(msgBuffer, "%ld", lengthOfFile);
		send(msgBuffer, strlen(msgBuffer)+1);


        sendFile(stream, lengthOfFile);

        fclose(stream);
    }
    else
    {
        printf("Client tried to get: %s\n", msgBuffer);
        send("0", 2);
    }

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
void Server::sendFile(FILE* file, long fileSize)
{
    puts("Sending file");
    size_t readFromFile = 0;

    // Start reading
    while(1)
    {
        readFromFile  = fread(msgBuffer, 1, BUFFERSIZE, file);

        // Send info
		send(msgBuffer, readFromFile);

        if(feof(file)) // ALl read
            break;
    }
}
