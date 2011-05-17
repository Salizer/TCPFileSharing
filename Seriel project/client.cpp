/*
	Description: Client for TCP file sharing.
	Author: NSR
	Date: 17/05 -11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ezV24/ezV24.h>
#include "transportlag.h"

#define MAXRCVLEN 1024

int main(int argc, char *argv[])
{
    serialConnect("/dev/ttyS1", V24_B9600); // VMWare port and 9600 baud rate
    //------------------------------------------------------------------
    // Init variabler og stringe.
    char* filnavn = argv[1];
    char buffer[MAXRCVLEN + 1];

    int filelenght;


    //--------------------------------------------------------------------
    //
    filnavn = "miley.jpg";
    printf("Client started:\n");

	serialSend(filnavn, strlen(filnavn)+1);

	serialReceive(buffer);

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
    // Print and serialSend file.

    printf("Filesize received %s.\n", buffer);

    memset(&buffer, 0, sizeof(buffer)); /* Set memory to 0's */

    while(filelenght > MAXRCVLEN) /* Read and write to file while bytes are 1000 and above */
    {

	serialReceive(buffer);
    fwrite(buffer, 1, MAXRCVLEN ,fp); /* Write to file */

	memset(&buffer, 0, sizeof(buffer));

    filelenght = filelenght - MAXRCVLEN; /* Calculate remaining bytes */

    }

    // Manage recieving and writing to file when bytes is 1024 and below (Last part of file).
	serialReceive(buffer);
	fwrite(buffer, 1, MAXRCVLEN, fp);

    memset(&buffer, 0, sizeof(buffer));

	//-----------------------------------------------------------
    //

    printf("End of Program\n");

    fclose(fp); /* Close file */

    serialClose(); // Close connection

   return EXIT_SUCCESS;
}

