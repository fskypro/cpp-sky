/*
** implement functions for io
** author: hyw
** date: 2012.10.25
*/

#include <stdio.h>
#include <vector>
#include "fsmacros.h"
#include "fs_path.h"
#include "fs_str/fs_string.h"
#include "fs_io.h"

#if defined(WIN32)									// windows 平台
#include <io.h>
#include <direct.h>
#define MKDIR(path, mode) mkdir(path)
#elif defined(__linux__)							// linux 平台
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#define MKDIR(path, mode) mkdir(path, mode)
#endif												// 其他平台暂时让它出错

#if defined(_MSC_VER)
#pragma warning(disable: 4996)						// access
#endif

NS_FS_BEGIN

bool fs_io::mkpath(const char* path, unsigned mode)
{
	string filePath = fs_path::normalizePath(path);
	vector<string> folders;
	fs_str::split(folders, filePath, fs_path::PATH_SPLITTER);
	if(folders.size() == 0) { return false; }
	string fileName = folders.back();
	folders.pop_back();										// 先忽略文件名
	filePath.clear();
	vector<string>::iterator iter = folders.begin();
	while(iter != folders.end())
	{
		filePath.append(*iter);
		if(access(filePath.c_str(), 0) == -1)				// 文件夹不存在
		{
			if(MKDIR(filePath.c_str(), mode) == -1)			// 创建文件夹失败
				return false;
		}
		filePath.append(fs_path::PATH_SPLITTER);
		++iter;
	}
	if(!fileName.empty())									// 有文件名
	{
		filePath.append(fileName);
		if(access(filePath.c_str(), 0) != -1)				// 文件已存在
			return true;

		FILE *pf = fopen(filePath.c_str(), "w+");
		if(!pf){ return false; }
		fclose(pf);
	}
	return true;
}

NS_FS_END
