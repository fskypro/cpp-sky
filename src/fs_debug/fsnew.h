/*
** overload new operator to trace the new and delete operation
** author: hyw
** date: 2012.10.22
*/

#ifndef __FS_NEW_H__
#define __FS_NEW_H__

#include <new>
#include <fstream>
#include "fsmacros.h"
#include "fsdebug.h"

#if defined(_WIN32) && defined(__FS_DEBUG)
NS_FS_BEGIN
namespace debug {
// -----------------------------------------------------------------------
// inners
// -----------------------------------------------------------------------
void addNewTrack(void*, const char*, unsigned);
void removeNewTrack(void*);

// -------------------------------------------------------------
// ���û��ɾ���� new ������undbl ��ʾ������ظ���
void dumpUnfree(std::ofstream&, bool undbl=true);

// ���ַ�����ʽ����ûɾ�� new ������undbl ��ʾ��¼���ظ���
std::string getUnfreeInfo(bool undbl=true);

}

NS_FS_END

// -----------------------------------------------------------------------
// overload new/delete and new[]/delete[] operator
// -----------------------------------------------------------------------
inline void* operator new(unsigned size, const char* file, unsigned line)
{
	void* p = (void*)malloc(size);
	NS_FS::debug::addNewTrack(p, file, line);
	return p;
}

inline void operator delete(void* p, const char* file, unsigned line){}
inline void operator delete(void* p)
{
	if(p)
	{
		NS_FS::debug::removeNewTrack(p);
		free(p);
	}
}

inline void* operator new[](unsigned size, const char* file, unsigned line)
{
	void* p = (void*)malloc(size);
	NS_FS::debug::addNewTrack(p, file, line);
	return p;
}

inline void operator delete[](void* p, const char* file, unsigned line){}
inline void operator delete[](void* p)
{
	if(p)
	{
		NS_FS::debug::removeNewTrack(p);
		free(p);
	}
}


#endif // __FS_DEBUG
#endif // __FS_NEW_H__
