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
	// �����ļ��л��ļ�
	// ��� path Ϊ��aa/bb/cc/ ��ֻ����·������� path Ϊ��aa/bb/cc �򴴽��ļ� cc
	// ��������ɹ���·��/�ļ��Ѿ������򷵻� true
	// -------------------------------------------------------------------
	bool mkpath(const char* path, unsigned mode=0777);
	inline bool mkpath(const string& path, unsigned mode=0777){ return mkpath(path.c_str(), mode); }
}
NS_FS_END

#endif
