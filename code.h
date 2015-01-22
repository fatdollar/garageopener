#ifndef _CODE_H_
#define _CODE_H_

#include <Arduino.h>
#include <EEPROM.h>

class Code
{
private:
	//memory map
	enum 
	{
		CODE0,
		CODE1=CODE0+4,
		CODE2=CODE1+4,
		CODE3=CODE2+4,
		CODE4=CODE3+4,
		CODE5=CODE4+4,
		CODE6=CODE5+4,
		CODE7=CODE6+4,
		CODE8=CODE7+4,
		CODE9=CODE8+4,
		CODEADMIN=CODE9+4,
	};
	static int keycount;
	static byte code[4];
	int badcodecount;

public:
	//functions
	Code();
	void addKey(byte key);
	byte checkCode();
	void resetCode();
	bool lockout();
	void resetLockout();
};

#endif