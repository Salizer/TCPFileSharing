// Projekt        Patient overvågning
//
// Fil            PCµcontroller.h
//
// Beskrivelse    Interface til klassen PCµcontroller.
//				  En klasse til serial kommunikation.
//
// Forfatter      Erik Gross Jensen
//
// Version        1.0 041200 EGJ - oprindelig version
//				  1.1 181108 NVJ - tilrettet 1. sem. projekt
// 				  1.2 140511 SLT - tilrettet til LTS

#pragma once

#include <Windows.h>
#include <iostream>
using namespace std;

class PCµcontroller
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
	PCµcontroller * PtrOn;
	PCµcontroller * PtrOff;
	PCµcontroller * PtrLeft; 
	PCµcontroller * PtrRight;
	PCµcontroller * PtrDown;
	PCµcontroller * PtrUp;
	PCµcontroller * PtrSeek;
};