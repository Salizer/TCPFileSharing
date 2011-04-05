#include <sys/types.h>      // Defines Datatypes.
#include <sys/socket.h>     // Defines Internet Protocol family.
#include <netinet/in.h>     // Defines Internet address family - TCP etc.
#include <arpa/inet.h>      // Definitions for internet operations.
#include <netdb.h>          // Defines network database operations.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // Standard symbolic constants and types.
#include <string.h>         // Defines string operations.

#define buffersize 256

int main(int argc, char *argv[])
{
    ///////////////////////////////////////////////
    //Definition of variables and objects.
    int sock, n;
    unsigned int length;
    struct sockaddr_in server, from;    //Defined in netinet/in.h
    //struct hostent *hp;
    char buffer[buffersize+1];

    ///////////////////////////////////////////////
    //Check if the required arguments are there.
    if (argc != 3)
    {
    printf("Error in arguments\n");
    exit(1);
    }

    ///////////////////////////////////////////////
    //Create socket with UDP protocol and IP domain
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        printf("Error: Socket.\n");
        exit(1);
    }

   /////////////////////////////////////////////////
   //Set internet domain and IP address, port.
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server.sin_port = htons(9001);

   //hp = gethostbyname(argv[1]);
   //if (hp==0) error("Unknown host");
   //bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    length=sizeof(struct sockaddr_in);

    //////////////////////////////////////////////
    //Get input from user.
    printf("Please enter the message: ");
    bzero(buffer,buffersize);
    fgets(buffer,buffersize,stdin);

    /////////////////////////////////////////////
    //Send input from buffer to server.
    n=sendto(sock,buffer, strlen(buffer)+1,0,(const struct sockaddr *)&server,length);
    if (n < 0)
    {
        printf("Error: Sending.\n");
        exit(1);
    }

    memset(&buffer, 0, sizeof(buffer));

    ////////////////////////////////////////////
    //Recieve designated data from server.
    n = recvfrom(sock,buffer,buffersize,0,(struct sockaddr *)&from, &length);
    if (n < 0) printf("Error: Recieving.\n");

    ///////////////////////////////////////////
    //Print to display the recieved data.
    write(1,"Message recieved: ",17);
    write(1,buffer,n);
    putchar('\n');

    ///////////////////////////////////////////
    //Close socket
    close(sock);

   return 0;
}
