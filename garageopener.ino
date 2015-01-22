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
byte res;
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
    PINOUT(GREENLED);
    PINOFF(GREENLED);
    PINOUT(REDLED);
    PINOFF(REDLED);
    PINOUT(SIGNALPIN);
    PINOFF(SIGNALPIN);
    PINOUT(ANDGATEPIN);
    PINOFF(ANDGATEPIN);
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
                    res = pwd.checkCode();
            		if(res == 1)
                    {
                        PINON(GREENLED);
            			signal2Door();
                    }
                    else if(res == 0)
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
        //set keypad row pins high
        PINOUT(8);
        PINON(8);
        PINOUT(10);
        PINON(10);
        PINOUT(6);
        PINON(6);
        //enable and gate
        PINON(ANDGATEPIN);
        Serial.println("SLEEP");
        delay(500);
        sleep_enable();
        attachInterrupt(0, wakeup_isr, HIGH);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        cli();
        // sleep_bod_disable();
        sei();
        Serial.println("SLEEP 1");
        delay(500);
        sleep_cpu();
        /* wake up here */
        sleep_disable();
        Serial.println("WAKEUP");
        PINOFF(ANDGATEPIN);
        mode=CODEENTRY;
        time = millis();
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
            pwd.resetLockout();
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

void wakeup_isr()
{
    sleep_disable();
    detachInterrupt(0);
}