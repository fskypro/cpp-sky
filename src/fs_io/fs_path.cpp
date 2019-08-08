/*
** implement functions for path
** author: hyw
** date: 2012.10.15
*/

#include <cassert>
#include <string.h>
#include <vector>
#include "fsmacros.h"
#include "fs_io/fs_path.h"
#include "fs_str/fs_string.h"

NS_FS_BEGIN

namespace fs_path{
#if defined(WIN32)
const char* PATH_SPLITTER = "\\";				// 路径分隔符
const char* PATH_ANTI_SPLITTER = "/";			// 兼容路径分隔符
static const char* PATH_DSPLITTER = "\\\\";		// 双路径分隔符（等于单个）
#else
const char* PATH_SPLITTER = "/";
const char* PATH_ANTI_SPLITTER = "\\";
static const char* PATH_DSPLITTER = "//";		// 双路径分隔符（等于单个）
#endif
}		// fs_path

// -----------------------------------------------------------------------
void fs_path::split(string& path, string& name, const string& filePath)
{
	if(filePath.empty()) return;
	string::const_iterator spIter = filePath.end() -1;
	string::const_iterator iter = filePath.end() - 1;
	while(1)
	{
		if(*iter == *PATH_SPLITTER ||
			*iter == *PATH_ANTI_SPLITTER)		// 路径分隔符都是单字符
		{ ++spIter; break; }
		if(iter == filePath.begin()) { break; }
		--iter; --spIter;
	}
	path.assign(filePath.begin(), iter);
	name.assign(spIter, filePath.end());
}

void fs_path::split(string& path, string& name, string &ext, const string& filePath)
{
	string tmpName;
	split(path, tmpName, filePath);
	std::size_t pos = tmpName.rfind(".");
	if (pos == string::npos) {			// 没有扩展名
		name = tmpName;
	} else {
		name = tmpName.substr(0, pos);
		ext = tmpName.substr(pos+1, tmpName.size() - pos);
	}
}

string fs_path::getFileDir(const string &filePath) {
	char ch;
	int count = filePath.size();
	while(--count > 0) {
		ch = filePath[count];
		if (ch == *PATH_SPLITTER) { break; }
		if (ch == *PATH_ANTI_SPLITTER) { break; }
	}
	return filePath.substr(0, ++count);
}

string fs_path::getFileName(const string &filePath, bool hasExt) {
	char ch;
	int index = filePath.size();
	while(--index > 0) {
		ch = filePath[index];
		if (ch == *PATH_SPLITTER) { break; }
		if (ch == *PATH_ANTI_SPLITTER) { break; }
	}
	string fileName = filePath.substr(++index, filePath.size());
	if (hasExt) { return fileName; }
	std::size_t pos = fileName.rfind(".");
	if (pos == string::npos) { return fileName; }
	return fileName.substr(0, pos);
}

string fs_path::getFileExt(const string &filePath) {
	std::size_t pos = filePath.rfind(".");
	if (pos == string::npos) { return ""; }
	return filePath.substr(pos+1, filePath.size());
}

// -----------------------------------------------------------------------
string fs_path::normalizePath(const string& path)
{
	string s = fs_str::replace(path, PATH_ANTI_SPLITTER, PATH_SPLITTER);	// 替换兼容分隔符
	unsigned len = s.length();
	while(true)
	{
		s = fs_str::replace(s, PATH_DSPLITTER, PATH_SPLITTER);			// 双分隔符换成单分隔符
		if(len == s.length()) break;
		len = s.length();
	}

	vector<string> folders, dirs;
	fs_str::split(dirs, s, PATH_SPLITTER);
	bool isPath = dirs.size() && dirs.back().empty();						// 以“/”或“\”结尾
	vector<string>::iterator iter = dirs.begin();
	while(iter != dirs.end())
	{
		if(*iter == "..")
		{
			if(folders.size())
				folders.pop_back();
			else
				folders.push_back(*iter);
		}
		else if(!iter->empty())
		{
			folders.push_back(*iter);
		}
		++iter;
	}
	if(isPath) { folders.push_back(""); }
	return fs_str::join(PATH_SPLITTER, folders.begin(), folders.end());
}

string& fs_path::normalizePath(string& path)
{
	path = fs_path::normalizePath((const string&)path);
	return path;
}

NS_FS_END
