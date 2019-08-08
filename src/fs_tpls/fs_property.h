/**
 * @file: fs_property.h
 * @brief: 实现属性模拟
 * @author: hyw
 * @version: 1.0
 * @date: 2018-03-22
 * @attention:
 *	need c++11 support
 */

#ifndef __FS_PROPERTY_H__
#define __FS_PROPERTY_H__

#include <functional>
#include "fsmacros.h"

NS_FS_BEGIN

namespace fs_tpl {

// ---------------------------------------------------------------------------------------
// rproperty
// ---------------------------------------------------------------------------------------
template<typename ValueType>
class rproperty {
public:
	typedef std::function<ValueType()> Getter;

public:
	explicit rproperty(Getter getter) : m_getter(getter) {}

	operator ValueType() {
		m_value = m_getter();
		return m_value;
	}

protected:
	ValueType m_value;
	Getter m_getter;

public:
	ValueType operator~() const { return ~m_value; }
	ValueType operator!() const { return !m_value; }
	ValueType operator->() const { return m_value; }
	ValueType operator->() { return m_value; }
	ValueType operator+(const ValueType &value) const { return m_value + value; }
	ValueType operator-(const ValueType &value) const { return m_value - value; }
	ValueType operator*(const ValueType &value) const { return m_value * value; }
	ValueType operator/(const ValueType &value) const { return m_value / value; }
	ValueType operator%(const ValueType &value) const { return m_value % value; }
	ValueType operator^(const ValueType &value) const { return m_value ^ value; }
	ValueType operator&(const ValueType &value) const { return m_value & value; }
	ValueType operator|(const ValueType &value) const { return m_value | value; }
	ValueType operator>(const ValueType &value) const { return m_value > value; }
	ValueType operator<(const ValueType &value) const { return m_value < value; }
	ValueType operator>>(const ValueType &value) const { return m_value >> value; }
	ValueType operator<<(const ValueType &value) const { return m_value << value; }
	bool operator==(const ValueType &value) const { return m_value == value; }
	bool operator!=(const ValueType &value) const { return m_value != value; }
	bool operator<=(const ValueType &value) const { return m_value <= value; }
	bool operator>=(const ValueType &value) const { return m_value >= value; }
	bool operator&&(const ValueType &value) const { return m_value && value; }
	bool operator||(const ValueType &value) const { return m_value || value; }

private:
	friend std::ostream &operator<<(std::ostream &os, const rproperty<ValueType> &prop) {
		os << prop.m_value;
		return os;
	}
};


// ---------------------------------------------------------------------------------------
// rwproperty
// ---------------------------------------------------------------------------------------
template<typename ValueType>
class rwproperty : public rproperty<ValueType> {
public:
	typedef std::function<ValueType()> Getter;
	typedef std::function<void(ValueType)> Setter;

	rwproperty(Getter getter, Setter setter)
	: rproperty<ValueType>(getter)
	, m_setter(setter) {
	}

	rwproperty &operator=(const rwproperty &prop) {
		if (this != &prop) {
			this->m_value = prop.m_value;
		}
		m_setter(prop.m_value);
		return *this;
	}

	rwproperty &operator=(const ValueType &value) {
		this->m_value = value;
		m_setter(value);
		return *this;
	}

private:
	ValueType m_value;
	Setter m_setter;


public:
	rwproperty<ValueType> &operator++() {
		++m_value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator--() {
		--m_value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator++(int) {
		rwproperty<ValueType> tmp = *this;
		m_value++;
		m_setter(m_value);
		return *tmp;
	}

	rwproperty<ValueType> &operator--(int) {
		rwproperty<ValueType> tmp = *this;
		m_value--;
		m_setter(m_value);
		return *tmp;
	}

	rwproperty<ValueType> &operator+=(const ValueType &value) {
		m_value += value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator-=(const ValueType &value) {
		m_value -= value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator*=(const ValueType &value) {
		m_value *= value;
		m_setter(m_value);
		return *this;
	}
	rwproperty<ValueType> &operator/=(const ValueType &value) {
		m_value /= value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator%=(const ValueType &value) {
		m_value %= value;
		m_setter(m_value);
		return *this;
	}
	rwproperty<ValueType> &operator^=(const ValueType &value) {
		m_value ^= value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator&=(const ValueType &value) {
		m_value &= value;
		m_setter(m_value);
		return *this;
	}
	rwproperty<ValueType> &operator|=(const ValueType &value) {
		m_value |= value;
		m_setter(m_value);
		return *this;
	}

	rwproperty<ValueType> &operator>>=(const ValueType &value) {
		m_value >>= value;
		m_setter(m_value);
		return *this;
	}
	rwproperty<ValueType> &operator<<=(const ValueType &value) {
		m_value <<= value;
		m_setter(m_value);
		return *this;
	}
private:
	friend std::istream &operator>>(std::istream &is, rwproperty<ValueType> &prop) {
		is >> prop.m_value;
		prop.m_setter(prop.m_value);
		return is;
	}
};


// ---------------------------------------------------------------------------------------
// use macros
// ---------------------------------------------------------------------------------------
#define fs_get(ValueType) [&]()->ValueType
#define fs_set(ValueType) [&](ValueType value)
#define fs_rproperty(type, name) rproperty<type> name = rproperty<type>
#define fs_rwproperty(type, name) rwproperty<type> name = rwproperty<type>
#define fs_rfixproperty(type, name) \
	private: \
	type m_##name; \
	public: \
	rproperty<type> name = rproperty<type> ( \
		fs_get(type) { return m_##name; }, \
	)

#define fs_rwfixproperty(type, name) \
	private: \
	type m_##name; \
	public: \
	rwproperty<type> name = rwproperty<type> ( \
		fs_get(type) { return m_##name; }, \
		fs_set(type) { m_##name = value; } \
	)
};
NS_FS_END
#endif

// --------------------------------------------------------------------------------------
// 用法
/* --------------------------------------------------------------------------------------
class Info {
public:
	void func() {
		cout << "xxxxxxx" << endl;
	}
};


class A {
public:
	fs_rwproperty(string, value) (
		fs_get(string) {				// get 函数
			return m_value;
		},
		fs_set(string) {				// set 函数
			m_value = value;
		}
	);

	fs_rwfixproperty(Infoi*, pInfo);

private;
	string m_value;
};

int main() {
	A a;
	a.pInfo->func();
	a.value = "yyyyyyyyyyyy";
	cout << a.value << endl;
	return 0;
}
* -------------------------------------------------------------------------------------*/

