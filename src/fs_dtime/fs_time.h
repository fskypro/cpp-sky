/**
 * @file: fsdaytime.h
 * @brief: 实现时分秒对象
 * @author: hyw
 * @version: 1.0
 * @date: 2018-01-14
 */

#ifndef __FS_DAY_TIME_H__
#define __FS_DAY_TIME_H__

#include <sstream>
#include <algorithm>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN

namespace fs_dtime {

class Time {
public:
	Time()
	: m_hour(0)
	, m_min(0)
	, m_sec(0) {
	}

	Time(int hour, int min, int sec) {
		update(hour, min, sec);
	}

	explicit Time(int secs)
	{
		update(0, 0, secs);
	}

	// ----------------------------------------------------
	inline Time &clean() {
		m_hour = m_min = m_sec = 0;
		return *this;
	}

public:
	inline int hour() const { return m_hour; }
	inline int min() const { return m_min; }
	inline int sec() const { return m_sec; }
	inline int secs() const { return m_hour * 3600 + m_min * 60 + m_sec; }

	// ------------------------------------------
	Time &addHour(int hour);
	Time &addMin(int min);
	Time &addSec(int sec);
	Time &update(int hour, int min, int sec);

	// ------------------------------------------
	bool operator==(const Time& t) {
		return m_hour == t.m_hour && m_min == t.m_min && m_sec == t.m_sec;
	}

	bool operator<(const Time& t) {
		return secs() < t.secs();
	}

	bool operator<=(const Time& t) {
		return secs() <= t.secs();
	}

	bool operator>(const Time& t) {
		return secs() > t.secs();
	}

	bool operator>=(const Time& t) {
		return secs() >= t.secs();
	}

public:
	// ------------------------------------------------------------------------
	// 解释字符串形式的时间，如果参数 sp 为 “:” 号，则格式为：
	//     23:15:56
	// 成功则返回 true，否则返回 false
	// ------------------------------------------------------------------------
	bool fromStr(const string &str, char sp=':');

	string toString() const {
		stringstream ss;
		ss << "Time[hour=" << m_hour << ", min=" << m_min << ", sec=" << m_sec << "]";
		return ss.str();
	}

private:
	int m_hour;
	int m_min;
	int m_sec;
};
};

NS_FS_END
#endif
