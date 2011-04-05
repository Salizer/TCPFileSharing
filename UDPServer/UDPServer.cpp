/** \file UDPServer.h
    \brief Contains an UDPServer class.(Implementation)
    \author NSR
    \date 04/04 -11
*/

#include "UDPServer.h"
#include <cctype> // Used for "toupper(char*)"

// Functions
// -Constructors and destructor
UDPServer::UDPServer(int argPort)
{
    // Set fields
    // -Port
    port = argPort;
    // -Server
    memset(&serverInfo, 0, sizeof(serverInfo)); // Clear memory
    serverInfo.sin_family = AF_INET; // IPv4 protocol for adresses
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any IP not just for one interface(Ex from exercise where we have an interface with IP 10.0.0.x)
    serverInfo.sin_port = port; // The port to be used
    bind(serverSocket, (sockaddr*) &serverInfo, sizeof(serverInfo)); // Set up socket with the info from 'serverInfo'

    // -Client
    memset(&currentConnection, 0, sizeof(currentConnection)); // Clear memory

    // Info to user
    printf("Server up and running on port %d\n", port);
}
UDPServer::~UDPServer()
{
    // Cleaning
    close(serverSocket); // Releases binding for port

    // Info to user
    puts("Server closed");
}

// -Client functions
void UDPServer::waitForClient()
{
    // Variables
    ssize_t received = -1; // Length of what received from client
    size_t lengthClient = sizeof(currentConnection); // Length/size of currentConnection

    // Wait for client
    // -Info to user
    puts("Waiting for client...");

    // -Waiting
    while(received == -1)
        received = recvfrom(serverSocket, buffer, BUFFSIZE, 0, (sockaddr*) &serverInfo, &lengthClient);

    // -Info about who connected
    printf("Client with IP %s connected\n", currentConnection.sin_addr);

    // Check what client wrote and provide feedback(Send uptime or loadAvg)
    char command[2] = {(char) toupper(buffer[0]), '\0'}; // To upper so not depending on client writing in lower- or upper-case
    if(!strcmp(command, "U")) // uptime
    {
        // Get uptime and send it
        getUptime(buffer); // Get uptime and save in buffer

        sendto(serverSocket, buffer, strlen(buffer) + 1, 0, (sockaddr*) &currentConnection, sizeof(currentConnection)); // Send info about uptime(+1 after strlen because of '\0')
    }
    else if(!strcmp(command, "L")) // loadAvg
    {
        // Get loadAvg and send it
        getUptime(buffer); // Get loadAvg and save in buffer

        sendto(serverSocket, buffer, strlen(buffer) + 1, 0, (sockaddr*) &currentConnection, sizeof(currentConnection)); // Send info about loadAvgs(+1 after strlen because of '\0')
    }
    else // Not supported command
    {
        sprintf(buffer, "Command not supported\n");
        sendto(serverSocket, buffer, strlen(buffer) + 1, 0, (sockaddr*) &currentConnection, sizeof(currentConnection));
    }
}

// -Helper functions
void UDPServer::getUptime(char* buff)
{
    // Variables
    FILE* file;
    long uptime;

    // Open file
    if(!fopen("/proc/uptime", "rt")) // Open and check if error
    {
        puts("Could not open /proc/uptime");
        exit(1);
    }

    // Read file and save in buffer
    fgets(buffer, BUFFSIZE, file); // Reads from file into buffer

    // -A 'finer' output
    uptime = atol(buffer); // Converts string to long value
    sprintf(buffer, "%02ld:%02ld:%02ld", uptime/3600, (uptime%3600) / 60, (uptime%3600) - ((uptime %3600) / 60) * 60); // A more 'nice' output

    // Clean up
    fclose(file); // Closes file
}
void UDPServer::getLoadAvg(char* buff)
{
    // Variables
    FILE* file;

    // Open file
    if(!fopen("/proc/loadavg", "rt")) // Open and check if error
    {
        puts("Could not open /proc/loadavg");
        exit(1);
    }

    // Read file and save in buffer
    fgets(buffer, BUFFSIZE, file);

    // Clean up
    fclose(file); // Closes file
}