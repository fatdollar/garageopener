#ifndef _ADMIN_H_
#define _ADMIN_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

class Admin
{
private:
	byte currentCode;
	byte codeCount;

public:
	Admin();
	bool edit(byte num);
	void remove(byte num);
	void lock();
	void reset();
};

#endif