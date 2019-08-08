/*
** implement string format
** author: hyw
** date: 2012.09.18
*/

#ifndef __FS_STR_FORMAT_H__
#define __FS_STR_FORMAT_H__

#include <string>
#include "fsmacros.h"
#include "fs_type/fs_type.h"
#include "fs_string.h"

using namespace std;

NS_FS_BEGIN

// --------------------------------------------------------------------------------------
// fs_str
// --------------------------------------------------------------------------------------
namespace fs_str
{
	// ---------------------------------------------------------------------------------------
	// FSFormatArg
	// ---------------------------------------------------------------------------------------
	class FSFormatArg {
	public:
		// numeric constructor
		template<typename T>
		FSFormatArg(T value) {
			static_assert(fs_type::is_numeric<T>::value, "FSFormatArg must be wraped a string or numeric!");
			m_strValue = nu2str(value);
		}

		// string constructor
		FSFormatArg(const string &value) : m_strValue(value) {}
		FSFormatArg(const char *value) : FSFormatArg(string(value)) {}
		FSFormatArg(char *value) : FSFormatArg(string(value)) {}

		// bool
		FSFormatArg(bool value) :   m_strValue(value ? "1" : "0") {}

	public:
		operator string() const {
			return m_strValue;
		}

	private:
		string m_strValue;

		friend ostream &operator<<(std::ostream &os, const FSFormatArg &value) {
			os << value.m_strValue;
			return os;
		}
	};

	// ---------------------------------------------------------------------------
	// 用字典参数格式化字符串
	// 字符串中的参数用左右大括号括起来，如：
	//     str = "aaa {xx} bbb { yyy } ccc {zzz} ddd"，如果 args 为 {"xx": 123, "yyy": 456}
	//     则输出为：aaa 123 bbb 456 ccc {zzz} ddd
	// 注意：
	//     1、如果 args 中不存在的参数，则原字符串对应该参数的地方，原样输出
	//     2、如果要输出一个 “{” 则连续输入两个 "{{"
	//     3、要输出一个 "}" 不需要连续写两个 “}}”
	// ---------------------------------------------------------------------------
	void format(string &strOut, const string &strFmt, const map<string, FSFormatArg> &args);

	inline string format(const string &strFmt, const map<string, FSFormatArg> &args) {
		string strOut;
		format(strOut, strFmt, args);
		return strOut;
	}

	template<typename T>
	inline void format(string &strOut, const string &strFmt, const map<string, T> &args) {
		format(strOut, strFmt, map<string, FSFormatArg>(args.begin(), args.end()));
	}

	template<typename T>
	inline string format(const string &strFmt, const map<string, T> &args) {
		string strOut;
		format(strOut, strFmt, map<string, FSFormatArg>(args.begin(), args.end()));
		return strOut;
	}
};

NS_FS_END
#endif
