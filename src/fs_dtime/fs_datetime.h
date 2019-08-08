/**
 * @file: fsdatetime.h
 * @brief: 自定义时间结构
 * @author: hyw
 * @version: 1.0
 * @date: 2018-01-13
 */

#ifndef __FS_DATETIME_H__
#define __FS_DATETIME_H__

#include <algorithm>
#include <string>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include "fsmacros.h"
#include "fs_time.h"

using namespace std;

NS_FS_BEGIN

namespace fs_dtime {

// ---------------------------------------------------------------------------------------
// DateTime class
// 规定：
//		1、每月 1 号的值为 1
//		2、周一至周日的值为：1、2、3、4、5、6、7；以周一作为每个星期的起始日
//		3、获取 yday(年的第几天) 时，如果是第 1 天，返回的是 1，即每年 1 月 1 号为第一天，值为 1
// ---------------------------------------------------------------------------------------
class DateTime {
private:
	inline void _init(int year, int mon=0, int mday=1, int hour=0, int min=0, int sec=0) {
		m_st.tm_year = year;
		m_st.tm_mon = mon;
		m_st.tm_mday = mday;
		m_st.tm_hour = hour;
		m_st.tm_min = min;
		m_st.tm_sec = sec;
		// 调用 mktime 会调整为正确的日期参数，譬如如果填写了月份为 13 月，则调用 mktime 后，会进一年
		mktime(&m_st);
	}

public:
	explicit DateTime(bool isGMTime = false) {
		time_t now = time(NULL);
		struct tm *st = isGMTime ? gmtime(&now) : localtime(&now);
		_init(st->tm_year, st->tm_mon, st->tm_mday, st->tm_hour, st->tm_min, st->tm_sec);
	}

	DateTime(const struct tm &st) {
		_init(st.tm_year, st.tm_mon, st.tm_mday, st.tm_hour, st.tm_min, st.tm_sec);
	}

	DateTime(time_t tt, bool isGMTime = false) {
		struct tm *st = isGMTime ? gmtime(&tt) : localtime(&tt);
		_init(st->tm_year, st->tm_mon, st->tm_mday, st->tm_hour, st->tm_min, st->tm_sec);
	}

	DateTime(int year, int mon, int mday=1, int hour=0, int min=0, int sec=0) {
		_init(year-1900, mon-1, mday, hour, min, sec);
	}

	DateTime(const DateTime &dt) {
		if (this != &dt) {
		_init(dt.m_st.tm_year, dt.m_st.tm_mon, dt.m_st.tm_mday,
			dt.m_st.tm_hour, dt.m_st.tm_min, dt.m_st.tm_sec);
		}
	}

	DateTime(int year, int mon, int mday, const Time &dayTime) {
		_init(year-1900, mon-1, mday, dayTime.hour(), dayTime.min(), dayTime.sec());
	}

	// ----------------------------------------------------
	DateTime &operator=(const DateTime &dt) {
		if (this != &dt) {
			_init(dt.m_st.tm_year, dt.m_st.tm_mon, dt.m_st.tm_mday,
				dt.m_st.tm_hour, dt.m_st.tm_min, dt.m_st.tm_sec);
		}
		return *this;
	}

	DateTime &operator=(const struct tm &st) {
		_init(st.tm_year, st.tm_mon, st.tm_mday, st.tm_hour, st.tm_min, st.tm_sec);
		return *this;
	}

	// ----------------------------------------------------
	DateTime &update(int year, int mon=1, int mday=1, int hour=0, int min=0, int sec=0) {
		_init(year-1900, mon-1, mday, hour, min, sec);
		return *this;
	}

	DateTime &clean() {
		_init(1900, 1, 1, 0, 0, 0);
		return *this;
	}

public:
	const struct tm &cdatetime() const { return m_st; }
	time_t sectime() const { return mktime(const_cast<struct tm*>(&m_st)); }
	operator struct tm() { return m_st; }
	string toString() {
		stringstream ss;
		ss << "DateTime["
			<< "year=" << year()
			<< ", mon=" << mon()
			<< ", mday=" << mday()
			<< ", hour=" << hour()
			<< ", min=" << min()
			<< ", sec=" << sec() << "]";
		return ss.str();
	}

	inline int year() const { return m_st.tm_year + 1900; }						// 年份
	inline int mon() const { return m_st.tm_mon + 1; }							// 1~12
	inline int mday() const { return m_st.tm_mday; }							// 1~31
	inline bool isAm() const { return m_st.tm_hour <= 12; }						// 是否是上午
	inline int hour() const { return m_st.tm_hour; }							// 0~23
	inline int apmHour() const { return isAm() ? hour() : hour() - 12; }		// 12 进制时间
	inline int min() const { return m_st.tm_min; }								// 0~59
	inline int sec() const { return m_st.tm_sec; }								// 0~59
	inline Time dayTime() const {
		return Time(m_st.tm_hour, m_st.tm_min, m_st.tm_sec);
	}

	// ----------------------------------------------------
	inline DateTime &year(int iyear) {
		m_st.tm_yday = iyear - 1900;
		mktime(&m_st);
		return *this;
	}
	inline DateTime &mon(int imon) {
		m_st.tm_mon = imon - 1;
		mktime(&m_st);
		return *this;
	}
	inline DateTime &mday(int imday) {
		m_st.tm_mday = imday;
		mktime(&m_st);
		return *this;
	}
	inline DateTime &hour(int ihour) {
		m_st.tm_hour = ihour;
		mktime(&m_st);
		return *this;
	}
	inline DateTime &min(int imin) {
		m_st.tm_min = imin;
		mktime(&m_st);
		return *this;
	}
	inline DateTime &sec(int isec) {
		m_st.tm_sec = isec;
		mktime(&m_st);
		return *this;
	}

	// ----------------------------------------------------
	inline DateTime &addyear(int value) { return year(year() + value); }
	inline DateTime &addmon(int value) { return mon(mon() + value); }
	inline DateTime &addmday(int value) { return mday(mday() + value); }
	inline DateTime &addhour(int value) { return hour(hour() + value); }
	inline DateTime &addmin(int value) { return hour(hour() + value); }
	inline DateTime &addsec(int value) { return sec(sec() + value); }

	// ------------------------------------------
	bool operator==(const DateTime& dt) { return sectime() == dt.sectime(); }
	bool operator<(const DateTime& dt) { return sectime() < dt.sectime(); }
	bool operator<=(const DateTime& dt) { return sectime() <= dt.sectime(); }
	bool operator>(const DateTime& dt) { return sectime() > dt.sectime(); }
	bool operator>=(const DateTime& dt) { return sectime() >= dt.sectime(); }

public:
	// readonly
	// 获取该日期为一年中的第几天，1 月 1 号为第一天，返回 1
	inline int yday() const {
		return m_st.tm_yday + 1;
	}

	// 获取星期值，周一为 1，周日为 7
	inline int wday() const {
		return m_st.tm_wday == 0 ? 7 : m_st.tm_wday;
	}

	// --------------------------------------------------------------
	// 获当前日期是当月下的第几个星期（顺序为：一、二、三、四、五、六、日。即星期的起始日为周一）
	inline int mweekth() const {
		int weekDay_1 = DateTime(year(), mon(), 1).wday();				// 当月一号为星期几
		return ((mday() - 1) + (weekDay_1 - 1)) / 7 + 1;
	}

	// 获取指定时间是当年的第几个星期
	inline int yweekth() const {
		int weekDay_11 = DateTime(year(), 1, 1).wday();					// 当年一月一号为星期几
		return ((yday() - 1) + ((weekDay_11 - 1) % 6)) / 7 + 1;
	}

	// --------------------------------------------------------------
	// 获取当天的凌晨秒时间（1970 年元月以来的的秒数）
	inline time_t sectimeOfDayStart() const {
		return DateTime(year(), mon(), mday()).sectime();
	}

	// 获取当天的结束秒时间（1970 年元月以来的秒数）
	inline time_t sectimeOfDayEnd() const {
		return sectimeOfDayStart() + 86399;								// 86399 = 24 * 3600 - 1
	}

	// 获取所在星期的周一凌晨时间值（周起始时间）
	inline time_t sectimeOfWeekStart() const {
		time_t mor = DateTime(year(), mon(), mday()).sectime();			// 当日凌晨时间值
		return mor - (wday() - 1) * 86400;								// 86400 = 3600 * 24
	}

	// 获取所在星期的周日结束时间值（23:59:59）
	inline time_t sectimeOfWeekEnd() const {
		return sectimeOfWeekStart() + 604799;							// 604799 = 3600 * 24 * 7 - 1
	}

	// 获取该日期所在当月内，当个星期的起始时间值（以周一为每个星期的第一天）
	// 如果该日期在当月的第一周，则实际返回的是当月 1 号的凌晨时间值
	// 如果该日期在当月的第二周以上，则实际返回的是该日期的周起始时间
	inline time_t sectimeOfMWeekthStart() const {
		int weekth = mweekth();
		if (weekth > 1) { return sectimeOfWeekStart(); }
		return DateTime(year(), mon(), 1).sectime();
	}

	// 获取该日期所在当月内，当个星期的结束时间（以周日为每个星期的最后一天）
	// 如果该日期在当月的最后一个星期，则实际上返回的是当月最后一天的结束时间值
	// 如果该日期不在当月最后一个星期，则实际返回的是当星期的结束时间值
	inline time_t sectimeOfMWeekthEnd() const {
		int weekth = mweekth();
		if (weekth >= weeksOfMon()) {											// 该时间在当月最后一个星期
			return DateTime(year(), mon(), daysOfMon(), 23, 59, 59).sectime();	// 则返回当月最后一天的结束时间值
		} else if (weekth == 1) {												// 第一个星期
			DateTime dt(year(), mon(), 1);										// 当月一号
			int sunday = 7 - dt.wday() + 1;
			return dt.mday(sunday).sectime() + 86399;							// 86399 = 24 * 3600 - 1
		}
		return sectimeOfWeekEnd();
	}

	// 获取该日期所在月份的一号凌晨时间值（月起始时间）
	inline time_t sectimeOfMonthStart() const {
		return DateTime(year(), mon(), 1).sectime();
	}

	// 获取该日期所在月份的最后一天的结束时间（23:59:59）
	inline time_t sectimeOfMonthEnd() const {
		return DateTime(year(), mon(), daysOfMon(), 23, 59, 59).sectime();
	}

	// 获取该日期所在年份的一月一号的时间值
	inline time_t sectimeOfYearStart() const {
		return DateTime(year(), 1, 1).sectime();
	}

	// 获取该日期所在年份的最后一天的结束时间（23:59:59）
	inline time_t sectimeOfYearEnd() const {
		return DateTime(year(), 12, 31, 23, 59, 59).sectime();
	}

	// --------------------------------------------------------------
	// 获取该日期所在月总共有多少天
	inline int daysOfMon() const {
		return daysOfMon(year(), mon());
	}

	// 获取当前日期所在年份的天数
	inline int daysOfYear() const {
		return daysOfYear(year());
	}

	// 获取当前日期所在月份有几个星期
	inline int weeksOfMon() const {
		return weeksOfMon(year(), mon());
	}

	// 获取当前日期所在年总共有多少个星期
	inline int weeksOfYear() const {
		return weeksOfYear(year());
	}

	// --------------------------------------------------------------
	// 按需求格式化
	// 格式化符号：
	//     %%	%字符
	//     %a	简写的星期名称(Mon, Tue, Wed, Thu, Fri, Sat, Sun)
	//     %A	完整的星期名称()
	//     %b	简写的月份名称(Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec)
	//     %B	完整的月份名称
	//     %d	用两位表示日期 01~31
	//     %H	用两位表示小时 00~23
	//     %I	用两位表示小时 00~12
	//     %m	01~12月
	//     %M	用两位表示分钟 00~59
	//     %P	字符AM或PM
	//     %S	用两位表示秒钟 00~59
	//     %w	每周的日期(1=星期一,7=星期天)
	//     %W	用两位表示年的第几个星期 0~53,以星期一为每周的开始
	//     %x	日期，格式为：年-月-日
	//     %X	时间，格式为：时:分:秒
	//     %y	用两位表示年份00~99
	//     %Y	用四位表示年份
	// --------------------------------------------------------------
	void format(const string &fmt, string &str) const;
	// C++11版本（使用右值引用，会优化返回的临时对象）
	string format(const string &fmt) const {
		string str;
		format(fmt, str);
		return str;
	}

public:
	// ------------------------------------------------------------------------
	// static get now time
	// ------------------------------------------------------------------------
	// 获取 1970 年以来到当前的秒数
	static time_t secNow() {
		return time(NULL);
	}

	// 获取 1970 年元月到现在的毫秒数
	static unsigned long msecNow() {
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	// 获取 1970 年以来到当前的浮点形式秒数
	static double dsecNow() {
		return msecNow() * 0.001;
	}

	// ------------------------------------------------------------------------
	// static functions
	// ------------------------------------------------------------------------
	// 判断指定年是否是闰年（闰年 2 月份有 29 天）：
	//	可被 400 整除，或可被 4 整除同时不被 100 整除
	static bool isLeapYear(int year) {
		if (year % 400 == 0) { return true; }
		if (year % 4 != 0) { return false; }
		if (year % 100 == 0) { return false; }
		return true;
	}

	// 获取指定月份的天数，如果输入错误则返回 0
	static int daysOfMon(int year, int mon) {
		if (mon < 1 || mon > 12) { return 0; }
		if (mon == 2) { return isLeapYear(year) ? 29 : 28; }
		if (mon < 8) { return 30 + mon % 2; }
		return 31 - (mon % 2);
	}

	// 获取指定年份总共有多少天
	static int daysOfYear(int year) {
		return isLeapYear(year) ? 366 : 365;
	}

	// 获取指定月份有几个星期
	static int weeksOfMon(int year, int mon) {
		int days = daysOfMon(year, mon);						// 当月天数
		int startWDay = DateTime(year, mon, 1).wday();			// 当月一号是星期几
		int firstWeekDays = 8 - startWDay;						// 第一个星期占用的天数
		days -= firstWeekDays;
		return int((double)days /  7.0 + 0.5) + 1;
	}

	// 获取指定年份一共有多少个星期
	static int weeksOfYear(int year) {
		int days = daysOfYear(year);							// 指定年份的总天数
		int startWDay = DateTime(year, 1, 1).wday();			// 一月一号为星期几
		int firstWeekDays = 8 - startWDay;						// 第一个星期占用的天数
		days -= firstWeekDays;									// 除去第一个星期占用天数后剩余天数
		return int((double)days / 7.0 + 0.5) + 1;
	}

	// ------------------------------------------------------------------------
	// 通过星期获取时间值
	// ------------------------------------------------------------------------
	// 获取指定年月的第 weekth 个星期的起始时间（星期的首日为周一）
	static time_t sectimeOfMWeekthStart(int year, int mon, int weekth) {
		DateTime dt(year, mon, 1);
		if (weekth <= 1) { return dt.sectime(); }						// 首个星期返回一号的时间
		int weekFirstDay = (weekth - 1) * 7 + 2 - dt.wday();			// 指定第 weekth 个星期起始日的号数
		weekFirstDay = std::min(weekFirstDay, daysOfMon(year, mon));	// 当月最多几天
		return dt.mday(weekFirstDay).sectime();
	}

	// 获取指定年份第 weekth 个星期的结束时间值（23:59:59）（星期的最后一天为周日）
	static time_t sectimeOfMWeekthEnd(int year, int mon, int weekth) {
		int weeks = weeksOfMon(year, mon);								// 指定年月有（跨）几个星期
		if (weekth >= weeks) {											//  如果是最后一个星期，则返回指定月份最后一天的结束时间
			return DateTime(year, mon, daysOfMon(year, mon), 23, 59, 59).sectime();
		}
		return sectimeOfMWeekthStart(year, mon, weekth+1) - 1;			// 否则返回下一个星期的起始时间减一
	}

	// 获取指定年份的第 weekth 个星期的起始时间
	static time_t sectimeOfYWeekthStart(int year, int weekth) {
		DateTime dt(year, 1, 1);
		if (weekth <= 1) { return dt.sectime(); }						// 第一个星期则返回 1 月 1 号起始时间
		int weekFirstDay = (weekth - 1) * 7 + 2 - dt.wday();			// 指定第 weekth 个星期为当年的第几天
		int mon = 1;
		while (mon <= 12) {
			int mdays = daysOfMon(year, mon);
			if (weekFirstDay <= mdays) { break; }
			weekFirstDay -= mdays;
			++mon;
		}
		return dt.mday(weekFirstDay).sectime();
	}

	// 获取指定年份的第 weekth 个星期的结束时间（23:59:59）
	static time_t sectimeOfYWeekthEnd(int year, int weekth) {
		int weeks = weeksOfYear(year);
		if (weekth >= weeks) {
			return DateTime(year, 12, 31, 23, 59, 59).sectime();
		}
		return sectimeOfYWeekthStart(year, weekth+1) - 1;
	}

	// ------------------------------------------------------------------------
	// 获取当前时间值（1970 年元月凌晨到当前的秒数）
	inline static time_t now() {
		return time(NULL);
	}

	// ------------------------------------------------------------------------
	// 按 fmt 指定的格式解释日期时间字符串，生成 DateTime 对象，解释失败则返回 false
	// 格式化符号：
	//     %%		% 号
	//     %Y		带世纪部分的十进制年份
	//     %m		十进制表示的月份
	//     %d		十进制表示的每月的第几天
	//     %H		24 小时制的小时
	//     %M		十时制表示的分钟数
	//     %S		十进制的秒数
	// 例如：
	//     fmt = %Y-%m-%d %H:%M:%S 能解释出 2018-01-10 08:00:00
	// 注意：
	//     如果不是正确的日期值，会产生进位或退位，譬如：
	//     2018-2-30，二月没有 30 天，则会自动变为：2018-3-2
	// ------------------------------------------------------------------------
	static bool fmtParse(DateTime &dt, const string &fmt, const string &strDTime);

private:
	struct tm m_st;
};

};

NS_FS_END

#endif
