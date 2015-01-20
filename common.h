#ifndef _COMMON_H_
#define _COMMON_H_

#define VERSION 0001

// #define METRICS	1

//operation modes
enum 
{
	CODEENTRY,
	ADMIN,
	SLEEP,
	ERROR,
	LOCKED,//disable all but admin password
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