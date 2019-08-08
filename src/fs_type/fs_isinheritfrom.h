/**
 * @file: fs_isinheritfrom.h
 * @brief: 判断指定的类型是否派生于另一个类型
 * @author: hyw
 * @version: 1.0
 * @date: 2018-03-21
 */

/* --------------------------------------------------------------------------------------
 * 用法：
 * class A {};
 * class B: public A {};
 * bool b = is_inheritfrom<B, A>::value;
 * -------------------------------------------------------------------------------------*/

#ifndef __FS_DEVIDE_FROM_H__
#define __FS_DEVIDE_FROM_H__

#include "../fsmacros.h"
#include "fs_issametype.h"


NS_FS_BEGIN
namespace fs_type {

template<typename D, typename B>
struct is_inheritfrom {
private:
	struct _Small { char c; };
	struct _Big { _Small s[2]; };

	template<typename T>
	static _Small _test(...);
	template<typename T>
	static _Big _test(T*);

	template<typename T>
	static T *_buildSub();

public:
	static const bool value = sizeof(_test<B>(_buildSub<D>())) == sizeof(_Big) && !is_sametype<D, B>::value;
};

};
NS_FS_END

#endif
