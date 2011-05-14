// Projekt        Patient overv�gning
//
// Fil            PC�controller.h
//
// Beskrivelse    Interface til klassen PC�controller.
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

class PC�controller
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
	PC�controller * PtrOn;
	PC�controller * PtrOff;
	PC�controller * PtrLeft; 
	PC�controller * PtrRight;
	PC�controller * PtrDown;
	PC�controller * PtrUp;
	PC�controller * PtrSeek;
};