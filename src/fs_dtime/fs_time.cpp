/**
 * @file: fstime.cpp
 * @brief: 实现时分秒对象
 * @author: hyw
 * @version: 1.0
 * @date: 2018-01-14
 */


#include <vector>
#include "fs_str/fs_string.h"
#include "fs_str/fs_convert.h"
#include "fs_time.h"

using namespace std;

NS_FS_BEGIN

namespace fs_dtime {
	Time &Time::update(int hour, int min, int sec) {
		m_sec = sec % 60;
		min += sec / 60;
		m_min = min % 60;
		m_hour = hour + min / 60;
		if (m_sec < 0) {
			m_min -= 1;
			m_sec += 60;
		}
		if (m_min < 0) {
			m_hour -= 1;
			m_min += 60;
		}
		return *this;
	}

	Time &Time::addHour(int hour) {
		m_hour += hour;
		return *this;
	}

	Time &Time::addMin(int min) {
		int mins = m_min + min;
		m_min = mins % 60;
		m_hour += mins / 60;
		if (m_min < 0) {
			m_hour -= 1;
			m_min += 60;
		}
		return *this;
	}

	Time &Time::addSec(int sec) {
		int secs = m_sec + sec;
		m_sec = secs % 60;
		m_min += secs / 60;
		m_hour += m_min / 60;
		m_min = m_min % 60;
		if (m_sec < 0) {
			m_min -= 1;
			m_sec += 60;
		}
		if (m_min < 0) {
			m_hour -= 1;
			m_min += 60;
		}
		return *this;
	}

	bool Time::fromStr(const string &fmtTime, char sp) {
		vector<string> hms;
		fs_str::split(hms, fmtTime, sp, true);
		if (hms.size() != 3) { return false; }
		int hour, min, sec;
		if (!fs_str::str2rint32(hms[0], hour)) {
			return false;
		}
		if (!fs_str::str2rint32(hms[1], min)) {
			return false;
		}
		if (!fs_str::str2rint32(hms[2], sec)) {
			return false;
		}
		update(hour, min, sec);
		return true;
	}

};

NS_FS_END

