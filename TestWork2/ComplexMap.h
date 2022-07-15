#pragma once

#include <map>
#include <algorithm>

using namespace std;

template <typename Tkey, typename TValue>
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

	map<Tkey, ValueType*> valuesMap;

	void AddValueType(Tkey key, ValueType* valueType) {
		pair<map<Tkey, ValueType*>::iterator, bool> inserted = valuesMap.insert(pair<Tkey, ValueType*>(key, valueType));
		if (inserted.second)
			return;

		delete valueType;
		throw "Key already exists";
	}

	void AddValueTypeOrReplace(Tkey key, ValueType* valueType) {
		pair<map<Tkey, ValueType*>::template iterator, bool> inserted = valuesMap.insert(pair<Tkey, ValueType*>(key, valueType));
		if (inserted.second)
			return;

		delete inserted.first->second;
		inserted.first->second = valueType;
	}

	template<typename TValueType>
	TValueType* GetValueType(Tkey key) {
		map<Tkey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			throw "Key not found";

		TValueType* valueType = dynamic_cast<TValueType*>(value->second);
		if (valueType == nullptr)
			throw "Invalid value type";

		return valueType;
	}

	template<typename TValueType>
	TValueType* GetValueTypeOrNullptr(Tkey key) {
		map<Tkey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			return nullptr;

		TValueType* valueType = dynamic_cast<TValueType*>(value->second);
		if (valueType == nullptr)
			return nullptr;

		return valueType;
	}

public:
	~ComplexMap() {
		RemoveAll();
	}

	int GetSize() {
		return valuesMap.size();
	}

	void AddValue(Tkey key, TValue value) {
		AddValueType(key, new SingleValueType(value));
	}
	void AddArray(Tkey key, TValue* values, int size) {
		AddValueType(key, new ArrayValueType(values, size));
	}
	void AddString(Tkey key, const char* line) {
		AddValueType(key, new StringValueType(line));
	}

	void AddValueOrReplace(Tkey key, TValue value) {
		AddValueTypeOrReplace(key, new SingleValueType(value));
	}
	void AddArrayOrReplace(Tkey key, TValue* values, int size) {
		AddValueTypeOrReplace(key, new ArrayValueType(values, size));
	}
	void AddStringOrReplace(Tkey key, const char* line) {
		AddValueTypeOrReplace(key, new StringValueType(line));
	}

	TValue GetValue(Tkey key) {
		SingleValueType* singleValue = GetValueType<SingleValueType>(key);
		return singleValue->Value;
	}
	TValue* GetArray(Tkey key, int* size) {
		ArrayValueType* arrayValue = GetValueType<ArrayValueType>(key);
		*size = arrayValue->Size;
		return arrayValue->Values;
	}
	char* GetString(Tkey key) {
		StringValueType* stringValue = GetValueType<StringValueType>(key);
		return stringValue->Line;
	}

	bool TryGetValue(Tkey key, TValue* value) {
		SingleValueType* singleValue = GetValueTypeOrNullptr<SingleValueType>(key);
		if (singleValue == nullptr)
			return false;

		*value = singleValue->Value;
		return true;
	}
	bool TryGetArray(Tkey key, TValue** values, int* size) {
		ArrayValueType* arrayValue = GetValueTypeOrNullptr<ArrayValueType>(key);
		if (arrayValue == nullptr)
			return false;

		*values = arrayValue->Values;
		*size = arrayValue->Size;
		return true;
	}
	bool TryGetString(Tkey key, char** line) {
		StringValueType* stringValue = GetValueTypeOrNullptr<StringValueType>(key);
		if (stringValue == nullptr)
			return false;

		*line = stringValue->Line;
		return true;
	}

	void Remove(Tkey key) {
		map<Tkey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			throw "Key not found";

		valuesMap.erase(value);
	}
	bool TryRemove(Tkey key) {
		map<Tkey, ValueType*>::template iterator value = valuesMap.find(key);
		if (value == valuesMap.end())
			return false;

		valuesMap.erase(value);
		return true;
	}
	void RemoveAll() {
		for_each(valuesMap.begin(), valuesMap.end(),
			[](pair<Tkey, ValueType*> value) {
				delete value.second;
			});
		valuesMap.clear();
	}
};

template <typename Tkey, typename TValue>
ComplexMap<Tkey, TValue>::ValueType::~ValueType() {}
