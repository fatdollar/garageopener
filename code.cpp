#include "code.h"

int Code::keycount = 0;
byte Code::code[4] = {0xFF,0xFF,0xFF,0xFF};

Code::Code()
{
	badcodecount = 0;
}

void Code::addKey(byte key)
{
	if(keycount == 4)
	{
		code[0]=code[1];
		code[1]=code[2];
		code[2]=code[3];
		code[3]=key;
	}
	else
	{
		code[keycount++]=key;
	}
}

byte  Code::checkCode()
{
	if(keycount != 4)
		return 2;
	for(int j=0; j<10; j++)
	{
		for(int i=0; i<4; i++)
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
						badcodecount++;
						resetCode();
						return 0;
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

bool Code::checkAdminCode()
{
	for(int i=0; i<4; i++)
	{
		if(code[i] != EEPROM.read(CODEADMIN+i))
			return false;
	}
	return true;
}

void Code::resetCode()
{
	keycount=0;
	code[0] = 0xFF;
	code[1] = 0xFF;
	code[2] = 0xFF;
	code[3] = 0xFF;
}

bool Code::lockout()
{
	if(badcodecount >= 5)
		return true;
	return false;
}

void Code::resetLockout()
{
	badcodecount = 0;
}