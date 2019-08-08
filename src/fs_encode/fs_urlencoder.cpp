/*
** implement url encode
** author: hyw
** date: 2018.03.15
*/

#include <sstream>
#include "fs_urlencoder.h"

NS_FS_BEGIN
namespace fs_encode {
	// --------------------------------------------------------------------------------------
	// inner
	// --------------------------------------------------------------------------------------
	// 规则：
	//     1、数字和字母原样输出
	//     2、以下四特殊字符原样输出：-   _   .   ~
	//     3、空格转换为 “+” 号
	//     4、其他符号转换为 % + 十六进制数值
	static const char originCharsMarks[] = {
	//	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, // 5
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // F
	};

	// -----------------------------------------------------------------
	static unsigned char toHex(unsigned char x) {
		return  x > 9 ? x + 55 : x + 48;
	}

	static unsigned char fromHex(unsigned char x) {
		if (x >= 'A' && x <= 'Z') { return x - 'A' + 10; }
		else if (x >= 'a' && x <= 'z') { return x - 'a' + 10; }
		else if (x >= '0' && x <= '9') { return x - '0'; }
		return x;
	}

	// --------------------------------------------------------------------------------------
	// public
	// --------------------------------------------------------------------------------------
	std::string UrlEncoder::encode(const std::string& str) {
		std::stringstream ss;
		std::string::const_iterator iter = str.begin();
		char mark;
		while (iter != str.end()) {
			mark = originCharsMarks[(unsigned char)*iter];
			if (mark == 1) {
				ss << *iter;
			} else if (*iter == ' ') {
				ss << '+';
			} else {
				ss << '%' << toHex((unsigned char)(*iter) >> 4) << toHex((unsigned char)(*iter) % 16);
			}
			++iter;
		}
		return ss.str();
	}

	std::string UrlEncoder::decode(const std::string& str) {
		std::stringstream ss;
		std::string::const_iterator iter = str.begin();
		while (iter != str.end()) {
			if (*iter == '+') {
				ss << ' ';
			} else if (*iter == '%') {
				++iter;
				if (iter == str.end() || iter+1 == str.end()) { return ""; }				// 解释失败
				unsigned char high = fromHex((unsigned char)*iter++);
				unsigned char low = fromHex((unsigned char)*iter);
				ss << (high << 4) + low;
			} else {
				ss << *iter;
			}
			++iter;
		}
		return ss.str();
	}

};
NS_FS_END
