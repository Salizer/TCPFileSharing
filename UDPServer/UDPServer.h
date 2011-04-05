#ifndef UDPSERVER_H
#define UDPSERVER_H

/** \file UDPServer.h
    \brief Contains an UDPServer class.(Header)
    \author NSR
    \date 04/04 -11
*/

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 100 // Size of buffer for messages(in/out)

class UDPServer
{
    // Fields
    private:
    sockaddr_in serverInfo; /** Info for the server */
    sockaddr_in currentConnection; /** Info for the client connected */
    char buffer[BUFFSIZE]; /** Buffer for in and out messages */
    int serverSocket; /** The socket the server listens on */
    int port; /** Port on which the server operates */


    // Functions
    public:
    // -Constructors and destructor
    UDPServer(int argPort); /** Initializes the server */
    ~UDPServer(); /** Cleaning */

    // -Client functions
    void waitForClient(); /** Waits for a client and 'serves' the client */

    // -Helper functions
    private:
    void getUptime(char* buff); /** Gets info from the file /proc/uptime and copies it to 'buff' */
    void getLoadAvg(char* buff); /** Gets info from the file /proc/loadavg and copies it to 'buff' */
};

#endif