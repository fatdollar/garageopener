#include "admin.h"

////////////////////////////////////////
//Admin::Admin() - initialize current code to nothing
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
Admin::Admin()
{
	currentCode = 0xFF;
}

////////////////////////////////////////
//Admin::edit() - edit or add desired code
//
//Inputs: byte num
//			first time code number, the 4 code values
//
//Outputs: bool
//			true - finished
//			false - not done yet
////////////////////////////////////////
bool Admin::edit(byte num)
{
	#ifdef DEBUG
		Serial.print("NUM ");
		Serial.println(num);
		Serial.print("currentCode ");
		Serial.println(currentCode);
	#endif
	if(currentCode == 0xFF)//if no current code, set current code
	{
		currentCode = num;
		codeCount = 0;
		return false;//not done yet
	}
	if(codeCount < 4)//save key in appropriate place
	{
		EEPROM.write(CODE0+(currentCode*4)+codeCount++,num);
		return false;//not done yet
	}
	currentCode = 0xFF;//reset
	return true;//return successful code entry
}

////////////////////////////////////////
//Admin::remove() - erase code x
//
//Inputs: byte num
//			code number to erase
//
//Outputs: none
////////////////////////////////////////
void Admin::remove(byte num)
{
	for(int i=0; i<4; i++)
		EEPROM.write(CODE0+(num*4)+i,0xFF);
}