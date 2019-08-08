/*
** implement utility functions for c string or cpp string
** author: hyw
** date: 2012.09.18
*/

#ifndef __FS_CHAR_H__
#define __FS_CHAR_H__

#include "fsmacros.h"

NS_FS_BEGIN

namespace _fs_char {

	inline bool isdigitChar(int chr) {
		return chr >= '0' && chr <= '9';
	}

	inline bool isupperChar(int chr) {
		return chr >= 'A' && chr <= 'Z';
	}

	inline bool islowerChar(int chr) {
		return chr >= 'a' && chr <= 'z';
	}

	inline int toupperChar(int chr) {
		return islowerChar(chr) ? chr - 32 : chr;
	}

	inline int tolowerChar(int chr) {
		return isupperChar(chr) ? chr + 32 : chr;
	}
};

NS_FS_END
#endif
