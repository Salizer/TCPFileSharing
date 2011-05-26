#include "PCcontroller.h"
#include <iostream>
#include <conio.h>
using namespace std;


int main()
{
	PCcontroller myController;
	myController.open(4 , 9600);
	cout << "Connection to microcontroller is open:" << endl;
	cout << myController.seek(135,352) << endl;

	while(true)
	{
		int c = getch();
		if(c == 0x1B)
		{
			cout << "ESC read" << endl;
			break;
		}
		else
		{
			switch(c)
			{
			case 'a':
				myController.seek(352,252);
				cout << "Skriver koord ud" << endl;
				break;
				case 32:
					cout << "Turn laser on/off." << endl;
					myController.laserOff(); // Venter på feedback
					myController.laserOn(); // Venter på feedback
					break;
				case 75:
					cout << "Left arrow read." << endl;
					myController.turnLeft();
					break;
				case 77:
					cout << "Rigth arrow read." << endl;
					myController.turnRight();
					break;
				case 72:
					cout << "Up arrow read." << endl;
					myController.turnUp();
					break;
				case 80:
					cout << "Down arrow read." << endl;
					myController.turnDown();
					break;
			}	
		}
	}
	return 0;
}