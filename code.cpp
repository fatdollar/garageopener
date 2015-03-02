#include "code.h"

int Code::keycount = 0;//no keys on start up
byte Code::code[4] = {0xFF,0xFF,0xFF,0xFF};//empty array
badcodecount = 0;//no bad tries on start up

////////////////////////////////////////
//Code::Code() - constructor
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
Code::Code()
{
}

////////////////////////////////////////
//Code::addKey() - add key press to code array and increment counter
//
//Inputs: byte key
//			character of key pressed
//
//Outputs: none
////////////////////////////////////////
void Code::addKey(byte key)
{
	if(keycount == 4)//if more than 4 keys pressed
	{
		//cycle key presses to last 4 pressed
		code[0]=code[1];
		code[1]=code[2];
		code[2]=code[3];
		code[3]=key;
	}
	else//add key to code and get ready for next
	{
		code[keycount++]=key;
	}
}

////////////////////////////////////////
//Code::checkCode() - check if code entered is valid
//
//Inputs: none
//
//Outputs: byte 0-2
//			0:	incorrect
//			1:	correct
//			2:	not enough keys pressed
////////////////////////////////////////
byte  Code::checkCode()
{
	if(keycount != 4)//not enough keys entered
		return 2;
	for(int j=0; j<10; j++)//check all 10 standard codes
	{
		for(int i=0; i<4; i++)//check all for bytes of each code
		{
			switch(j)
			{
				case 0:
					if(code[i] != EEPROM.read(CODE0+i))
					{
						j++;
						i=-1;
					}
				break;
				case 1:
					if(code[i] != EEPROM.read(CODE1+i))
					{
						j++;
						i=-1;
					}
				break;
				case 2:
					if(code[i] != EEPROM.read(CODE2+i))
					{
						j++;
						i=-1;
					}
				break;
				case 3:
					if(code[i] != EEPROM.read(CODE3+i))
					{
						j++;
						i=-1;
					}
				break;
				case 4:
					if(code[i] != EEPROM.read(CODE4+i))
					{
						j++;
						i=-1;
					}
				break;
				case 5:
					if(code[i] != EEPROM.read(CODE5+i))
					{
						j++;
						i=-1;
					}
				break;
				case 6:
					if(code[i] != EEPROM.read(CODE6+i))
					{
						j++;
						i=-1;
					}
				break;
				case 7:
					if(code[i] != EEPROM.read(CODE7+i))
					{
						j++;
						i=-1;
					}
				break;
				case 8:
					if(code[i] != EEPROM.read(CODE8+i))
					{
						j++;
						i=-1;
					}
				break;
				case 9:
					if(code[i] != '1')//EEPROM.read(CODE9+i))
					{
						badcodecount++;//code did not match any codes
						resetCode();
						return 0;//failure
					}
				break;
			}
		}
	}
	//success
	badcodecount = 0;
	resetCode();
	return 1;
}

////////////////////////////////////////
//Code::checkAdminCode() - check if admin code attempt is correct
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
bool Code::checkAdminCode()
{
	for(int i=0; i<4; i++)
	{
		if(code[i] != '2'/*EEPROM.read(CODEADMIN+i)*/)
			return false;
	}
	return true;
}

////////////////////////////////////////
//Code::resetCode() - reinitialize code for new entry
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
void Code::resetCode()
{
	keycount=0;
	code[0] = 0xFF;
	code[1] = 0xFF;
	code[2] = 0xFF;
	code[3] = 0xFF;
}

////////////////////////////////////////
//Code::lockout() - check if too many failed attempts
//
//Inputs: none
//
//Outputs: bool
//			true - too many attempts
//			false - not too many yet...
////////////////////////////////////////
bool Code::lockout()
{
	if(badcodecount >= 5)
		return true;
	return false;
}

////////////////////////////////////////
//Code::resetLockout() - set badcodecount to 0
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
void Code::resetLockout()
{
	badcodecount = 0;
}