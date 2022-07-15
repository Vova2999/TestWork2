#pragma once

#include <map>
#include <algorithm>

using namespace std;

template <typename TKey, typename TValue>
class ComplexMap {
	class ValueType {
	public:
		virtual ~ValueType() = 0;
	};

	class SingleValueType : public ValueType {
	public:
		TValue Value;

		SingleValueType(TValue value)
			: Value{ value } {
		}

		virtual ~SingleValueType() {
		}
	};

	class ArrayValueType : public ValueType {
	public:
		TValue* Values;
		int Size;

		ArrayValueType(TValue* values, int size)
			: Size{ size } {
			Values = new TValue[size];
			memcpy_s(Values, size * sizeof(TValue), values, size * sizeof(TValue));
		}

		virtual ~ArrayValueType() {
			delete[] Values;
		}
	};

	class StringValueType : public ValueType {
	public:
		char* Line;

		StringValueType(const char* line) {
			int length = strlen(line) + 1;
			Line = new char[length];
			strcpy_s(Line, length, line);
		}

		virtual ~StringValueType() {
			delete[] Line;
		}
	};

	map<TKey, ValueType*> valuesMap;

	bool TryAddValueType(TKey key, ValueType* valueType) {
		pair<map<TKey, ValueType*>::iterator, bool> inserted = valuesMap.insert(pair<TKey, ValueType*>(key, valueType));
		if (inserted.second)
			return true;

		delete valueType;
		return false;
	}

	void AddValueType(TKey key, ValueType* valueType) {
		if (!TryAddValueType(key, valueType))
			throw "Key already exists";
	}

	void AddValueTypeOrReplace(int key, ValueType* valueType) {
		pair<map<int, ValueType*>::template iterator, bool> inserted = valuesMap.insert(pair<int, ValueType*>(key, valueType));
		if (inserted.second)
			return;

		delete inserted.first->second;
		inserted.first->second = valueType;
	}

	template<typename TValueType>
	TValueType* GetValueType(TKey key) {
		map<TKey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			throw "Key not found";

		TValueType* valueType = dynamic_cast<TValueType*>(value->second);
		if (valueType == nullptr)
			throw "Invalid value type";

		return valueType;
	}

	template<typename TValueType>
	TValueType* GetValueTypeOrNullptr(TKey key) {
		map<TKey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			return nullptr;

		TValueType* valueType = dynamic_cast<TValueType*>(value->second);
		if (valueType == nullptr)
			return nullptr;

		return valueType;
	}

	template<typename TValueType>
	TValueType* GetOrAddValueType(TKey key, TValueType* valueType) {
		pair<map<int, ValueType*>::template iterator, bool> inserted = valuesMap.insert(pair<int, ValueType*>(key, valueType));
		if (inserted.second)
			return valueType;

		delete valueType;
		valueType = dynamic_cast<TValueType*>(inserted.first->second);
		if (valueType == nullptr)
			throw "Invalid value type";

		return valueType;
	}

public:
	~ComplexMap() {
		RemoveAll();
	}

	int GetSize() {
		return valuesMap.size();
	}

	void AddValue(TKey key, TValue value) {
		AddValueType(key, new SingleValueType(value));
	}
	void AddArray(TKey key, TValue* values, int size) {
		AddValueType(key, new ArrayValueType(values, size));
	}
	void AddString(TKey key, const char* line) {
		AddValueType(key, new StringValueType(line));
	}

	bool TryAddValue(TKey key, TValue value) {
		return TryAddValueType(key, new SingleValueType(value));
	}
	bool TryAddArray(TKey key, TValue* values, int size) {
		return TryAddValueType(key, new ArrayValueType(values, size));
	}
	bool TryAddString(TKey key, const char* line) {
		return TryAddValueType(key, new StringValueType(line));
	}

	void AddValueOrReplace(TKey key, TValue value) {
		AddValueTypeOrReplace(key, new SingleValueType(value));
	}
	void AddArrayOrReplace(TKey key, TValue* values, int size) {
		AddValueTypeOrReplace(key, new ArrayValueType(values, size));
	}
	void AddStringOrReplace(TKey key, const char* line) {
		AddValueTypeOrReplace(key, new StringValueType(line));
	}

	TValue GetValue(TKey key) {
		SingleValueType* singleValue = GetValueType<SingleValueType>(key);
		return singleValue->Value;
	}
	TValue* GetArray(TKey key, int* size) {
		ArrayValueType* arrayValue = GetValueType<ArrayValueType>(key);
		*size = arrayValue->Size;
		return arrayValue->Values;
	}
	char* GetString(TKey key) {
		StringValueType* stringValue = GetValueType<StringValueType>(key);
		return stringValue->Line;
	}

	bool TryGetValue(TKey key, TValue* value) {
		SingleValueType* singleValue = GetValueTypeOrNullptr<SingleValueType>(key);
		if (singleValue == nullptr)
			return false;

		*value = singleValue->Value;
		return true;
	}
	bool TryGetArray(TKey key, TValue** values, int* size) {
		ArrayValueType* arrayValue = GetValueTypeOrNullptr<ArrayValueType>(key);
		if (arrayValue == nullptr)
			return false;

		*values = arrayValue->Values;
		*size = arrayValue->Size;
		return true;
	}
	bool TryGetString(TKey key, char** line) {
		StringValueType* stringValue = GetValueTypeOrNullptr<StringValueType>(key);
		if (stringValue == nullptr)
			return false;

		*line = stringValue->Line;
		return true;
	}

	TValue GetOrAddValue(TKey key, TValue value) {
		SingleValueType* singleValue = GetOrAddValueType(key, new SingleValueType(value));
		return singleValue->Value;
	}
	TValue* GetOrAddArray(TKey key, int* resultSize, TValue* values, int size) {
		ArrayValueType* arrayValue = GetOrAddValueType<ArrayValueType>(key, new ArrayValueType(values, size));
		*resultSize = arrayValue->Size;
		return arrayValue->Values;
	}
	char* GetOrAddString(TKey key, const char* line) {
		StringValueType* stringValue = GetOrAddValueType<StringValueType>(key, new StringValueType(line));
		return stringValue->Line;
	}

	void Remove(TKey key) {
		map<TKey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			throw "Key not found";

		valuesMap.erase(value);
	}
	bool TryRemove(TKey key) {
		map<TKey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			return false;

		valuesMap.erase(value);
		return true;
	}
	void RemoveAll() {
		for_each(valuesMap.begin(), valuesMap.end(),
			[](pair<TKey, ValueType*> value) {
				delete value.second;
			});
		valuesMap.clear();
	}
};

template <typename TKey, typename TValue>
ComplexMap<TKey, TValue>::ValueType::~ValueType() {}
