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

//memory map
enum
{
	CODE0,
	CODE1=CODE0+4,
	CODE2=CODE1+4,
	CODE3=CODE2+4,
	CODE4=CODE3+4,
	CODE5=CODE4+4,
	CODE6=CODE5+4,
	CODE7=CODE6+4,
	CODE8=CODE7+4,
	CODE9=CODE8+4,
	CODEADMIN=CODE9+4,
};

#endif