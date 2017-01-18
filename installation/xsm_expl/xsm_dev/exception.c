#include "exception.h"

#include <stdio.h>

static
xsm_exception
_exception;

int
exception_set (char *message, int exptype, int mode)
{
	_exception.message = message;
	_exception.type = exptype;
	_exception.mode = mode;

	return XSM_SUCCESS;
}

char*
exception_message ()
{
	return _exception.message;
}

int
exception_code()
{
	return _exception.type;
}

void
exception_set_ma (int address)
{
	_exception.ma = address;
}

void
exception_set_epn (int page)
{
	_exception.epn = page;
}

int
exception_get_ma()
{
	return _exception.ma;
}

int
exception_get_epn ()
{
	return _exception.epn;
}