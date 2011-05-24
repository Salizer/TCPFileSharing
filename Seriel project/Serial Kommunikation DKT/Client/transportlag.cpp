// File: transportlag.cpp
/*
	Description: Transportlag for serial file sharing.
	Author: NSR
	Date: 24/05 -11
*/

#include "CRC-16.h"
#include "transportlag.h"
#include <cstdio> // printf function.


void serialSend(char * data , int size) // size = der skal sendes
{
	// Size checking
	if(size > MAX_DATA_LENGTH)
		size = MAX_DATA_LENGTH;

    // Initiate used variables etc.
	static char seq = 0;
	char crchigh;
	char crclow;
	char array[1028];
	char buffer[1028];

	// Calculate the CRC16
	crcCalc(data, size, crchigh, crclow);

    // Define each part of transportlayer into an array.
	array[0] = crchigh;
	array[1] = crclow;
	array[2] = 0;   // Data to be sent
	array[3] = seq;

    // Fill up the rest of the array(1024) with the data to be sent.
	for(int i = 4; i < size+4; i++)
	{
		array[i] = data[i-4];
	}

	do   // This has to be done once, no matter what.
	{
	sendPackage(array, size+4); // Send crc+data+seq+databit (everything)

    // Print sent data.
    printf("Data: %s\n", array + 4);

	while(!receivePackage(buffer)) // Wait till we get something back from other end.
        {}

	}while(!buffer[2] == 1 && buffer[3] == seq); // Keep doing before mentioned till we get awknowledge and the seq are the same as we sent.

	seq = !seq; // Keep changing seq after each succesfull delivered package, to make sure that the old/new isnt alike this in the checks. (to prevent packet loss)
}

int serialReceive(char * data)
{
    // Initiate variables
	char buffer[1028];
	char crchigh;
	char crclow;
	int read = 0;

    // Keep recieving data till CRC is correct on own/sent calculated CRC.
	do
	{
        while(!(read = receivePackage(buffer))) // Wait to recieve something
            {}

        // Print out recieved data.
        printf("Data: %s\n", buffer+4);

        // Calculate CRC16 from the recieved info
        crcCalc(buffer+4, read-4, crchigh, crclow);

        if(crchigh == buffer[0] && crclow == buffer[1]) // Hvis pakke er rigtig
        {
            char ack[] = {0,0,1,buffer[3]};     // Make array with ack with seq number.
            sendPackage(ack, 4);                // Send the ack package back to sender.
            break;                              // Break out of do while(1)
        }
        else // Fejl i pakke
        {
            char wrong[] =  {0,0,0,buffer[3]};  // If recieved package is wrong, make new package with no acknowledge, but with recieved seq
            sendPackage(wrong , 4);             // Send the wrong package back to sender.
        }
	}while(1);

	for(int i = 4; i < read; i++) // Copy recieved to 'data'
		data[i-4] = buffer[i];

	return(read-4);  // Return only data bytes recieved.
}

void serialConnect(char* port, int baudRate)
{
   SLIPConnect(port, baudRate); // Connect through slip with certain port/boudrate.
}

void serialClose()
{
    SLIPClose(); // Close slip connection
}
