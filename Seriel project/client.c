/*
Description: Client for TCP file sharing.
Author: NSR
Date: 24/03 -11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Standard symbolic constants and types
#include <arpa/inet.h> // Definitions for internet operations
#include <sys/types.h> // Defines Datatypes.
#include <netinet/in.h> // Defines Internet address family - TCP etc.
#include <sys/socket.h> // Defines Internet Protocol family.

#define MAXRCVLEN 1024
#define PORTNUM 9000

int main(int argc, char *argv[])

{
    //------------------------------------------------------------------
    // Init variabler og stringe.
    char* filnavn = argv[1];
    char buffer[MAXRCVLEN + 1];

    int filelenght;


    //--------------------------------------------------------------------
    // 

    printf("Client started:\n");
	
	send(filnavn, strlen(filnavn)+1);
	
	recieve(buffer);
	
    filelenght = atoi(buffer); /* Convert string with bytes to integer */

    if(filelenght == 0) /* If file is not know by server */
    {
        printf("No such file, exiting.\n");
        exit(1); /* Close terminal */
    }

    //------------------------------------------------------------------------------
    // Open new file/existing with specified filename and write in binary.

    FILE *fp;
    fp = fopen(filnavn, "wb");

    //------------------------------------------------------------------------------
    // Print and send file.

    printf("Filesize received %s.\n", buffer);
	
    memset(&buffer, 0, sizeof(buffer)); /* Set memory to 0's */

    while(filelenght > MAXRCVLEN) /* Read and write to file while bytes are 1000 and above */
    {
	
	recieve(buffer)
    fwrite(buffer, 1, MAXRCVLEN ,fp); /* Write to file */
    
	memset(&buffer, 0, sizeof(buffer));

    filelenght = filelenght - MAXRCVLEN; /* Calculate remaining bytes */

    }

    // Manage recieving and writing to file when bytes is 1024 and below (Last part of file).
	recieve(buffer);
	fwrite(buffer, 1, MAXRCVLEN, fp);
    
    memset(&buffer, 0, sizeof(buffer));
    
	//-----------------------------------------------------------
    // 

    printf("End of Program\n");

    fclose(fp); /* Close file */

   return EXIT_SUCCESS;
}

