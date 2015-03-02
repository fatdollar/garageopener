#ifndef _COMMON_H_
#define _COMMON_H_

#define VERSION 0001

#define GREENLED 11
#define REDLED 12
#define SIGNALPIN 13
#define ANDGATEPIN 3

#define PINOUT(x) pinMode(x, OUTPUT)
#define PINON(x) digitalWrite(x, HIGH)
#define PINOFF(x) digitalWrite(x, LOW)

// #define METRICS	1
// #define DEBUG	1
// #define SLEEP	1

//operation modes
enum 
{
	CODEENTRY,//standard mode, listens for correct code entry
	ADMIN,//administration/code management
	#ifdef SLEEP
	SLEEP,//device sleeps to reduce power consumption
	#endif
	WAIT,//allow for signal to be on for a given time
	LOCKOUT,//break in deterent/security, temporary disabling
	ADMINLOCKED,//disable all but admin password
};

//admin modes
enum
{
	ENTRY,//only acces admin capabilities if admin code entered
	MAIN,//main selection menu
	EDIT,//add/edit codes
	DELETE,//disable codes
	LOCK,//enter ADMINLOCKED mode
	RESET,//reset device
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