#ifndef _CODE_H_
#define _CODE_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

class Code
{
private:
	//variables
	static int keycount;//count of keys entered 0-4
	static byte code[4];//current code
	static int badcodecount;//number of incorrect tries before correct try 0-5

public:
	//functions
	Code();
	void addKey(byte key);
	byte checkCode();
	bool checkAdminCode();
	void resetCode();
	bool lockout();
	void resetLockout();
};

#endif