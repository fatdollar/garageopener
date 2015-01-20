// garageopener.ino
#include <avr/sleep.h>
#include "common.h"
#include "EEPROM.h"
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
	showVersion();
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
            		checkCode();
            		break;
            	}
            	else
            	{
            		if(codecount < 4)
            		{
            			code[codecount++] = keys.key(0).kchar;
            			Serial.print(keys.key(0).kchar);
            		}
            	}
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

void checkCode()
{
	for(int j=0; j<10; j++)
	{
		for(int i=0; i<4; i++)
		{
			switch(j)
			{
				case 0:
					if(code[i] != EEPROM.read(CODE0+i))
					{
						j++;
						i=-1;
					}
				break;
				case 1:
					if(code[i] != EEPROM.read(CODE1+i))
					{
						j++;
						i=-1;
					}
				break;
				case 2:
					if(code[i] != EEPROM.read(CODE2+i))
					{
						j++;
						i=-1;
					}
				break;
				case 3:
					if(code[i] != EEPROM.read(CODE3+i))
					{
						j++;
						i=-1;
					}
				break;
				case 4:
					if(code[i] != EEPROM.read(CODE4+i))
					{
						j++;
						i=-1;
					}
				break;
				case 5:
					if(code[i] != EEPROM.read(CODE5+i))
					{
						j++;
						i=-1;
					}
				break;
				case 6:
					if(code[i] != EEPROM.read(CODE6+i))
					{
						j++;
						i=-1;
					}
				break;
				case 7:
					if(code[i] != EEPROM.read(CODE7+i))
					{
						j++;
						i=-1;
					}
				break;
				case 8:
					if(code[i] != EEPROM.read(CODE8+i))
					{
						j++;
						i=-1;
					}
				break;
				case 9:
					if(code[i] != '1')//EEPROM.read(CODE9+i))
					{
						Serial.print("FAILURE");
						resetCode();
						return;
					}
				break;
			}
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