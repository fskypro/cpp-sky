/*
** implement convert between string and numeric
** author: hyw
** date: 2012.09.18
*/

#include "fs_convert.h"

NS_FS_BEGIN

// --------------------------------------------------------------------------------------
// 数值类型转换为字符串类型
// 参数 base，是指以 base 进制的格式返回
// 注意：有符号的只支持 10 进制，无符号的支持 2、8、10、16 进制
// --------------------------------------------------------------------------------------
namespace fs_str {
	// ------------------------------------------------------------------------
	// inner methods
	// ------------------------------------------------------------------------
	// 转换为二进制
	static void _binay2str(UINT64 value, string &str) {
		static const char buff[] = { '0', '1' };
		do {
			str.insert(0, 1,  buff[value & 1]);
			value >>= 1;
		} while (value > 0);
	}

	// 转换为 8、10、16 进制
	template<typename T>
	static void _nu2str(T value, const char *fmt, string &str) {
		if (*fmt != '\0') {
			char buff[64];
			FS_SNPRINTF(buff, sizeof(buff), fmt, value);
			str = buff;
		} else {
			_binay2str((UINT64)value, str);
		}
	}

	// ------------------------------------------------------------------------
	// 数值类型转换为字符串类型
	// ------------------------------------------------------------------------
	string nu2str(INT16 value) {
		string str;
		_nu2str(value, "%hd", str);
		return str;
	}

	string nu2str(UINT16 value, RADIX_BASE base) {
		static const char *fmts[] = {"", "%ho", "%hu", "%hx", "%hX"};
		string str;
		_nu2str(value, fmts[base], str);
		return str;
	}

	// -------------------------------------------------------
	string nu2str(INT32 value) {
		string str;
		_nu2str(value, "%d", str);
		return str;
	}

	string nu2str(UINT32 value, RADIX_BASE base) {
		static const char *fmts[] = {"", "%o", "%u", "%x", "%X"};
		string str;
		_nu2str(value, fmts[base], str);
		return str;
	}

	// -------------------------------------------------------
	string nu2str(INT64 value) {
		string str;
		_nu2str(value, "%lld", str);
		return str;
	}

	string nu2str(UINT64 value, RADIX_BASE base) {
		static const char *fmts[] = {"", "%llo", "%llu", "%llx", "%llX"};
		string str;
		_nu2str(value, fmts[base], str);
		return str;
	}

	// -------------------------------------------------------
	string nu2str(FLOAT value) {
		char buff[32];
		FS_SNPRINTF(buff, sizeof(buff), "%f", value);
		return buff;
	}

	string nu2str(DOUBLE value) {
		char buff[32];
		FS_SNPRINTF(buff, sizeof(buff), "%lf", value);
		return buff;
	}

	string nu2str(LDOUBLE value) {
		char buff[32];
		FS_SNPRINTF(buff, sizeof(buff), "%Lf", value);
		return buff;
	}
};

// --------------------------------------------------------------------------------------
// 字符串转换为数值类型
// 参数 base，是指以 base 进制的格式传入
// 转换成功则返回 true，否则返回 false
// --------------------------------------------------------------------------------------
namespace fs_str {
	// -------------------------------------------------------------------------------------
	// inner methods
	// -------------------------------------------------------------------------------------
	// 字符转化为数值，转换失败则返回 -1
	static int _char2nu(char c, RADIX_BASE base) {
		switch (base) {
		case RB_D:									// 十进制
			return c >= '0' && c <= '9' ? c - '0' : -1;
		case RB_X:
		case RB_x:									// 十六进制
			if (c >= '0' && c <= '9') { return c - '0'; }
			if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
			if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
			return -1;
		case RB_B: 									// 二进制
			return c == '0' ? 0 : (c == '1' ? 1 : -1);
		case RB_O:									// 八进制
			return c >= '0' && c <= '7' ? c - '0' : -1;
		}
		return -1;
	}

	// 数值进位
	static void _carryValue(UINT64 &value, RADIX_BASE base) {
		switch(base) {
			case RB_D: value *= 10; break;
			case RB_X:
			case RB_x: value <<= 4; break;
			case RB_B: value <<= 1; break;
			case RB_O: value <<= 3; break;
		}
	}

	// 解释正负号，返回 -1 表示遇到 “-” 号；返回 1 表示遇到 “+” 号；返回 0 表示没有正负号
	static int _parseSign(const string &str, string::const_iterator &iter) {
		if (*iter == '+') {
			++iter;
			return 1;
		} else if (*iter == '-') {
			++ iter;
			return -1;
		}
		return 0;
	}

	// 去除进制前缀
	static void _cutRadixPrefix(RADIX_BASE base, const string &str, string::const_iterator &iter) {
		if (*iter == '0') {
			++iter;
		}

		switch(base) {
		case RB_x:
		case RB_X:
			if (*iter == 'x' || *iter == 'X') {
				++iter;
			}
			break;
		case RB_B:
			if (*iter == 'b' || *iter == 'B') {
				++iter;
			}
			break;
		default:
			break;
		}
	}

	// 解释正整数部分，返回：-1 表示解释失败；0 表示遇到小数点；1 表示解释成功
	static int _parseInteger(RADIX_BASE base, const string &str, string::const_iterator &iter, UINT64 &value) {
		if (iter == str.end()) { return -1; }						// 空字符串

		int number;
		while (iter != str.end()) {
			number = _char2nu(*iter, base);
			if (number < 0) { 			// 解释字符串失败
				return *iter == '.' ? 0 : -1;
			}
			_carryValue(value, base);
			value += number;
			++iter;
		}
		return 1;
	}

	// -------------------------------------------------------------------------------------
	// 字符串转换为数值类型
	// -------------------------------------------------------------------------------------
	// long long
	bool str2rint64(const string &str, INT64 &value, RADIX_BASE base) {
		string::const_iterator iter = str.begin();
		if (iter == str.end()) { return false; }				// 空字符串

		UINT64 tmpValue = 0;
		int sign = _parseSign(str, iter);						// 解释正负号
		_cutRadixPrefix(base, str, iter);
		if (iter == str.end()) { return false; }

		int ret = _parseInteger(base, str, iter, tmpValue);
		if (ret == -1) { return false; }						// 解释失败返回 false

		// 遇到小数点
		if (ret == 0) {
			// 不是十进制数，不允许有小数点
			if (base != RB_D) { return false; }

			// 如果是十进制，则允许带小数点，但小数点后面的数会被忽略，并且小数点后的字符必须为数字
			while (++iter != str.end()) {
				if (_char2nu(*iter, base) < 0) {				// 小数点后面的字符不是数字
					return false;
				}
			}
		}
		value = sign < 0 ? -1 * tmpValue : tmpValue;
		return true;
	}

	// unsigned long long
	bool str2ruint64(const string &str, UINT64 &value, RADIX_BASE base) {
		string::const_iterator iter = str.begin();
		if (iter == str.end()) { return false; }				// 空字符串

		UINT64 tmpValue = 0;
		int sign = _parseSign(str, iter);						// 解释正负号
		if (sign == -1) { return false; }						// 无符号整型，不允许有 负号

		_cutRadixPrefix(base, str, iter);						// 去除进制前缀
		if (iter == str.end()) { return false; }

		int ret = _parseInteger(base, str, iter, tmpValue);
		if (ret == -1) { return false; }						// 解释失败

		// 遇到小数点
		if (ret == 0) {
			// 不是十进制数，不允许有小数点
			if (base != RB_D) { return false; }

			// 如果是十进制，则允许带小数点，但小数点后面的数会被忽略，并且小数点后的字符必须为数字
			while (++iter != str.end()) {
				if (_char2nu(*iter, base) < 0) {				// 小数点后面的字符不是数字
					return false;
				}
			}
		}
		value = tmpValue;
		return true;
	}

	// long double
	bool str2rldouble(const string &str, LDOUBLE &value) {
		string::const_iterator iter = str.begin();
		if (iter== str.end()) { return false; }				// 空字符串

		bool hasDot = false;
		LDOUBLE tmpValue = 0.0L;
		int sign = _parseSign(str, iter);					// 解释正负号

		if (iter == str.end()) { return false; }				// 只有加减号的空字符串

		// 整数部分解释
		while (iter != str.end()) {
			if (*iter >= '0' && *iter <= '9') {
				tmpValue = tmpValue * 10 + (*iter - '0');
				++iter;
				continue;
			}
			if (*iter == '.') {
				hasDot = true;
				break;
			}
			return false;
		}

		// 小数部分解释
		if (hasDot) {
			LDOUBLE dec = 0.1L;
			while (++iter != str.end()) {
				if (*iter >= '0' && *iter <= '9') {
					tmpValue += dec * (*iter - '0');
					dec *= 0.1;
				} else {
					return false;
				}
			}
		}
		value = sign < 0 ? -1 * tmpValue : tmpValue;
		return true;
	}

	// --------------------------------------------------------------
	// [unsigned] int
	bool str2rint32(const string &str, INT32 &value, RADIX_BASE base) {
		INT64 tmpValue;
		if (str2rint64(str, tmpValue, base)) {
			value = (INT32)tmpValue;
			return true;
		}
		return false;
	}

	bool str2ruint32(const string &str, UINT32 &value, RADIX_BASE base) {
		UINT64 tmpValue;
		if (str2ruint64(str, tmpValue, base)) {
			value = (UINT32)tmpValue;
			return true;
		}
		return false;
	}

	// [unsigned] short
	bool str2rint16(const string &str, INT16 &value, RADIX_BASE base) {
		INT64 tmpValue;
		if (str2rint64(str, tmpValue, base)) {
			value = (INT16)tmpValue;
			return true;
		}
		return false;
	}

	bool str2ruint16(const string &str, UINT16 &value, RADIX_BASE base) {
		UINT64 tmpValue;
		if (str2ruint64(str, tmpValue, base)) {
			value = (UINT16)tmpValue;
			return true;
		}
		return false;
	}

	// decemial
	bool str2rdouble(const string &str, DOUBLE &value) {
		LDOUBLE tmpValue;
		if (str2rldouble(str, tmpValue)) {
			value = (DOUBLE)tmpValue;
			return true;
		}
		return false;
	}

	bool str2rfloat(const string &str, FLOAT &value) {
		LDOUBLE tmpValue;
		if (str2rldouble(str, tmpValue)) {
			value = (FLOAT)tmpValue;
			return true;
		}
		return false;
	}
};


// --------------------------------------------------------------------------------------
// 字符串转换为数值类型
// 参数 base，是指以 base 进制的格式传入
// 转换成功则返回转换后的值，否则返回默认值
// --------------------------------------------------------------------------------------
namespace fs_str {
	// long long
	INT64 str2int64(const string &str, RADIX_BASE base, INT64 defv) {
		INT64 value;
		if (str2rint64(str, value, base))
			return value;
		return defv;
	}

	// unsigned long long
	UINT64 str2uint64(const string &str, RADIX_BASE base, UINT64 defv) {
		UINT64 value;
		if (str2ruint64(str, value, base))
			return value;
		return defv;
	}

	// long double
	LDOUBLE str2ldouble(const string &str, LDOUBLE defv) {
		LDOUBLE value;
		if (str2rldouble(str, value))
			return value;
		return defv;
	}
};



NS_FS_END
