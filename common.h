#ifndef _COMMON_H_
#define _COMMON_H_

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