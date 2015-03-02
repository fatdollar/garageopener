// garageopener.ino
//built-in headers
#ifdef SLEEP
    #include <avr/sleep.h>
#endif
#include <EEPROM.h>

//libraries
#include "Keypad.h"

//project files
#include "common.h"
#include "keyinput.h"
#include "code.h"
#include "admin.h"

KeyInput keys;
Code pwd;
Admin admin;
Key *keystates;
unsigned long time, lockouttime;
volatile byte mode = CODEENTRY, adminMode = ENTRY;
byte res;
bool zerokey, onekey, lockon = false;
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
    PinOutOff(GREENLED);//enable green LED port 11
    PinOutOff(REDLED);//enable red LED port 12
    PinOutOff(SIGNALPIN);//enable door signal port 13
    #ifdef SLEEP
    PinOutOff(ANDGATEPIN);//enable AND Gate for sleep interrupt
    #endif
	time = millis();
}

void loop()
{
	#ifdef METRICS//loop timer for performance monitoring
    loopCount++;//increment counter every time through loop
    if ( (millis()-startTime)>5000 )//dispaly metrics every 5 seconds
    {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        //reinitialize for next 5 seconds
        startTime = millis();
        loopCount = 0;
    }
    #endif
    #ifdef SLEEP
    if((time + 120000) <= millis())//check for inactiviy - 2 minutes
    	mode=SLEEP;
    #endif
    #ifdef DEBUG
        if(millis()%2000 == 0)//print current mode every 2 seconds
        {
            Serial.print("MODE ");
            Serial.println(mode);
            Serial.print("ADMIN MODE ");
            Serial.println(adminMode);
        }
    #endif
    switch(mode)//handle input based on current mode
    {
    	case CODEENTRY:
        if(keys.getKeys())
        {
            time=millis();//update time so we don't go to sleep
            // check for easter egg
            zerokey = ((keys.key(0).kchar == '*' || keys.key(0).kchar == '9') && keys.key(0).kstate == HOLD);
            onekey = ((keys.key(1).kchar == '*' || keys.key(1).kchar == '9') && keys.key(1).kstate == HOLD);
            if(zerokey && onekey)//easter egg detected
            {
                #ifdef DEBUG
                Serial.println("ENTER ADMIN");
                #endif
                PINON(GREENLED);
                mode = ADMIN;//enter admin mode
            }
            //log key presses
            else if ( keys.key(0).stateChanged && keys.key(0).kstate == PRESSED )   // Only find keys that have changed state.
            {
                #ifdef DEBUG
                Serial.println(keys.key(0).kchar);
                #endif
                if(keys.key(0).kchar == '#')//if enter code key is pressed
                {
                    #ifdef DEBUG
                    Serial.println("CHECK CODE");
                    #endif
                    mode = WAIT;//enter wait mode if code is correct
                    res = pwd.checkCode();//check the code entered
                    if(res == 1)//correct code open door
                    {
                        PINON(GREENLED);
                        signal2Door();
                    }
                    else if(res == 0)//incorrect code
                    {
                        #ifdef DEBUG
                        Serial.println("BAD CODE");
                        #endif
                        PINON(REDLED);
                        if(pwd.lockout())//lock keypad after too many incorrect tries
                        {
                            #ifdef DEBUG
                            Serial.println("ENTER LOCKOUT");
                            #endif
                            lockouttime =  millis();//get lockout start time
                            mode = LOCKOUT;//enter lockout mode
                        }
                    }
                }
                else//if any other key log it
                {
                    pwd.addKey(keys.key(0).kchar);
                }
            }
        }
        break;
        case ADMIN:
        switch(adminMode)
        {
            case ENTRY:
            if(getAdminCode() == 1)//get admin password
            #ifdef DEBUG
            {
                Serial.println("ENTER ADMIN MAIN");
                adminMode = MAIN;
            }
            #else
                adminMode = MAIN;//enter admin main menu if password correct
            #endif
            break;
            case MAIN:
            if(keys.getKeys())//get key pressed for main menu selection
            {
                #ifdef DEBUG
                Serial.print("MAIN SELECTION ");
                Serial.println(keys.key(0).kchar);
                Serial.print("STATE ");
                Serial.println(keys.key(0).kstate);
                #endif
                //if 0-4
                if(keys.key(0).kchar > 0x30 && keys.key(0).kchar < 0x35 && keys.key(0).kstate == PRESSED)
                {
                    PINON(REDLED);
                    adminMode = keys.key(0).kchar - 0x2F;//2-5 are possible modes
                    delay(500);//wait so led is visible
                    PINOFF(REDLED);
                }
            }
            break;
            case EDIT:
            if(keys.getKeys())
            {
                //if 0-9
                if(keys.key(0).kchar >= 0x30 && keys.key(0).kchar <= 0x39 && keys.key(0).kstate == PRESSED)
                {
                    if(admin.edit(keys.key(0).kchar-0x30))//selected password changed
                    {
                        #ifdef DEBUG
                            Serial.println("SUCCESSFUL PASSWORD ENTRY");
                        #endif
                        adminMode = MAIN;//go back to main menu
                    }
                }
            }
            break;
            case DELETE:
            admin.remove(0);
            break;
            case LOCK:
            if(getAdminCode() == 1)
            {
                PINON(REDLED);
                PINOFF(GREENLED);
                mode = ADMINLOCKED;
                adminMode = ENTRY;
            }
            break;
            case RESET:
            default:
            adminMode = ENTRY;
            mode = CODEENTRY;
            break;
        }
        break;
        #ifdef SLEEP
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
        #endif
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
        if(getAdminCode() == 1)
        {
            PINON(GREENLED);
            mode = WAIT;
        }
        break;
    }
}

////////////////////////////////////////
//PinOutOff() - set pin to output and turn it off
//
//Inputs: byte pin
//          desired pin number
//
//Outputs: none
////////////////////////////////////////
void PinOutOff(byte pin)
{
    PINOUT(pin);
    PINOFF(pin);
}

#ifdef DEBUG
////////////////////////////////////////
//showVersion() - "splash screen" shows name, version and creator
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
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

////////////////////////////////////////
//signal2Door() - turns on signal pin for 0.5s
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
void signal2Door()
{
    PINON(SIGNALPIN);
    delay(500);
    PINOFF(SIGNALPIN);
}

////////////////////////////////////////
//getAdminCode() - checks input for correct admin code and handles incorrect attempts
//
//Inputs: none
//
//Outputs: byte
//          0:  wrong code, lockout
//          1:  correct code
//          2:  no code entered
////////////////////////////////////////
byte getAdminCode()
{
    if(keys.getKeys())//if keys are pressed
    {
        time = millis();//update sleep timer
        if ( keys.key(0).stateChanged && keys.key(0).kstate == PRESSED )
        {
            if(keys.key(0).kchar == '#')//if check code key
            {
                if(pwd.checkAdminCode())//check for correct code
                {
                    PINON(GREENLED);//success
                    return 1;
                }
                else
                {
                    PINON(REDLED);//incorrect, lockout
                    lockouttime =  millis();//set lockout time to now
                    mode = LOCKOUT;//enter lockoutmode
                    adminMode = ENTRY;//reset admin mode to password entry
                    return 0;
                }
            }
            else//log key press
            {
                pwd.addKey(keys.key(0).kchar);
                return 2;
            }
        }
    }
    return 2;//no activity
}

# ifdef SLEEP
////////////////////////////////////////
//wakeup_isr() - exit sleep mode
//
//Inputs: none
//
//Outputs: none
////////////////////////////////////////
void wakeup_isr()
{
    sleep_disable();
    detachInterrupt(0);
}
#endif