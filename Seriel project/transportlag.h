#ifndef TRANSPORTLAG_H
#define TRANSPORTLAG_H
/*
	Description: Transport layer functions for serial communication.
	Author: NSRD
	Date: 17/05 -11
*/
#include "slip.h"

#define MAX_DATA_LENGTH = 1024; // Max amount of data able to send(If trying to send larger amount then it only sends this amount of bytes)

void serialSend(char * data , int size/* size = der skal sendes */);
int serialReceive(char * data);
void serialConnect(char* port, int baudRate);
void serialClose();

#endif
