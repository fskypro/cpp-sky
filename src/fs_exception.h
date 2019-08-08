/**
 * @file: fsstr_exception.h
 * @brief: 字符串相关异常
 * @author: hyw
 * @version: 1.0
 * @date: 2017-12-19
 */

#ifndef __FSSTR_EXCEPTION_H__
#define __FSSTR_EXCEPTION_H__

#include <stdexcept>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN

class FS_StrException: public std::exception { 
public:
	explicit FS_StrException(const string &buffer);

	FS_StrException(const string &buffer, int err);

	virtual ~FS_StrException() throw();

	virtual const char* what() const throw();

	int getErrCode() { return _code; }

private:
	void getBacktrace();

private:
	string  _buffer;
	int	 _code;
};

NS_FS_END

#endif
