/*
** implement functions for io
** author: hyw
** date: 2012.10.25
*/

#ifndef __FS_IO_H__
#define __FS_IO_H__

#include <string>
#include "fsmacros.h"
using namespace std;

NS_FS_BEGIN
namespace fs_io{
	// -------------------------------------------------------------------
	// 创建文件夹或文件
	// 如果 path 为：aa/bb/cc/ 则只创建路径，如果 path 为：aa/bb/cc 则创建文件 cc
	// 如果创建成功或路径/文件已经存在则返回 true
	// -------------------------------------------------------------------
	bool mkpath(const char* path, unsigned mode=0777);
	inline bool mkpath(const string& path, unsigned mode=0777){ return mkpath(path.c_str(), mode); }
}
NS_FS_END

#endif
