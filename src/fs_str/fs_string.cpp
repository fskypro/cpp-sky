/*
** implement utility functions for cpp string
** author: hyw
** date: 2012.09.18
*/

#include <string.h>
#include <iostream>
#include <sstream>
#include "_fschar.h"
#include "fs_string.h"
#include "fsmacros.h"

NS_FS_BEGIN

// ---------------------------------------------------------------------------------
// fs_str
// ---------------------------------------------------------------------------------
namespace fs_str
{
	// -------------------------------------------------------------------
	// 字符串判别
	// -------------------------------------------------------------------
	// 判断一个字符串是否全部字符为数字
	bool isDigit(const string& str) {
		if (str.empty()) { return false; }
		string::const_iterator iter = str.begin();
		while(iter != str.end()) {
			if(!_fs_char::isdigitChar(*iter++))
				return false;
		}
		return true;
	}

	bool isWhiteSpace(const string& str) {
		const char* p = str.c_str();
		while(*p != 0) {
			if(*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n')
				return false;
			++p;
		}
		return true;
	}

	// -----------------------------------------------------------------------------
	// 大小写转换
	// -----------------------------------------------------------------------------
	string lower(const string& str) {
		string result;
		string::const_iterator iter = str.begin();
		while(iter != str.end()) {
			result.append(1, _fs_char::isupperChar(*iter) ?
				_fs_char::tolowerChar(*iter++) : *iter++);
		}
		return result;
	}

	string& lower(string& str) {
		string::iterator iter = str.begin();
		while(iter != str.end()) {
			if(_fs_char::isupperChar(*iter))
				*iter = _fs_char::tolowerChar(*iter);
			++iter;
		}
		return str;
	}

	// -------------------------------------------------------
	string upper(const string& str) {
		string result;
		string::const_iterator iter = str.begin();
		while(iter != str.end()) {
			result.append(1, _fs_char::islowerChar(*iter) ?
				_fs_char::toupperChar(*iter++) : *iter++);
		}
		return result;
	}

	string& upper(string& str) {
		string::iterator iter = str.begin();
		while(iter != str.end()) {
			if(_fs_char::islowerChar(*iter))
				*iter = _fs_char::toupperChar(*iter);
			++iter;
		}
		return str;
	}

	// ---------------------------------------------------------------------------
	// 去空格
	// ---------------------------------------------------------------------------
	string ltrim(const string& str) {
		char c;
		size_t pos = 0;
		size_t len = str.length();
		while(pos < len) {
			c = str[pos];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				++pos;
			} else {
				break;
			}
		}
		return str.substr(pos);
	}

	string& ltrim(string& str) {
		char c;
		size_t pos = 0;
		size_t len = str.length();
		while(pos < len) {
			c = str[pos];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				++pos;
			} else {
				break;
			}
		}
		str.erase(0, pos);
		return str;
	}

	string rtrim(const string& str) {
		char c;
		size_t len = str.length();
		if (len == 0) { return ""; }

		int pos = len - 1;
		while(pos >= 0) {
			c = str[pos];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				--pos; --len;
			} else {
				break;
			}
		}
		return str.substr(0, len);
	}

	string& rtrim(string& str) {
		char c;
		size_t len = str.length();
		if (len == 0) { return str; }

		int pos = len - 1;
		while(pos >= 0) {
			c = str[pos];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				--pos;
			} else {
				break;
			}
		}
		++pos;
		str.erase(pos, len - pos);
		return str;
	}

	string trim(const string& str) {
		string result(str);
		rtrim(result);
		return ltrim(result);
	}

	string& trim(string& str) {
		rtrim(str);
		return ltrim(str);
	}

	// ---------------------------------------------------------------------------
	// 替换
	// ---------------------------------------------------------------------------
	string& replace(string &str, const string& strOld, const string& strNew) {
		stringstream ss;
		size_t index = 0;
		size_t len = str.size();
		size_t site;
		while (index < len) {
			site = str.find(strOld, index);
			if(site == string::npos) {
				ss << str.substr(index);
				break;
			} else {
				ss << str.substr(index, site-index) << strNew;
				index = site + strOld.size();
			}
		}
		str = ss.str();
		return str;
	}

	string replace(const string &str, const string& strOld, const string& strNew) {
		string result(str);
		return replace(result, strOld, strNew);				// 期望编译器能将该临时变量优化为返回的临时变量
	}
};

NS_FS_END
