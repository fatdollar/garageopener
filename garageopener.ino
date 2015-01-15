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
unsigned long loopCount;
unsigned long startTime;
String msg;

void setup()
{
	Serial.begin(9600);
	attachInterrupt(0, wakeup, RISING);
	time = millis();
}

void loop()
{
    // loopCount++;
    // if ( (millis()-startTime)>5000 ) {
    //     Serial.print("Average loops per second = ");
    //     Serial.println(loopCount/5);
    //     startTime = millis();
    //     loopCount = 0;
    // }
	if((time + 120000) <= millis())
		mode=SLEEP;
    switch(mode)
    {
    	case CODEENTRY:
		if(keys.getKeys())
		{
			time=millis();
	        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
	        {
	            if ( keys.key(i).stateChanged && keys.key(i).kstate == PRESSED )   // Only find keys that have changed state.
	            {
	                // switch (keys.key(i).kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
	                //     case PRESSED:
	                //     msg = " PRESSED.";
	                // break;
	                //     case HOLD:
	                //     msg = " HOLD.";
	                // break;
	                //     case RELEASED:
	                //     msg = " RELEASED.";
	                // break;
	                //     case IDLE:
	                //     msg = " IDLE.";
	                // }
	                // Serial.print("Key ");
	                code[codecount++] = keys.key(i).kchar;
	                Serial.print(keys.key(i).kchar);
	                // Serial.println(msg);
	            }
	            if(codecount == 4)
	            	checkCode();
	        }
			// check for easter egg
			bool zerokey = ((keys.key(0).kchar == '*' || keys.key(0).kchar == '9') && keys.key(0).kstate == HOLD);
			bool onekey = ((keys.key(1).kchar == '*' || keys.key(1).kchar == '9') && keys.key(1).kstate == HOLD);
			if(zerokey && onekey)
			{
				Serial.println("ADMIN");
				mode = ADMIN;
			}
	    }
		break;
		case ADMIN:
		resetCode();
		break;
		case SLEEP:
		Serial.println("SLEEPING");
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		break;
		case ERROR:
		break;
		case LOCKED:
		break;
	}
}

void checkCode()
{
	for(int i=0; i<4; i++)
	{
		if(code[i]!='1')
		{
			resetCode();
			return;
		}
	}
	//success
	signal2Door();
	resetCode();
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
	Serial.println("SUCCESS");
}

void wakeup()
{
	//wake up here
	mode = CODEENTRY;
}