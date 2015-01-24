#ifndef _ADMIN_H_
#define _ADMIN_H_

#include <Arduino.h>
#include <EEPROM>
#include "common.h"

class Admin
{
	private:

	public:
		Admin();
		void add(byte num);
		void edit(byte num);
		void remove(byte num);
		void lock();
		void reset();
};

#endif