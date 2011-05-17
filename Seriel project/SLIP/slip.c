#include "slip.h"

// Global defines
#define FRAME_CHAR 'A' // Start and end of data
#define FRAME_CHAR_SUB1 'B' // Substitute character 1 in case of FRAME_CHAR occuring in data
#define FRAME_CHAR_SUB2 'C' // Substitute character 2 in case of FRAME_CHAR occuring in data
#define SUB_SUB 'D' // If FRAME_CHAR_SUB1 occurs in data

// Global variables
v24_port_t* currentConnection = 0; // Used to keep track of connection

// Implementation of the functions in "slip.h"
int SLIPConnect(char* port, int baudRate)
{
	// Tmp variables
	int params = 0;

	// Open
	currentConnection = v24OpenPort(port, V24_STANDARD);
	if(currentConnection == 0) // Error occured
		return(0);

	// Settings
	/*
	 *	Baud rate: 9600
	 * 	Databit: 8 bit
	 * 	Parity bit generation: Disabled
	 */
	params = v24SetParameters(currentConnection, baudRate, V24_8BIT, V24_NONE);
	if(params!=V24_E_OK) // Error occured
	{
		v24ClosePort(currentConnection);
		return(0);
	}
	v24SetTimeouts (currentConnection, 50); // Timeout(So doesn´t waits forever in reading loop)(Sat to 5 seconds)

	return(1);
}

void SLIPClose()
{
	v24ClosePort(currentConnection);
}

void sendPackage(char* data, size_t n)
{
	// Check size
	if(n > DATA_MAX)
		return;

	// Start of package
	v24Putc(currentConnection, FRAME_CHAR);

	// Send data
	while(n) // For each byte
	{
		// Case of FRAME_CHAR or FRAME_CHAR_SUB1 in data(Else just send)
		switch(*data)
		{
			case FRAME_CHAR:
				v24Putc(currentConnection, FRAME_CHAR_SUB1);
				v24Putc(currentConnection, FRAME_CHAR_SUB2);
			break;
			case FRAME_CHAR_SUB1:
				v24Putc(currentConnection, FRAME_CHAR_SUB1);
				v24Putc(currentConnection, SUB_SUB);
			break;
			default: // Just send
				v24Putc(currentConnection, *data);
			break;
		}
		// Next byte
		data++;
		n--;
	}

	// End of package
	v24Putc(currentConnection, FRAME_CHAR);
}

size_t receivePackage(char data[])
{
	// Remember case of double FRAME_CHAR in start of new package: FRAME_CHAR --DATA-- FRAME_CHAR FRAME_CHAR --DATA-- FRAME_CHAR
	// Also remember not to give back FRAME_CHAR´s in start and end

	// Tmp variables
	int currentChar = 0; // Current char read from buffer('int' because checking for errors[-1])
	int specialChar = 0; // In case of byte stuffing('int' because checking for errors[-1])
	size_t index = 0; // Index for changing 'data'
	size_t read = 0; // How many bytes read


	// Get data
	while(1) // Until end of 'frame' encountered
	{
		// Read one char
		currentChar = v24Getc (currentConnection);

		// Check for error
		if(currentChar == -1)
			return(0);

		// Check if special char or just normal
		if((char)currentChar == FRAME_CHAR) // Case if beginning of package or end of package
		{
			if(read > 0) // Have read something so should be end of package
			    return(read);
		}
		else if((char)currentChar == FRAME_CHAR_SUB1)
		{
			specialChar = v24Getc (currentConnection); // Get next char
			switch((char)specialChar)
			{
				case FRAME_CHAR_SUB2:
					data[index] = FRAME_CHAR;
				break;
				case SUB_SUB:
					data[index] = FRAME_CHAR_SUB1;
				break;
				default: // Error occured(But still send, then upper-layer should decide what to do)
					data[index] = (char)specialChar;
					return(0);
				break;
			}
			read++;
			index++;
		}
		else // Normal
		{
			data[index] = (char)currentChar;
			read++;
			index++;
		}
	}
}
