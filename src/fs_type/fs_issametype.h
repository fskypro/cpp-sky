/**
 * @file: fs_issametype.h
 * @brief: 判断两个类型是否相同
 * @author: hyw
 * @version: 1.0
 * @date: 2018-03-21
 */

/* --------------------------------------------------------------------------------------
 * 用法：
 * class A {};
 * class B: public A {};
 * bool b = is_issametype<B, A>::value;
 * -------------------------------------------------------------------------------------*/

#ifndef __FS_ISSAMETYPE_H__
#define __FS_ISSAMETYPE_H__

#include "../fsmacros.h"

NS_FS_BEGIN
namespace fs_type {

template<typename T1, typename T2>
struct is_sametype {
private:
	struct _Small { char c; };
	struct _Big { _Small s[2]; };

	template<typename T>
	static _Small _test(...);
	template<typename T>
	static _Big _test(T);

	template<typename T>
	static T _buildInst();

public:
	static const bool value = sizeof(_test<T1>(_buildInst<T2>())) == sizeof(_Big);
};

};
NS_FS_END;
#endif
