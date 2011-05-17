#ifndef TRANSPORTLAG_H
#define TRANSPORTLAG_H
#include "slip.h"

void send(char * data , int size/* size = der skal sendes */);
int recieve(char * data);
void serialConnect(char* port, int baudRate);
void serialClose();

#endif
