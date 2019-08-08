/**
 * @file: fs_type.h
 * @brief: 类型 traits 扩展
 * @author: hyw
 * @version: 1.0
 * @date: 2018-03-25
 * @description:
 *	C++11 must be supported
 */

#ifndef __FS_TYPE_H__
#define __FS_TYPE_H__

#include <type_traits>
#include <string>

NS_FS_BEGIN
namespace fs_type {
// ---------------------------------------------------------------------------------------
// 判断是否是字符串类型
// ---------------------------------------------------------------------------------------
template<typename T>
struct is_string {
	static const bool value =
		std::is_same<typename std::decay<T>::type, char*>::value ||
		std::is_same<typename std::decay<T>::type, const char*>::value ||
		std::is_same<typename std::decay<T>::type, std::string>::value;
};

// ---------------------------------------------------------------------------------------
// 判断是否是数值类型（包括整数和浮点数）
// ---------------------------------------------------------------------------------------
template<typename T>
struct is_numeric {
	static const bool value = std::is_integral<T>::value || std::is_floating_point<T>::value;
};

};
NS_FS_END
#endif