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
unsigned long time, lockouttime;
volatile byte mode = CODEENTRY;
bool lockon = false;
#ifdef METRICS
unsigned long loopCount;
unsigned long startTime;
#endif

void setup()
{
    #if (METRICS | DEBUG)
	Serial.begin(9600);
    #endif
    #ifdef DEBUG
	showVersion();
    #endif
	attachInterrupt(0, wakeup, RISING);
    pinMode(GREENLED, OUTPUT);
    PINOFF(GREENLED);
    pinMode(REDLED, OUTPUT);
    PINOFF(REDLED);
    pinMode(SIGNALPIN, OUTPUT);
    PINOFF(SIGNALPIN);
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
				mode = ADMIN;
			}
            else if ( keys.key(0).stateChanged && keys.key(0).kstate == PRESSED )   // Only find keys that have changed state.
            {
            	if(keys.key(0).kchar == '#')
            	{
                    mode = WAIT;
            		if(pwd.checkCode())
                    {
                        PINON(GREENLED);
            			signal2Door();
                    }
                    else
                    {
                        PINON(REDLED);
                        if(pwd.lockout())
                        {
                            lockouttime =  millis();
                            mode = LOCKOUT;
                        }
                    }
            	}
            	else
        		{
        			pwd.addKey(keys.key(0).kchar);
        		}
            }
        }
        break;
        case ADMIN:
        break;
        case SLEEP:
        Serial.println("SLEEPING");
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        break;
        case ERROR:
        break;
        case WAIT:
        if((time + 1000) <= millis())
        {
            PINOFF(GREENLED);
            PINOFF(REDLED);
            mode = CODEENTRY;
        }
        break;
        case LOCKOUT:
        if((lockouttime + 30000) <= millis())
        {
            PINOFF(REDLED);
            lockon = false;
            mode = CODEENTRY;
            break;
        }
        if((time + 1000) <= millis())
        {
            time = millis();
            if(lockon)
            {
                PINON(REDLED);
                lockon = false;
            }
            else
            {
                PINOFF(REDLED);
                lockon = true;
            }
        }
        break;
        case ADMINLOCKED:
        break;
    }
}

#ifdef DEBUG
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
#endif

void signal2Door()
{
    PINON(SIGNALPIN);
    delay(500);
    PINOFF(SIGNALPIN);
}

void wakeup()
{
	//wake up here
	mode = CODEENTRY;
}