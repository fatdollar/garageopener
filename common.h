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
	CODEENTRY,
	ADMIN,
	#ifdef SLEEP
	SLEEP,
	#endif
	ERROR,
	WAIT,
	LOCKOUT,
	ADMINLOCKED,//disable all but admin password
};

//admin modes
enum
{
	ADD,
	EDIT,
	DELETE,
	LOCK,
};

#endif