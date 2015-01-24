#ifndef _CODE_H_
#define _CODE_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

class Code
{
private:
	static int keycount;
	static byte code[4];
	int badcodecount;

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