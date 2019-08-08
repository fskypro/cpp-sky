/*
** implement convert between string and numeric
** author: hyw
** date: 2012.09.18
*/

#ifndef __FS_STR_CONVERT_H__
#define __FS_STR_CONVERT_H__

#include <string>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN

// ---------------------------------------------------------------------------------------
// 类型定义
// ---------------------------------------------------------------------------------------
namespace fs_str
{
	typedef short INT16;
	typedef unsigned short UINT16;
	typedef int INT32;
	typedef unsigned int UINT32;
	typedef long INT64;					// linux
	typedef unsigned long UINT64;		// linux
	typedef float FLOAT;
	typedef double DOUBLE;
	typedef long double LDOUBLE;

	// 进制定义，分别是：二进制、八进制、十进制、小写十六进制、大写十六进制
	enum RADIX_BASE { RB_B, RB_O, RB_D, RB_x, RB_X };
};

// --------------------------------------------------------------------------------------
// 数值类型转换为字符串类型
// 参数 base，是指以 base 进制的格式返回
// 注意：有符号的只支持 10 进制，无符号的支持 2、8、10、16 进制
// --------------------------------------------------------------------------------------
namespace fs_str {
	string nu2str(INT64 value);
	string nu2str(UINT64 value, RADIX_BASE base=RB_D);

	string nu2str(INT32 value);
	string nu2str(UINT32 value, RADIX_BASE base=RB_D);

	string nu2str(INT16 value);
	string nu2str(UINT16 value, RADIX_BASE base=RB_D);

	string nu2str(FLOAT value);
	string nu2str(DOUBLE value);
	string nu2str(LDOUBLE value);

};

// --------------------------------------------------------------------------------------
// 字符串转换为数值类型
// 参数 base，是指以 base 进制的格式传入
// 转换成功则返回 true，否则返回 false
// --------------------------------------------------------------------------------------
namespace fs_str {
	// [unsigned] long long
	bool str2rint64(const string &str, INT64 &value, RADIX_BASE base=RB_D);
	bool str2ruint64(const string &str, UINT64 &value, RADIX_BASE base=RB_D);

	// [unsigned] int
	bool str2rint32(const string &str, INT32 &value, RADIX_BASE base=RB_D);
	bool str2ruint32(const string &str, UINT32 &value, RADIX_BASE base=RB_D);

	// [unsigned] short
	bool str2rint16(const string &str, INT16 &value, RADIX_BASE base=RB_D);
	bool str2ruint16(const string &str, UINT16 &value, RADIX_BASE base=RB_D);

	// decemial
	bool str2rldouble(const string &str, LDOUBLE &value);
	bool str2rdouble(const string &str, DOUBLE &value);
	bool str2rfloat(const string &str, FLOAT &value);
};

// --------------------------------------------------------------------------------------
// 字符串转换为数值类型
// 参数 base，是指以 base 进制的格式传入
// 转换成功则返回转换后的值，否则返回默认值
// --------------------------------------------------------------------------------------
namespace fs_str {
	// [unsigned] long long
	INT64 str2int64(const string &str, RADIX_BASE base, INT64 defv=0L);
	inline INT64 str2int64(const string &str, INT64 defv=0L) {
		return str2int64(str, RB_D, defv);
	}

	UINT64 str2uint64(const string &str, RADIX_BASE base, UINT64 defv=0L);
	inline UINT64 str2uint64(const string &str, UINT64 defv=0L) {
		return str2uint64(str, RB_D, defv);
	}

	// [unsigned] int
	inline INT32 str2int32(const string &str, RADIX_BASE base, INT32 defv=0) {
		return (INT32)str2int64(str, base, defv);
	}
	inline INT32 str2int32(const string &str, INT32 defv=0) {
		return str2int32(str, RB_D, defv);
	};

	inline UINT32 str2uint32(const string &str, RADIX_BASE base, UINT32 defv=0) {
		return (UINT32)str2uint64(str, base, defv);
	}
	inline UINT32 str2uint32(const string &str, UINT32 defv=0) {
		return str2uint32(str, RB_D, defv);
	}

	// [unsigned] short
	inline INT16 str2int16(const string &str, RADIX_BASE base, INT16 defv=0) {
		return (INT16)str2int64(str, base, defv);
	}
	inline INT16 str2int16(const string &str, INT16 defv=0) {
		return str2int16(str, RB_D, defv);
	}

	inline UINT16 str2uint16(const string &str, RADIX_BASE base, UINT16 defv=0) {
		return (UINT16)str2uint64(str, base, defv);
	}
	inline UINT16 str2uint16(const string &str, UINT16 defv=0) {
		return str2uint16(str, RB_D, defv);
	}

	// decimal
	LDOUBLE str2ldouble(const string &str, LDOUBLE defv=0.0L);
	inline DOUBLE str2double(const string &str, DOUBLE defv=0.0) {
		return (DOUBLE)str2ldouble(str, defv);
	}
	inline FLOAT str2float(const string &str, FLOAT defv=0.0f) {
		return (FLOAT)str2ldouble(str, defv);
	}
};

NS_FS_END

#endif

