/*
	Description: Client for TCP file sharing.
	Author: NSR
	Date: 24/03 -11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // Standard symbolic constants and types
#include <arpa/inet.h>      // Definitions for internet operations
#include <sys/types.h>      // Defines Datatypes.
#include <netinet/in.h>     // Defines Internet address family - TCP etc.
#include <sys/socket.h>     // Defines Internet Protocol family.

#define MAXRCVLEN 1000
#define PORTNUM 9000

int main(int argc, char *argv[])

{
    //------------------------------------------------------------------
    // Init variabler og stringe.

    char* IP = argv[1];
    char* filnavn = argv[2];

    char buffer[MAXRCVLEN + 1];
    char okmes[] = "Ok";

    int len, mysocket, filelenght;

    //--------------------------------------------------------------------
    // Init og connect to ip/socket.

    struct sockaddr_in dest;

    mysocket = socket(AF_INET, SOCK_STREAM, 0); /* Opret socket */

    memset(&dest, 0, sizeof(dest));             /* Zero the struct */
    dest.sin_family = AF_INET;                  /* TCP protocol */
    dest.sin_addr.s_addr = inet_addr(IP);       /* Set destination IP number */
    dest.sin_port = htons(PORTNUM);             /* Set destination port number */

    connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)); /* Connect to server */

    //--------------------------------------------------------------------
    // Send filename and recieve total bytesize.

    printf("Client started:\n");

    send(mysocket, filnavn, strlen(filnavn) + 1, 0);    /* Send filnavn string */
    recv(mysocket, buffer, MAXRCVLEN, 0);               /* Recieve byte amount (string) */
    filelenght = atoi(buffer);                          /* Convert string with bytes to integer */

    if(filelenght == 0)                                 /* If file is not know by server */
    {
        printf("No such file, exiting.\n");
        close(mysocket);                                /* Close socket */
        exit(1);                                        /* Close terminal */
    }

    //------------------------------------------------------------------------------
    // Open new file/existing with specified filename and write in binary.

    FILE *fp;
    fp = fopen(filnavn, "wb");

    //------------------------------------------------------------------------------
    // Print and send file.

    printf("Filesize received %s.\n", buffer);
    memset(&buffer, 0, sizeof(buffer));                 /* Set memory to 0's */

    send(mysocket, okmes, strlen(okmes) + 1, 0);        /* Send Ok message */

    while(filelenght > MAXRCVLEN)                       /* Read and write to file while bytes are 1000 and above */
    {

    len = recv(mysocket, buffer, MAXRCVLEN, 0);         /* Recieve 1000 bytes */
    fwrite(buffer, 1, MAXRCVLEN ,fp);                   /* Write to file */
    printf("Received %s (%d bytes).\n", buffer, len);
    memset(&buffer, 0, sizeof(buffer));

    send(mysocket, okmes, strlen(okmes)+1, 0);          /* Send Ok to server */

    filelenght = filelenght - len;                      /* Calculate remaining bytes */

    }

    // Manage recieving and writing to file when bytes is 1000 and below (Last part of file).
    len = recv(mysocket, buffer, filelenght, 0);
    fwrite(buffer, 1, len, fp);
    printf("Received %s (%d bytes).\n", buffer, len);
    memset(&buffer, 0, sizeof(buffer));
    send(mysocket, okmes, strlen(okmes)+1, 0);

    //-----------------------------------------------------------
    // End program, close files and sockets.

    printf("End of Program\n");

    fclose(fp);                                 /* Close file */
    close(mysocket);                            /* Close socket */

   return EXIT_SUCCESS;
}

