// File: client.cpp
/*
	Description: Client for serial file sharing.
	Author: NSRD
	Date: 24/05 -11
*/

#include <stdio.h>  // file function etc.
#include <stdlib.h> //atoi function.
#include <string.h> // memset function.

#include "transportlag.h"

#define MAXRCVLEN 1024

int main(int argc, char *argv[])
{
    //------------------------------------------------------------------
    // Init variabler og stringe.
    char* filnavn = argv[1];
    char buffer[MAXRCVLEN + 1];

    int filelenght;
    //------------------------------------------------------------------
    // Connect to USB serial.
    serialConnect("/dev/ttyUSB0", V24_B9600); // USB port and 9600 baud rate
    //--------------------------------------------------------------------
    // Send requested filename, then recieve filelenght
    printf("Client started:\n");

	serialSend(filnavn, strlen(filnavn)+1); // Send filename.

	serialReceive(buffer); // Recieve filelenght in bytes as string

    filelenght = atoi(buffer); /* Convert string with bytes to integer */

    if(filelenght == 0) /* If file is not known by server */
    {
        printf("No such file, exiting.\n");
        exit(1); /* Close terminal */
    }

    //------------------------------------------------------------------------------
    // Open new file/existing with specified filename and write in binary.

    FILE *fp;
    fp = fopen(filnavn, "wb");

    //------------------------------------------------------------------------------
    // Print and serialSend file.

    printf("Filesize received %s.\n", buffer); // Print out the filelenght

    memset(&buffer, 0, sizeof(buffer)); /* Set memory to 0's */

    while(filelenght > MAXRCVLEN) /* Recieve and write to file while bytes are 1024 and above */
    {
        serialReceive(buffer); // Recieve more data

        fwrite(buffer, 1, MAXRCVLEN ,fp); /* Write to file */

        memset(&buffer, 0, sizeof(buffer));

        filelenght = filelenght - MAXRCVLEN; /* Calculate remaining bytes */

    }

    // Manage recieving and writing to file when bytes is 1024 and below (Last part of file).
	serialReceive(buffer);
	fwrite(buffer, 1, filelenght, fp);

    memset(&buffer, 0, sizeof(buffer));

	//-----------------------------------------------------------
    // End program.

    printf("End of Program\n");

    fclose(fp); /* Close file */

    serialClose(); // Close serial connection

   return EXIT_SUCCESS;
}

