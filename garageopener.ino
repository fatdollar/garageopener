// garageopener.ino
//built-in headers
#include <avr/sleep.h>
#include <EEPROM.h>

//libraries
#include "Keypad.h"

//project files
#include "common.h"
#include "keyinput.h"
#include "code.h"


KeyInput keys;
Code pwd;
Key *keystates;
unsigned long time;
volatile byte mode = CODEENTRY;
#ifdef METRICS
unsigned long loopCount;
unsigned long startTime;
#endif

void setup()
{
	Serial.begin(9600);
	showVersion();
	attachInterrupt(0, wakeup, RISING);
	time = millis();
}

void loop()
{
	#ifdef METRICS
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }
    #endif

    if((time + 120000) <= millis())
    	mode=SLEEP;
    switch(mode)
    {
    	case CODEENTRY:
    	if(keys.getKeys())
    	{
    		time=millis();
			// check for easter egg
			bool zerokey = ((keys.key(0).kchar == '*' || keys.key(0).kchar == '9') && keys.key(0).kstate == HOLD);
			bool onekey = ((keys.key(1).kchar == '*' || keys.key(1).kchar == '9') && keys.key(1).kstate == HOLD);
			if(zerokey && onekey)
			{
				Serial.println("ADMIN");
				mode = ADMIN;
			}
            else if ( keys.key(0).stateChanged && keys.key(0).kstate == PRESSED )   // Only find keys that have changed state.
            {
            	if(keys.key(0).kchar == '#')
            	{
            		Serial.println("CHECK CODE");
            		if(pwd.checkCode())
            			signal2Door();
            		break;
            	}
            	else
        		{
        			pwd.addKey(keys.key(0).kchar);
        		}
            }
        }
        break;
        case ADMIN:
        // resetCode();
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

void showVersion()
{
	String ver = "Super G Door by Johnathan\nv";
	int temp = VERSION;
	for (int i=3; i>=0; i--)
	{
		ver += ((temp >> (8*i)&0xFF));
		if(i+=0)
			ver += '.';
	}
	Serial.println(ver);
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