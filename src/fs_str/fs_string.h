/*
** implement utility functions for cpp string
** author: hyw
** date: 2012.09.18
*/


#ifndef __FS_STRING_H__
#define __FS_STRING_H__

#include <string>
#include <sstream>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN

// --------------------------------------------------------------------------------------
// fs_str
// --------------------------------------------------------------------------------------
namespace fs_str
{
	// ---------------------------------------------------------------------------------
	// 字符串判别
	// ---------------------------------------------------------------------------------
	// 判断一个字符串是否全部字符为数字
	bool isDigit(const string& str);

	// 判断一个字符串是否为空白串（即整个字符串皆为空格、\t、\n、\r）
	bool isWhiteSpace(const string& str);

	// 判断某字符串是否以指定的字符串开头
	inline bool isStartsWidth(const string &str, const string &sub) {
		return str.substr(0, sub.length()) == sub;
	}

	// 判断某字符串是否以指定的字符串结尾
	inline bool isEndsWidth(const string &str, const string &sub) {
		return str.substr(str.length() - sub.length(), sub.length()) == sub;
	}

	// ---------------------------------------------------------------------------------
	// 大小写转换
	// ---------------------------------------------------------------------------------
	string lower(const string& str);
	string& lower(string& str);

	string upper(const string& str);
	string& upper(string& str);

	// ---------------------------------------------------------------------------
	// 去掉字符串左边的空格
	// ---------------------------------------------------------------------------
	string ltrim(const string&);
	string& ltrim(string&);

	// ---------------------------------------------------------------------------
	// 去掉字符串右边的空格
	// ---------------------------------------------------------------------------
	string rtrim(const string&);
	string& rtrim(string&);

	// ---------------------------------------------------------------------------
	// 去掉字符串两边的空格
	// ---------------------------------------------------------------------------
	string trim(const string&str);
	string& trim(string&);

	// ---------------------------------------------------------------------------------
	// 将一个字符串列表中的所有字符串与指定的字符串进行分隔组合
	// 如：
	//    char cs[][3] = {"abc", "def", "ghi"};
	//    vector<string> v(cs, cs+3);
	//    string resutl = join(", ", v.begin(), v.end());
	//    result 的值将为："abc, def, ghi"
	//
	//     string resutl = join(", ", v.begin(), v.end(), [](vector<string>::iterator iter)->string{ return iter->substr(0, 2); });
	//    result 的值为："ab, de, gh"
	// ---------------------------------------------------------------------------------
	template<typename Iter>
	std::string join(const std::string& sp, Iter begin, Iter end) {
		if (begin == end) { return ""; }
		Iter iter = begin;
		std::stringstream ss;
		ss << *iter++;
		while (iter != end) {
			ss << sp << *iter++;
		}
		return ss.str();
	}

	template<typename Iter, typename FN>
	std::string join(const std::string& sp, Iter begin, Iter end, FN fn) {
		if (begin == end) { return ""; }
		Iter iter = begin;
		std::stringstream ss;
		ss << fn(iter++);
		while(iter != end) {
			ss << sp << fn(iter++);
		}
		return ss.str();
	}

	// ---------------------------------------------------------------------------------
	// 按指定分隔字符串拆分一个字符串
	// 参数：
	//    out    : 输出容器，可以是 vector<string>, list<string>, deque<string> 等
	//    str    : 要拆分的字符串
	//    sp     : 分隔符
	//    strip  : 是否削去每个子串两边的空格
	// ---------------------------------------------------------------------------------
	template<typename T>
	void split(vector<T>& out, const string& str, const string& sp, bool strip=true) {
		if(sp == "") return;
		T elem;
		size_t pos = 0, site;
		while(true) {
			stringstream ss;
			site = str.find(sp, pos);				// 查找分隔符所在位置
			if(site == str.npos) break;				// 再也找不到分隔符
			ss << str.substr(pos, site - pos);		//  取得分隔符前面的字符串
			elem = T();
			ss >> elem;
			out.push_back(elem);
			pos = site + sp.size();
		}
		stringstream ss;
		ss << str.substr(pos);
		ss >> elem;
		out.push_back(elem);
	}

	// 字符串版本
	template<>
	inline void split<string>(vector<string>& out, const string& str, const string& sp, bool strip) {
		if(sp == "") return;
		size_t pos = 0, site;
		string tmp;
		while(true) {
			site = str.find(sp, pos);									// 查找分隔符所在位置
			if(site == str.npos) break;									// 再也找不到分隔符
			tmp = str.substr(pos, site - pos);
			if (strip) {
				out.push_back(trim(str.substr(pos, site - pos)));		//  取得分隔符前面的字符串
			} else {
				out.push_back(str.substr(pos, site - pos));				//  取得分隔符前面的字符串
			}
			pos = site + sp.size();
		}
		out.push_back(strip ? trim(str.substr(pos)) : str.substr(pos));
	}

	// 字符分隔版本
	template<typename T>
	void split(vector<T>& out, const string& str, char sp, bool strip=true) {
		split(out, str, string(1, sp), strip);
	}

	// 返回去临时值版本
	template<typename T>
	vector<T> split(const string& str, const string& sp, bool strip=true) {
		vector<T> vs;
		split(vs, str, sp, strip);
		return vs;
	}

	// ---------------------------------------------------------------------------
	// 用新的字符串替换掉指定字符串中的所有子串
	// ---------------------------------------------------------------------------
	string& replace(string &str, const string& strOld, const string& strNew);
	string replace(const string &str, const string& strOld, const string& strNew);
};
NS_FS_END
#endif
