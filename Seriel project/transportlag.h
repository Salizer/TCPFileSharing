#ifndef TRANSPORTLAG_H
#define TRANSPORTLAG_H
/*
	Description: Transport layer functions for serial communication.
	Author: NSRD
	Date: 17/05 -11
*/
#include "slip.h"

void serialSend(char * data , int size/* size = der skal sendes */);
int serialReceive(char * data);
void serialConnect(char* port, int baudRate);
void serialClose();

#endif
