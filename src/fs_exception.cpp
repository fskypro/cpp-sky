/**
* @file: fsstr_exception.cpp
* @brief: 字符串相关异常
* @author: hyw
* @version: 1.0
* @date: 2017-12-19
*/

#include <execinfo.h>
#include <string.h>
#include <stdlib.h>
#include <cerrno>
#include "fsstr_exception.h"

NS_FS_BEGIN

FS_StrException::FS_StrException(const string &buffer)
:_buffer(buffer), _code(0)
{
//	getBacktrace();
}

FS_StrException::FS_StrException(const string &buffer, int err)
{
	_buffer = buffer + " :" + strerror(err);
	_code   = err;
//	getBacktrace();
}

FS_StrException::~FS_StrException() throw()
{
}

const char* FS_StrException::what() const throw()
{
	return _buffer.c_str();
}

void FS_StrException::getBacktrace()
{
	void * array[64];
	int nSize = backtrace(array, 64);
	char ** symbols = backtrace_symbols(array, nSize);

	for (int i = 0; i < nSize; i++)
	{
		_buffer += symbols[i];
		_buffer += "\n";
	}
	free(symbols);
}

NS_FS_END
