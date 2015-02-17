#include "admin.h"

Admin::Admin()
{
	currentCode = 0xFF;
}

bool Admin::edit(byte num)
{
	#ifdef DEBUG
		Serial.print("NUM ");
		Serial.println(num);
		Serial.print("currentCode ");
		Serial.println(currentCode);
	#endif
	if(currentCode == 0xFF)
	{
		Serial.print("code");
		currentCode = num;
		codeCount = 0;
		return false;
	}
	if(codeCount < 4)
	{
		Serial.print("digit");
		EEPROM.write(CODE0+(currentCode*4)+codeCount++,num);
		return false;
	}
	currentCode = 0xFF;
	return true;
}

void Admin::remove(byte num)
{
	
}

