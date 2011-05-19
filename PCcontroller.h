// Projekt        Patient overv�gning
//
// Fil            PCcontroller.h
//
// Beskrivelse    Interface til klassen PCcontroller.
//				  En klasse til serial kommunikation.
//
// Forfatter      Erik Gross Jensen
//
// Version        1.0 041200 EGJ - oprindelig version
//				  1.1 181108 NVJ - tilrettet 1. sem. projekt

#pragma once

#include <Windows.h>
#include <iostream>
using namespace std;

class PCcontroller
{
public:
	bool open( int port, int baud );
	bool close();
	bool send( char *sendPtr, int antal );
	int receive( char *rxPtr );
	int inWatingBuffer();
	char receiveOneChar();
	bool laserOn();
	bool laserOff();
	bool turnLeft();
	bool turnRight();
	bool turnDown();
	bool turnUp();
	bool seek(int x, int y);
private:
	HANDLE HComdev;					//Def af handle til pc
	DWORD dwErrorFlags; 
	COMSTAT ComStat;
	DCB dcb;						//Def af variabel til DCB structen
};
