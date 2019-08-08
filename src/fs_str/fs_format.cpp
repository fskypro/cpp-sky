/*
** implement string format
** author: hyw
** date: 2012.09.18
*/

#include <sstream>
#include "fs_format.h"

NS_FS_BEGIN

void fs_str::format(string &strOut, const string &strFmg, const map<string, FSFormatArg> &args) {
	  stringstream ss;
	  string::const_iterator iter = strFmg.begin();
	  string key, trimKey;
	  bool scoped = false;
	  char ch;
	  while(iter != strFmg.end()) {
		  ch = *iter;
		  if (scoped) {
			  if (ch == '}') {
				trimKey = key;
				trimKey = trim(trimKey);
				if (args.find(trimKey) != args.end()) {
					ss << args.at(trimKey);
				} else {
					ss << '{' << key << '}';
				}
				key.clear();
				scoped = false;
			} else if (ch == '{') {
				ss << '{' << key << ch;
				scoped = false;
			}
			else {
				key.append(1, ch);
			}
		} else {
			if (ch == '{') {
				scoped = true;
			} else {
				ss << ch;
			}
		}
		++iter;
	}
	strOut = ss.str();
}

NS_FS_END