/**
 * @file: fs_collection.h
 * @brief: 集合类型
 * @author: hyw
 * @version: 1.0
 * @date: 2018-03-21
 */

 #ifndef __FS_COLLECTION_H__
#define __FS_COLLECTION_H__

 #include <vector>
 #include <set>

NS_FS_BEGIN
namespace fs_type {
	// -------------------------------------------------------------------------------------
	// 提取 map key 和 value
	// -------------------------------------------------------------------------------------
	// 提取 key value 类型集合的 key 到 vector
	template<typename TKeyValue>
	std::vector<typename TKeyValue::key_type> vectorKeys(const TKeyValue &kvs) {
		std::vector<typename TKeyValue::key_type> keys;
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			keys.push_back(iter->first);
		}
		return keys;
	}

	// 提取 key value 类型集合的 key 到 set
	template<typename TKeyValue>
	typename std::set<typename TKeyValue::key_type> setKeys(const TKeyValue &kvs) {
		std::set<typename TKeyValue::key_type> keys;
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			keys.insert(iter->first);
		}
		return keys;
	}

	// 提取 key value 类型集合的 value 到 vector
	template<typename TKeyValue>
	std::vector<typename TKeyValue::mapped_type> vectorValues(const TKeyValue &kvs) {
		std::vector<typename TKeyValue::mapped_type> values;
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			values.push_back(iter->second);
		}
		return values;
	}

	// 提取 key value 类型集合的 value 到 set
	template<typename TKeyValue>
	std::set<typename TKeyValue::mapped_type> setValues(const TKeyValue &kvs) {
		std::set<typename TKeyValue::mapped_type> values;
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			values.insert(iter->second);
		}
		return values;
	}

	// -------------------------------------------------------------------------------------
	// 元素是否存在
	// -------------------------------------------------------------------------------------
	template<typename Elem, typename T>
	bool __fnpick(const Elem &elem, const T &value) { return value == elem; }

	// 判断元素是否在集合中
	template<typename Elems, typename T>
	bool contains(const Elems &elems, const T &value,
		bool (*fn)(const typename Elems::value_type&, const T&)=__fnpick<typename Elems::value_type, T>)
	{
		typename Elems::const_iterator iter = elems.begin();
		for (; iter != elems.end(); ++ iter) {
			if (fn(*iter, value)) { return true; }
		}
		return false;
	}

	// 判断元素是否在 map keys 中
	template<typename TKeyValue, typename T>
	bool hasKey(const TKeyValue &kvs, const T &value,
		bool (*fn)(const typename TKeyValue::key_type&, const T&)=__fnpick<typename TKeyValue::key_type, T>)
	{
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			if (fn(iter->first, value)) { return true; }
		}
		return false;
	}

	// 判断元素是否在 map values 中
	template<typename TKeyValue, typename T>
	bool hasValue(const TKeyValue &kvs, const T &value,
		bool (*fn)(const typename TKeyValue::mapped_type&, const T&)=__fnpick<typename TKeyValue::mapped_type, T>)
	{
		typename TKeyValue::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			if (fn(iter->second, value)) { return true; }
		}
		return false;
	}

	// -------------------------------------------------------------------------------------
	// 将 key value 集合的 key value 反向提取返回
	// -------------------------------------------------------------------------------------
	// 将 map 的 key value 改为 value key
	template<typename Key, typename Value>
	std::map<Value, Key> toValueKey(const std::map<Key, Value> &kvs) {
		std::map<Value, Key> vks;
		typename std::map<Key, Value>::const_iterator iter = kvs.begin();
		for (; iter != kvs.end(); ++iter) {
			vks[iter->second] = iter->first;
		}
		return vks;
	}
};
NS_FS_END
 #endif
