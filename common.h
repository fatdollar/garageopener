#ifndef _COMMON_H_
#define _COMMON_H_

#define VERSION 0001

#define GREENLED 11
#define REDLED 12
#define SIGNALPIN 13
#define ANDGATEPIN 3

#define PINOUT(x) pinMode(x, OUTPUT);Serial.print("OUT ");Serial.println(x)
#define PINON(x) digitalWrite(x, HIGH);Serial.print("ON ");Serial.println(x)
#define PINOFF(x) digitalWrite(x, LOW);Serial.print("OFF ");Serial.println(x)

// #define METRICS	1
#define DEBUG	1
// #define SLEEP	1

//operation modes
enum 
{
	CODEENTRY,
	ADMIN,
	#ifdef SLEEP
	SLEEP,
	#endif
	WAIT,
	LOCKOUT,
	ADMINLOCKED,//disable all but admin password
};

//admin modes
enum
{
	ENTRY,
	MAIN,
	EDIT,
	DELETE,
	LOCK,
	RESET,
};

//memory map
enum 
{
	CODESUSED,//bit encoded bool for active codes, 2bytes
	CODE0=CODESUSED+1,//10 4-byte user codes
	CODE1=CODE0+4,
	CODE2=CODE1+4,
	CODE3=CODE2+4,
	CODE4=CODE3+4,
	CODE5=CODE4+4,
	CODE6=CODE5+4,
	CODE7=CODE6+4,
	CODE8=CODE7+4,
	CODE9=CODE8+4,
	CODEADMIN=CODE9+4,//1 4-byte admin code
};

#endif