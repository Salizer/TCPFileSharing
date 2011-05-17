#include "CRC-16.h"
#include "transportlag.h"
#include <cstdio> // TMP DELETE AFTER TESTING



////////////////////////Husk at indsætte begrænsninger//


void send(char * data , int size) // size = der skal sendes
{
	static char seq = 0;
	char crchigh;
	char crclow;
	char array[1028];
	char buffer[1028];

	crcCalc(data, size, crchigh, crclow);

	array[0] = crchigh;
	array[1] = crclow;
	array[2] = 0;
	array[3] = seq;

	for(int i = 4; i < size+4; i++)
	{
		array[i] = data[i-4];
	}

	do
	{
	sendPacket(array, size+4);

    // NOTICE: ONLY FOR TESTING. REMOVE AFTERWARDS
    printf("Data: %u %u %u %u %s\n", (unsigned char)array[0], (unsigned char)array[1], (unsigned char)array[2], (unsigned char)array[3], array + 4);
	while(!receivePackage(buffer))
        {}

	}while(!buffer[2] == 1 && buffer[3] == seq);

	seq = !seq;
}

int recieve(char * data)
{
	char buffer[1028];
	char crchigh;
	char crclow;
	int read = 0;

	do
	{
        while(!(read = receivePackage(buffer)))
            {}

        // NOTICE: ONLY FOR TESTING. REMOVE AFTERWARDS
        int high = (unsigned char)buffer[0];
        int low = (unsigned char)buffer[1];
        int type = (unsigned char)buffer[2];
        int seq = (unsigned char)buffer[3];

        printf("High: %i\nLow: %i\nType: %i\nSeq: %i\nData: %s\n", high, low, type, seq, buffer+4);

        crcCalc(buffer+4, read-4, crchigh, crclow);

        if(crchigh == buffer[0] && crclow == buffer[1]) // Pakke rigtig
        {
            char ack[] = {0,0,1,buffer[3]};
            sendPacket(ack, 4);
            break;
        }
        else // Fejl i pakke
        {
            char wrong[] =  {0,0,0,buffer[3]};
            sendPacket(wrong , 4);
        }
	}while(1);

	for(int i = 4; i < read; i++) // Kopier til 'data'
		data[i-4] = buffer[i];

	return(read);

	SLIPClose();
}

serialConnect(char* port, int baudRate)
{
   SLIPConnect(port, baudRate);
}

serialClose()
{
    SLIPClose();
}