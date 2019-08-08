/*
** implement base64 encode
** author: hyw
** date: 2018.03.15
*/

#ifndef __FS_BASE_64_H__
#define __FS_BASE_64_H__

#include "fsmacros.h"

NS_FS_BEGIN
namespace fs_encode {
class Base64 {
private:
	Base64();

public:
	// 编码
	static std::string encode(const unsigned char *str, size_t size);

	// 解码，如果传入的不是空串，但返回的是空串，则表示解码失败
	static std::string decode(const std::string &str);
}; };

NS_FS_END
#endif
