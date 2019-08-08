/*
** implement url encode
** author: hyw
** date: 2018.03.15
*/

#ifndef __FS_URLENCODER_H__
#define __FS_URLENCODER_H__

#include "fsmacros.h"

NS_FS_BEGIN

namespace fs_encode {

class UrlEncoder {
private:
	UrlEncoder();

public:
	// url 编码
	static std::string encode(const std::string &str);

	// url 解码，注意，如果解码失败，则返回空串
	static std::string decode(const std::string &str);
};
};
NS_FS_END

#endif

