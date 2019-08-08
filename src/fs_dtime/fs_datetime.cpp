/**
 * @file: fsdatetime.cpp
 * @brief: 解释字符串格式的日期时间
 * @author: hyw
 * @version: 1.0
 * @date: 2018-01-12
 */

#include <algorithm>
#include "fs_str/fs_string.h"
#include "fs_str/fs_convert.h"
#include "fs_datetime.h"

using namespace std;


NS_FS_BEGIN


namespace fs_dtime
{
	// ----------------------------------------------------------------------------------
	// implement format
	// ----------------------------------------------------------------------------------
	static const char *STR_WDAYS[] = {"Monday", "Tuesday", "Wednesday", "Thurday", "Friday", "Saturday", "Sunday"};
	static const char *STR_SMP_WDAYS[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	static const char *STR_MONS[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	static const char *STR_SMP_MONS[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	void DateTime::format(const string &fmt, string &str) const {
		string strYear = fs_str::nu2str(year());
		string strMon, strDay, strHour, strMin, strSec;

		string::const_iterator iterFmt = fmt.begin();
		while(iterFmt != fmt.end()) {
			if (*iterFmt != '%') {
				str.append(1, *iterFmt);
				++iterFmt;
				continue;
			}
			++iterFmt;
			switch (*iterFmt) {
				case '%':
					str.append(1, '%');
					break;
				case 'a':
					str.append(STR_SMP_WDAYS[wday()-1]);
					break;
				case 'A':
					str.append(STR_WDAYS[wday()-1]);
					break;
				case 'b':
					str.append(STR_SMP_MONS[mon()-1]);
					break;
				case 'B':
					str.append(STR_MONS[mon()-1]);
					break;
				case 'd':
					str.append(mday() < 10 ? "0" + fs_str::nu2str(mday()) : fs_str::nu2str(mday()));
					break;
				case 'H':
					str.append(hour() < 10 ? "0" + fs_str::nu2str(hour()) : fs_str::nu2str(hour()));
					break;
				case 'I':
					str.append(apmHour() < 10 ? "0" + fs_str::nu2str(apmHour()) : fs_str::nu2str(apmHour()));
					break;
				case 'm':
					str.append(mon() < 10 ? "0" + fs_str::nu2str(mon()) : fs_str::nu2str(mon()));
					break;
				case 'M':
					str.append(min() < 10 ? "0" + fs_str::nu2str(min()) : fs_str::nu2str(min()));
					break;
				case 'P':
					str.append(hour() <= 12 ? "AM" : "PM");
					break;
				case 'S':
					str.append(sec() < 10 ? "0" + fs_str::nu2str(sec()) : fs_str::nu2str(sec()));
					break;
				case 'w':
					str.append(fs_str::nu2str(wday()));
					break;
				case 'W':
					str.append(fs_str::nu2str(yweekth()));
					break;
				case 'x':
					strMon = mon() < 10 ? "0" + fs_str::nu2str(mon()) : fs_str::nu2str(mon());
					strDay = mday() < 10 ? "0" + fs_str::nu2str(mday()) : fs_str::nu2str(mday());
					str.append(strYear + "-" + strMon + "-" + strDay);
					break;
				case 'X':
					strHour = hour() < 10 ? "0" + fs_str::nu2str(hour()) : fs_str::nu2str(hour());
					strMin = min() < 10 ? "0" + fs_str::nu2str(min()) : fs_str::nu2str(min());
					strSec = sec() < 10 ? "0" + fs_str::nu2str(sec()) : fs_str::nu2str(sec());
					str.append(strHour + ":" + strMin + ":" + strSec);
					break;
				case 'y':
					str.append(strYear.substr(strYear.length() - 2, 2));
					break;
				case 'Y':
					str.append(strYear);
					break;
				default:
					str.append("%");
					continue;
			}
			++iterFmt;
		}
	}

	// ----------------------------------------------------------------------------------
	// implement fmtParse
	// ----------------------------------------------------------------------------------
	// 读取年份，成功则返回 true，否则返回 false
	bool _read_Y(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		string syear = strDTime.substr(iter - strDTime.begin(), 4);
		if (syear.length() != 4) { return false; }
		if (!fs_str::isDigit(syear)) { return false; }
		st.tm_year = fs_str::str2int32(syear) - 1900;
		iter += 4;
		return true;
	}

	// 读取月份，如果连续两个是数字的话，则取两个，否则只取一个
	bool _read_m(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		if (iter == strDTime.end()) { return false; }
		if (*iter < '0' || *iter > '9') { return false; }
		string smonth(1, *iter++);
		if (*iter >= '0' && *iter <= '9') {
			smonth.append(1, *iter++);
		}
		st.tm_mon = fs_str::str2int32(smonth) - 1;
		return true;
	}

	// 读取日，如果连续两个是数字的话，则取两个，否则只取一个
	bool _read_d(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		if (iter == strDTime.end()) { return false; }
		if (*iter < '0' || *iter > '9') { return false; }
		string sday(1, *iter++);
		if (*iter >= '0' && *iter <= '9') {
			sday.append(1, *iter++);
		}
		st.tm_mday = fs_str::str2int32(sday);
		return true;
	}

	// 读取时，如果连续两个是数字的话，则取两个，否则只取一个
	bool _read_H(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		if (iter == strDTime.end()) { return false; }
		if (*iter < '0' || *iter > '9') { return false; }
		string shour(1, *iter++);
		if (*iter >= '0' && *iter <= '9') {
			shour.append(1, *iter++);
		}
		st.tm_hour = fs_str::str2int32(shour);
		return true;
	}

	// 读取分，如果连续两个是数字的话，则取两个，否则只取一个
	bool _read_M(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		if (iter == strDTime.end()) { return false; }
		if (*iter < '0' || *iter > '9') { return false; }
		string sminute(1, *iter++);
		if (*iter >= '0' && *iter <= '9') {
			sminute.append(1, *iter++);
		}
		st.tm_min= fs_str::str2int32(sminute);
		return true;
	}

	// 读取秒，如果连续两个是数字的话，则取两个，否则只取一个
	bool _read_S(struct tm &st, const string &strDTime, string::const_iterator &iter) {
		if (iter == strDTime.end()) { return false; }
		if (*iter < '0' || *iter > '9') { return false; }
		string ssec(1, *iter++);
		if (*iter >= '0' && *iter <= '9') {
			ssec.append(1, *iter++);
		}
		st.tm_sec = fs_str::str2int32(ssec);
		return true;
	}


	bool DateTime::fmtParse(DateTime &dt, const string &fmt, const string &strDTime) {
		struct tm tmptm;
		string::const_iterator iterFmt = fmt.begin();
		string::const_iterator iterDT = strDTime.begin();
		while(iterFmt != fmt.end()) {
			if (iterDT == strDTime.end()) {
				return false;
			}

			if (*iterFmt == '%') {
				++iterFmt;
				switch (*iterFmt) {
				case '%':
					if(*iterDT == '%') {
						++iterDT;
						++iterFmt;
						continue;
					}
					return false;
				case 'Y':
					if (_read_Y(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				case 'm':
					if (_read_m(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				case 'd':
					if (_read_d(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				case 'H':
					if (_read_H(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				case 'M':
					if (_read_M(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				case 'S':
					if (_read_S(tmptm, strDTime, iterDT)) {
						++iterFmt;
						continue;
					}
					return false;
				default:
					return false;
				}
			} else if (*iterFmt == *iterDT) {
				++iterFmt;
				++iterDT;
				continue;
			}
			return false;
		}
		if (iterFmt == fmt.end() && iterDT == strDTime.end()) {
			dt.clean();
			dt = tmptm;
			return true;
		}
		return false;
	}

};

NS_FS_END

