// File: Server.cpp
/*
	Description: Server class for file transfer over serial connection.
	Author: NSRD
	Date: 17/05 -11
*/

#include "Server.h"

Server::Server(char* port, int baudRate)
{
    serialConnect(port, baudRate); // Connect to serial with defined port and baudrate.
}
Server::~Server()
{
    serialClose(); // Close serial connection.
    printf("Server closed\n");
}

void Server::waitForClient()
{

    printf("Waiting for client.\n");

	serialReceive(msgBuffer); // Recieve filename from client.

    FILE* stream;

    if(stream = fopen(msgBuffer, "r"))      // If opening a file with the name recieved is succesfull continue
    {
        long lengthOfFile = getFileLength(stream);      // Get file length of opened file in bytes.

        printf("User requested: %s with a size of %ld bytes\n", msgBuffer, lengthOfFile); // Print out filename and lenght of file.

        sprintf(msgBuffer, "%ld", lengthOfFile); // Convert lenght of file to string.

		serialSend(msgBuffer, strlen(msgBuffer)+1); // Send lenght of file


        sendFile(stream, lengthOfFile);     // Use function to send file, giving pointer and lenght of the file to send.

        fclose(stream);  // Close file.
    }
    else // If file couldnt be found, print out message and send to client 0 filelenght (its not found).
    {
        printf("Client tried to get: %s\n", msgBuffer);
        serialSend("0", 2);
    }

    printf("Client connection closed\n");
}

// -Helper functions
long Server::getFileLength(FILE* file)
{
    fseek(file, 0L, SEEK_END); // Search for end of file.
    long length = ftell(file); // Returns the current value of the file-position indicator for the stream measured in bytes from the beginning of the file.
    rewind(file); // Back to start of file

    return(length); // Return the lenght.
}
void Server::sendFile(FILE* file, long fileSize)
{
    puts("Sending file");
    size_t readFromFile = 0;

    // Start reading
    while(1)
    {
        readFromFile  = fread(msgBuffer, 1, BUFFERSIZE, file);

        // serialSend info
		serialSend(msgBuffer, readFromFile);

        if(feof(file)) // If everything has been read, break out of while loop.
            break;
    }
}
