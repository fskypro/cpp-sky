/*
** implement base64 encode
** author: hyw
** date: 2018.03.15
*/

#include <sstream>
#include <string>
#include "fs_base64.h"

NS_FS_BEGIN
namespace fs_encode {
	// -------------------------------------------------------------------------------------
	// inner
	// -------------------------------------------------------------------------------------
	// Base64 编码使用的标准字典
	static const std::string encodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// 解码表
	const char decodeTable[] = {
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
		-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
		-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
	};

	// -------------------------------------------------------------------------------------
	// public
	// -------------------------------------------------------------------------------------
	std::string Base64::encode(const unsigned char * str, size_t size) {
		unsigned char byte0;
		unsigned char byte1;
		unsigned char byte2;
		std::stringstream ss;
		size_t index = 0;
		while (index < size - 2) {
			byte0 = str[index++];
			byte1 = str[index++];
			byte2 = str[index++];
			ss << encodeTable[byte0 >> 2];
			ss << encodeTable[((byte0 & 0x03) << 4) + (byte1 >> 4)];
			ss << encodeTable[((byte1 & 0x0f) << 2) + (byte2 >> 6)];
			ss << encodeTable[byte2 & 0x3f];
		}
		if (index < size) {
			byte0 = str[index++];
			ss << encodeTable[byte0 >> 2];

			if (index >= size) {
				ss << encodeTable[(byte0 & 0x03) << 4] << "==";
			} else {
				unsigned byte1 =str[index];
				ss << encodeTable[((byte0 & 0x03) << 4) + (byte1 >> 4)];
				ss << encodeTable[(byte1 & 0x0f) << 2] << "=";
			}
		}
		return ss.str();
	}

	std::string Base64::decode(const std::string &str) {
		std::stringstream ss;
		std::string::const_iterator iter = str.begin();
		char chr;
		int index = 0;
		int bin = 0;
		while(iter != str.end()) {
			chr = *iter++;
			// 在解码时，4 个字符为一组进行一轮字符匹配。两个条件：
			// 1、如果某一轮匹配的第二个是“=”且第三个字符不是“=”，说明这个带解析字符串不合法，直接返回空
			// 2、如果当前 “=” 不是第二个字符，且后面的字符只包含空白符，则说明这个这个条件合法，可以继续。
			if (chr == '=') {
				if (*iter != '=' && (index % 4) == 1) {
					return "";
				}
				continue;
			}
			chr = decodeTable[(int)chr];
			if (chr < 0) { continue; }			// 过滤所有不合法的字符

			switch(index % 4) {
			case 0:
				bin = chr << 2;
				break;
			case 1:
				bin |= chr >> 4;
				ss << bin;
				bin = (chr & 0x0f) << 4;
				break;
			case 2:
				bin |= chr >> 2;
				ss << bin;
				bin = ( chr & 0x03 ) << 6;
				break;
			case 3:
				bin |= chr;
				ss << bin;
				break;
			}
			index++;
		}
		return ss.str();
	}
};

NS_FS_END
