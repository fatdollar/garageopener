// garageopener.ino
#include <avr/sleep.h>
#include "common.h"
#include "Keypad.h"
#include "keyinput.h"

KeyInput keys;
unsigned long time;
int codecount = 0;
byte code[4] = {0xFF,0xFF,0xFF,0xFF};
Key *keystates;
volatile byte mode = CODEENTRY;

void setup()
{
	attachInterrupt(0, wakeup, RISING);
	time = millis();
}

void loop()
{
	switch(mode)
	{
	case CODEENTRY:
		if((time + 120000) <= millis())
		{
			mode=SLEEP;
			//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		}
		keystates = keys.getKeys();
		for(int i=0; i<10; i++)
		{
			if(keystates[i].kstate == PRESSED)
			{
				code[codecount++]=keystates[i].kchar;
				break;
			}
			resetCode();
		}
		// check for easter egg
		break;
	case ADMIN:
		break;
	case SLEEP:
		break;
	case ERROR:
		break;
	case LOCKED:
		break;
	}
	if(codecount==4)
	{
		checkCode();
		resetCode();
	}

}

void checkCode()
{
	for(int i=0; i<4; i++)
	{
		if(code[i]!='1')
			return;
	}
	//success
	signal2Door();
}

void resetCode()
{
	codecount=0;
	code[0] = 0xFF;
	code[1] = 0xFF;
	code[2] = 0xFF;
	code[3] = 0xFF;
}

void signal2Door()
{
	
}

void wakeup()
{
	//wake up here
	mode = CODEENTRY;
}