#include "slip.h"

// Global defines
#define FRAME_CHAR 'A' // Start and end of data
#define FRAME_CHAR_SUB1 'B' // Substitute character 1 in case of FRAME_CHAR occuring in data
#define FRAME_CHAR_SUB2 'C' // Substitute character 2 in case of FRAME_CHAR occuring in data
#define SUB_SUB 'D' // If FRAME_CHAR_SUB1 occurs in data

// Global variables
v24_port_t* currentConnection = 0; // Used to keep track of connection

// Implementation of the functions in "slip.h"
int SLIPConnect()
{
	// Tmp variables
	int params = 0;
	
	// Open
	currentConnection = v24OpenPort("/dev/ttyusb0",V24_STANDARD);
	if(currentConnection == 0) // Error occured
		return(-1);
	
	// Settings
	/*
	 *	Baud rate: 9600
	 * 	Databit: 8 bit
	 * 	Parity bit generation: Disabled
	 */
	params = v24SetParameters(currentConnection,V24_B9600,V24_8BIT,V24_NONE);
	if(rc!=V24_E_OK) // Error occured
	{
		v24ClosePort(UsedPort);
		return(-1);
	}
	int v24SetTimeouts (currentConnection, 50); // Timeout(So doesn´t waits forever in reading loop)(Sat to 5 seconds)
}

void SLIPClose()
{
	v24ClosePort(UsedPort);
}

void sendPacket(char* data, size_t n)
{
	// Check size
	if(n > DATA_MAX)
		return;
	
	// Start of package
	24Putc(currentConnection, FRAME_CHAR);
	
	// Send data
	while(n) // For each byte
	{
		// Case of FRAME_CHAR or FRAME_CHAR_SUB1 in data(Else just send)
		switch(*data)
		{
			case FRAME_CHAR:
				24Putc(currentConnection, FRAME_CHAR_SUB1);
				24Putc(currentConnection, FRAME_CHAR_SUB2);
			break;
			case FRAME_CHAR_SUB1:
				24Putc(currentConnection, FRAME_CHAR_SUB1);
				24Putc(currentConnection, SUB_SUB);
			break;
			default: // Just send
				24Putc(currentConnection, *data);
			break;
		}
		// Next byte
		data++;
	}
	
	// End of package
	24Putc(currentConnection, FRAME_CHAR);
}

size_t receivePackage(char data[]);
{
	// Remember case of double FRAME_CHAR in start of new package: FRAME_CHAR --DATA-- FRAME_CHAR FRAME_CHAR --DATA-- FRAME_CHAR
	// Also remember not to give back FRAME_CHAR´s in start and end
	
	// Tmp variables
	char currentChar = 0;
	char specialChar = 0; // In case of byte stuffing
	size_t index = 0; // Index for changing 'data'
	size_t read = 0; // How many bytes read
	
	if(v24Getc(currentConnection) != FRAME_CHAR) // Error in data stream
	{
		// Ignore until next FRAME_CHAR encountered
		while(v24Getc (currentConnection) != FRAME_CHAR)
			{if(currentChar == -1) return(0);} // -1 means error in reading
			
		return(0);
	}
	
	// Get data
	while((currentChar = v24Getc (currentConnection)) != FRAME_CHAR) // Until end of 'frame' encountered
	{
		// Check for error
		if(currentChar == -1)
			return(0);
		
		// Check if special char or just normal
		if(currentChar == FRAME_CHAR_SUB1)
		{
			specialChar = v24Getc (currentConnection); // Get next char
			switch(specialChar)
			{
				case FRAME_CHAR_SUB2:
					data[index] = FRAME_CHAR;
				break;
				case SUB_SUB:
					data[index] = FRAME_CHAR_SUB1;
				break;
				default: // Error occured
					// Ignore until next FRAME_CHAR encountered
					while(v24Getc (currentConnection) != FRAME_CHAR)
						{if(currentChar == -1) return(0);} // -1 means error in reading
					return(0);
				break;
			}
		}
		else // Normal
			data[index] = currentChar;

		read++;
		index++;
	}
	return(read);
}