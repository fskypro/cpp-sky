/*
** implement functions for path
** author: hyw
** date: 2012.10.15
*/

#ifndef __FS_PATH_H__
#define __FS_PATH_H__

#include <string>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN
namespace fs_path{

	extern const char* PATH_SPLITTER;		// 路径分隔符
	extern const char* PATH_ANTI_SPLITTER;	// 兼容路径分隔符

	// -------------------------------------------------------------------
	// 拆分文件路径和文件名
	// 参数：
	//     path    : 输出路径
	//     name    : 输出文件名
	//     filePath: 输入文件名及路径
	// 如：aa\bb\cc\dd\ee.txt 调用 split 后
	//     path == aa\bb\cc\dd
	//     name == ee.txt
	// -------------------------------------------------------------------
	void split(string& path, string& name, const string& filePath);

	// -------------------------------------------------------------------
	// 拆分文件路径和文件名
	// 参数：
	//     path    : 输出路径
	//     name    : 输出文件名
	//     ext  : 扩展名
	//     filePath: 输入文件名及路径
	// 如：aa\bb\cc\dd\ee.txt 调用 split 后
	//     path == aa\bb\cc\dd
	//     name == ee
	//     ext == txt
	// -------------------------------------------------------------------
	void split(string& path, string& name, string &ext, const string &filePath);

	// -------------------------------------------------------------------
	// 获取路径的目录部分
	// 如：aa/bb/cc/dd.txt
	//      则返回 aa/bb/cc/
	// -------------------------------------------------------------------
	string getFileDir(const string &filePath);

	// -------------------------------------------------------------------
	// 获取路径的文件名
	// 如：aa/bb/cc/dd.txt
	//      如果 hasExt == true，则返回 dd.txt
	//      如果 hasExt == false，则返回 dd
	// -------------------------------------------------------------------
	string getFileName(const string &filePath, bool hasExt=true);

	// -------------------------------------------------------------------
	// 获取文件的扩展名
	// 如：aa/bb/cc/dd.txt
	//      则返回 txt
	// -------------------------------------------------------------------
	string getFileExt(const string &filePath);

	// -------------------------------------------------------------------
	// 标准化一个路径
	// 如在 Windows 平台有路径：
	/*     c:\abc\def\ghi/jkl\mno\..\\pqr///stu\vwx\ 标准化后将变为：c:\abc\def\ghi\jkl\pqr\stu\vwx\ */
	//     ../aa/bb/cc//dd.txt 标准化后为：../aa/bb/cc/dd.txt
	// -------------------------------------------------------------------
	string normalizePath(const string& path);
	string& normalizePath(string& path);

}
NS_FS_END

#endif
