// Projekt        Patient overvågning
//
// Fil            PCcontroller.cpp
//
// Beskrivelse    Implementering af klassen PCcontroller.
//				  En klasse til serial kommunikation.
//
// Forfatter      Erik Gross Jensen
//
// Version        1.0 041200 EGJ - oprindelig version
//				  1.1 040202 EGJ - flere kommentarer tilføjet
//				  1.2 181108 NVJ - tilrettet 1. sem. projekt

#include "PCcontroller.h"
#include <cstring>
#include <stdlib.h>


//*************************************************************************
bool PCcontroller::open( int port, int baud )
// Åbner en seriel port for kommunikation. 
// Input:  port : Comport nummer 1,2,3,4 eller 5
//         baud : Hastigheden for kommunikationen: 9600, 19200 eller 38400
// Output: true hvis porten kunne åbnes
//		   false hvis porten ikke kunne åbnes. Når der returneres false skyldes
//         det ofte at et andet program bruger den serielle port
//*************************************************************************
{
	char portstr[5] = "COM ";

    portstr[3] = 48 + port ; 


	HComdev = CreateFile( portstr,  // Navnet på den port der skal åbnes(COMX)
						  GENERIC_READ | GENERIC_WRITE,  // read/write types
						  NULL,
						  0,
						  OPEN_EXISTING,
						  0,
						  0);

	if( HComdev == INVALID_HANDLE_VALUE )
		return false;
	else
	{
		//sæt hastighed m.m. 
		GetCommState( HComdev, &dcb );
		dcb.DCBlength = sizeof(DCB);

		//set baud rate
		if( baud == 9600 )
			dcb.BaudRate = CBR_9600;
		else if( baud == 19200 )
			dcb.BaudRate = CBR_19200;
		else if( baud == 38400 )
			dcb.BaudRate = CBR_38400;
		else 
			return false;

		// set databit 
		dcb.ByteSize = 8;
		
		//set paritet
		dcb.Parity = 0;

		//set stopbit
		dcb.StopBits = ONESTOPBIT;

		SetCommState( HComdev, &dcb );

    	return true;
	}
}


//******************************************
bool PCcontroller::close()
// Lukker forbindelsen
// Input:
// Output: true hvis forbindelsen blev lukket ellers false
//******************************************
{
	if( CloseHandle(HComdev) )
		return true;
	else
		return false;
}


//*************************************************
bool PCcontroller::send( char *sendPtr, int antal )
// Sender et antal karakterer på en seriel port. 
// Husk porten skal være åben inden send kan bruges
// Input: *sendPtr : en pointer til den char streng der skal sendes
//        antal    : Er det antal char der skal sendes 
// Output: true hvis afsendelse gik godt
//		   false hvis der ikke blev sendt noget. Når der returneres 
//		   false skyldes det ofte at porten ikke er åbnet
//*************************************************
{
 	DWORD dwBytesWritten;
    
	if( WriteFile(HComdev, sendPtr, antal, &dwBytesWritten, 0) )
		return true ;
	else 
		return false;
}


//*************************************************
int PCcontroller::inWatingBuffer()
// Tæller antallet af karakterer der findes i receive buffer
// Husk porten skal være åben inden send kan bruges
// Input:
// Output: returnerer antallet af karakterer der er findes i receive buffer
//*************************************************
{
	ClearCommError( HComdev, &dwErrorFlags, &ComStat );
	return ComStat.cbInQue ;
}


//*************************************************
int PCcontroller::receive( char *rxPtr )
// Henter de karakterer der findes i receive buffer. Bemærk hvis der 
// ingen karakterer er i receive buffer vil funktionen først returnere 
// når der kommer en karakter i  receive bufferen.
// Input: en pointer til et char array hvor data skal overføres til
// Output: returnerer det antal karakterer der er overført til rxPtr
//*************************************************
{
    DWORD byteRead,dwBytesRead;

	dwBytesRead = inWatingBuffer();
	ReadFile( HComdev, rxPtr, dwBytesRead, &byteRead, 0 );
	return byteRead;
}


//*************************************************
char PCcontroller::receiveOneChar()
// Læser 1 karakter fra receive buffer. Bemærk hvis der ingen karakterer
// er i receive buffer vil funktionen først returnere, når der kommer en 
// karakter i receive bufferen.
// Input:
// Output: Returnerer den læste karakter. 
//*************************************************
{
	char rxBuf;
    DWORD byteRead;
	
	ReadFile( HComdev, &rxBuf, 1, &byteRead, 0 );
	return rxBuf;
}

//*************************************************
// Laserfunktionerne samt funktionerne for motorens beveægelser
// Oprindeligt ikke en del af PCcontroller-klassen givet i 1. semester
//*************************************************
bool PCcontroller::laserOn()
{
	char tmp;
	char sendChar = '5'; // Sendchar(1)
	send(&sendChar, 1); // sender en charværdi
	tmp = receiveOneChar(); // modtag char og gem
	if(tmp == 1) // hvis tmp == 1, return true, else false
	return true;
	else 
	return false;
}

bool PCcontroller::laserOff()
{
	char temp;
	char sendChar = '5';
	send(&sendChar, 1); // 
	temp = receiveOneChar();
	if(temp == 0)
	return true;
	else 
	return false;
}

bool PCcontroller::turnLeft()
{
	char lol = '1';
	if(lol == '1')
	send(&lol, 1);
	return true;
}

bool PCcontroller::turnRight()
{
	char lol = '2';
	if(lol == '2')
	send(&lol, 1);
	return true;
}

bool PCcontroller::turnDown()
{
	char sendChar = '3';
	if(sendChar == '3')
	send(&sendChar, 1); // Turn right
	return true;
}

bool PCcontroller::turnUp()
{
	char sendChar = '4';
	if(sendChar == '4')
	send(&sendChar, 1); // Turn right
	return true;
}

bool PCcontroller::seek(int xkor, int ykor)
{	
	int const size = 5; // size for sending coordinates with null-termination
	char sendChar = '6'; 
	char x[size];
	char y[size];
	send(&sendChar, 1); // Send besked om anmodning
	itoa(xkor, x, 10); // Send koordinater vha. itoa(int value, char * str, int base);
	itoa(ykor, y, 10); // Send koordinater vha. itoa(int value, char * str, int base);
	
	int xlenght = (size-1)-strlen(x);
	int ylenght = (size-1)-strlen(y);

	x[size-1] = '\0';
	for(int i = 0; i < xlenght; i++)
	{
		for(int j = strlen(x); j > 0; j--)
		{
		x[j] = x[j-1];
		}
	x[0] = '0';
	}

	y[size-1] = '\0';
	for(int h = 0; h < ylenght; h++)
	{
		for(int g = strlen(y); g > 0; g--)
		{
		y[g] = y[g-1];
		}
	y[0] = '0';
	}
	
	// test
	if(x[0] == '+')
		x[0] = '1';
	else
		x[0] = '0';
	
	if(y[0] == '+')
		y[0] = '1';
	else
		y[0] = '0';

	send(x, size-1); // minus 1??
	send(y, size-1);
	while(!receiveOneChar() == '1');
	
	return true;
}