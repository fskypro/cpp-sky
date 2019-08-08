/*
** project defination
** author: hyw
** date: 2012.04.25
*/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cassert>
#include "fsmacros.h"

NS_FS_BEGIN

namespace fs_tpl {

class SingletonBase { };

template <typename T, typename Base = SingletonBase>
class Singleton : public Base
{
public:
	static inline T* pInst()
	{
		if (!sm_pInst)
		{
			sm_pInst = new T;
		}
		return sm_pInst;
	}

	static inline T &inst()
	{
		return *(pInst());
	}

	static void release()
	{
		if (sm_pInst != 0)
		{
			delete sm_pInst;
			sm_pInst = 0;
		}
	}

protected:
	Singleton(): Base() { assert(sm_pInst == 0); }

	template<typename A1>
	Singleton(A1 a1): Base(a1) { assert(sm_pInst == 0); }

	template<typename A1, typename A2>
	Singleton(A1 a1, A2 a2): Base(a1, a2) { assert(sm_pInst == 0); }

	template<typename A1, typename A2, typename A3>
	Singleton(A1 a1, A2 a2, A3 a3): Base(a1, a2, a3) { assert(sm_pInst == 0); }

	template<typename A1, typename A2, typename A3, typename A4>
	Singleton(A1 a1, A2 a2, A3 a3, A4 a4): Base(a1, a2, a3, a4) { assert(sm_pInst == 0); }

	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	Singleton(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5): Base(a1, a2, a3, a4, a5) { assert(sm_pInst == 0); }

	virtual ~Singleton() { }

private:
    static T* sm_pInst;
};

};
NS_FS_END

// -----------------------------------------------------------------------
// declarations
// -----------------------------------------------------------------------
#define FS_SINGLETON_DEFINE_INST(T, ...)\
	template<>\
	T* NS_FS::fs_tpl::Singleton<T, ##__VA_ARGS__>::sm_pInst = NULL

#define FS_SINGLETON_FRIEND_SUB(T, ...)\
	friend T* NS_FS::fs_tpl::Singleton<T, ##__VA_ARGS__>::pInst();\
	friend void NS_FS::fs_tpl::Singleton<T, ##__VA_ARGS__>::release();

#endif
