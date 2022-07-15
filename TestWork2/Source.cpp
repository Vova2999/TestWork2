#include <string>
#include <iostream>
#include <functional>
#include "ComplexMap.h"

using namespace std;

template <typename TValue>
string ArrayToString(TValue* array, int size) {
	string result;
	for (int i = 0; i < size - 1; i++)
		result += to_string(array[i]) + ", ";
	result += to_string(array[size - 1]);
	return result;
}

template <typename TValue>
bool ArraysEqual(TValue* firstArray, int firstSize, TValue* secondArray, int secondSize) {
	if (firstSize != secondSize)
		return false;
	for (int i = 0; i < firstSize; i++)
		if (firstArray[i] != secondArray[i])
			return false;
	return true;
}

template <typename TKey, typename TValue>
void AssertGetValue(ComplexMap<TKey, TValue>& complexMap, TKey key, TValue expectedValue) {
	TValue value = complexMap.GetValue(key);

	cout << "Key: " << key << " Value: " << value << endl;
	cout << "Key: " << key << " Value: " << expectedValue << " (test)" << endl << endl;

	if (value != expectedValue)
		throw "Values not equal!";
}

template <typename TKey, typename TValue>
void AssertGetArray(ComplexMap<TKey, TValue>& complexMap, TKey key, TValue* expectedArray, int expectedSize) {
	int size;
	TValue* array = complexMap.GetArray(key, &size);

	cout << "Key: " << key << " Size: " << size << " Values: " << ArrayToString(array, size) << endl;
	cout << "Key: " << key << " Size: " << expectedSize << " Values: " << ArrayToString(expectedArray, expectedSize) << " (test)" << endl << endl;

	if (!ArraysEqual(array, size, expectedArray, expectedSize))
		throw "Arrays not equal!";
}

template <typename TKey, typename TValue>
void AssertGetString(ComplexMap<TKey, TValue>& complexMap, TKey key, const char* expectedLine) {
	char* line = complexMap.GetString(key);

	cout << "Key: " << key << " Line: " << line << endl;
	cout << "Key: " << key << " Line: " << expectedLine << " (test)" << endl << endl;

	if (strcmp(line, expectedLine) != 0)
		throw "Strings not equal!";
}

template <typename TKey, typename TValue>
void AssertTryAddValue(ComplexMap<TKey, TValue>& complexMap, TKey key, TValue value, bool mustBeAdded) {
	bool isAdded = complexMap.TryAddValue(key, value);

	if (isAdded)
		cout << "Key: " << key << " Value: " << value << " added" << endl;
	else
		cout << "Key: " << key << " Value: " << value << " not added" << endl;

	if (isAdded != mustBeAdded)
		throw isAdded ? "Value must be not added!" : "Value must be added!";
}

template <typename TKey, typename TValue>
void AssertTryAddArray(ComplexMap<TKey, TValue>& complexMap, TKey key, TValue* array, int size, bool mustBeAdded) {
	bool isAdded = complexMap.TryAddArray(key, array, size);

	if (isAdded)
		cout << "Key: " << key << " Values: " << ArrayToString(array, size) << " added" << endl;
	else
		cout << "Key: " << key << " Values: " << ArrayToString(array, size) << " not added" << endl;

	if (isAdded != mustBeAdded)
		throw isAdded ? "Values must be not added!" : "Values must be added!";
}

template <typename TKey, typename TValue>
void AssertTryAddString(ComplexMap<TKey, TValue>& complexMap, TKey key, const char* line, bool mustBeAdded) {
	bool isAdded = complexMap.TryAddString(key, line);

	if (isAdded)
		cout << "Key: " << key << " Line: " << line << " added" << endl;
	else
		cout << "Key: " << key << " Line: " << line << " not added" << endl;

	if (isAdded != mustBeAdded)
		throw isAdded ? "Line must be not added!" : "Line must be added!";
}

template <typename TKey, typename TValue>
void AssertTryGetValue(ComplexMap<TKey, TValue>& complexMap, TKey key, bool mustBeFound, TValue expectedValue) {
	TValue value;
	bool isFound = complexMap.TryGetValue(key, &value);

	if (isFound)
		cout << "Key: " << key << " Value: " << value << endl;
	else
		cout << "Key: " << key << " Value: " << "none" << endl;

	if (mustBeFound)
		cout << "Key: " << key << " Value: " << expectedValue << " (test)" << endl << endl;
	else
		cout << "Key: " << key << " Value: " << "none" << " (test)" << endl << endl;

	if (isFound != mustBeFound)
		if (isFound)
			throw "Value is found!";
		else
			throw "Value is not found!";

	if (isFound && value != expectedValue)
		throw "Values not equal!";
}

template <typename TKey, typename TValue>
void AssertTryGetArray(ComplexMap<TKey, TValue>& complexMap, TKey key, bool mustBeFound, TValue* expectedArray, int expectedSize) {
	int size;
	TValue* array;
	bool isFound = complexMap.TryGetArray(key, &array, &size);

	if (isFound)
		cout << "Key: " << key << " Size: " << size << " Values: " << ArrayToString(array, size) << endl;
	else
		cout << "Key: " << key << " Size: " << "none" << " Values: " << "none" << endl;

	if (mustBeFound)
		cout << "Key: " << key << " Size: " << expectedSize << " Values: " << ArrayToString(expectedArray, expectedSize) << " (test)" << endl << endl;
	else
		cout << "Key: " << key << " Size: " << "none" << " Values: " << "none" << " (test)" << endl << endl;

	if (isFound != mustBeFound)
		if (isFound)
			throw "Value is found!";
		else
			throw "Value is not found!";

	if (isFound && !ArraysEqual(array, size, expectedArray, expectedSize))
		throw "Arrays not equal!";
}

template <typename TKey, typename TValue>
void AssertTryGetString(ComplexMap<TKey, TValue>& complexMap, TKey key, bool mustBeFound, const char* expectedLine) {
	char* line;
	bool isFound = complexMap.TryGetString(key, &line);

	if (isFound)
		cout << "Key: " << key << " Line: " << line << endl;
	else
		cout << "Key: " << key << " Line: " << "none" << endl;
	if (isFound)
		cout << "Key: " << key << " Line: " << expectedLine << " (test)" << endl << endl;
	else
		cout << "Key: " << key << " Line: " << "none" << " (test)" << endl << endl;

	if (isFound != mustBeFound)
		if (isFound)
			throw "Value is found!";
		else
			throw "Value is not found!";

	if (isFound && strcmp(line, expectedLine) != 0)
		throw "Strings not equal!";
}

void AssertConstCharException(const char* previewMessage, function<void()> action) {
	try {
		cout << previewMessage << endl;
		action();
	}
	catch (const char* message) {
		cout << "Exception found! Message: " << message << endl << endl;
		return;
	}

	cout << "Exception not found!" << endl << endl;
	throw "Exception not found!";
}

void SimpleTest() {
	ComplexMap<int, int> complexMap;

	if (complexMap.GetSize() != 0)
		throw "ComplexMap size not 0";

	complexMap.AddValue(16, 222);
	AssertGetValue(complexMap, 16, 222);
	complexMap.AddValue(63, 333);
	AssertGetValue(complexMap, 63, 333);

	int array1[] = { 4, 5, 6, 7 };
	int array2[] = { 74, 73, 72 };
	complexMap.AddArray(142, array1, sizeof(array1) / sizeof(int));
	AssertGetArray(complexMap, 142, array1, sizeof(array1) / sizeof(int));
	complexMap.AddArray(147, array2, sizeof(array2) / sizeof(int));
	AssertGetArray(complexMap, 147, array2, sizeof(array2) / sizeof(int));

	complexMap.AddString(993, "123123");
	AssertGetString(complexMap, 993, "123123");
	complexMap.AddString(995, "321321");
	AssertGetString(complexMap, 995, "321321");

	if (complexMap.GetSize() != 6)
		throw "ComplexMap size not 6";

	complexMap.RemoveAll();
	if (complexMap.GetSize() != 0)
		throw "ComplexMap size not 0";
}

void SimpleTestWithOtherTypes() {
	ComplexMap<double, double> complexMap;

	if (complexMap.GetSize() != 0)
		throw "ComplexMap size not 0";

	complexMap.AddValue(16.6, 222.2);
	AssertGetValue(complexMap, 16.6, 222.2);
	complexMap.AddValue(63.1, 333.7);
	AssertGetValue(complexMap, 63.1, 333.7);

	double array1[] = { 4.1, 5.2, 6.3, 7.4 };
	double array2[] = { 74.5, 73.6, 72.7 };
	complexMap.AddArray(142.9, array1, sizeof(array1) / sizeof(double));
	AssertGetArray(complexMap, 142.9, array1, sizeof(array1) / sizeof(double));
	complexMap.AddArray(147.8, array2, sizeof(array2) / sizeof(double));
	AssertGetArray(complexMap, 147.8, array2, sizeof(array2) / sizeof(double));

	complexMap.AddString(993.2, "123123");
	AssertGetString(complexMap, 993.2, "123123");
	complexMap.AddString(995.3, "321321");
	AssertGetString(complexMap, 995.3, "321321");

	if (complexMap.GetSize() != 6)
		throw "ComplexMap size not 6";

	complexMap.RemoveAll();
	if (complexMap.GetSize() != 0)
		throw "ComplexMap size not 0";
}

void ExceptionOnGetMissingKeys() {
	int size;
	ComplexMap<int, int> complexMap;

	AssertConstCharException("Check exception on GetValue by missing key", [&]() { complexMap.GetValue(16); });
	AssertConstCharException("Check exception on GetArray by missing key", [&]() { complexMap.GetArray(16, &size); });
	AssertConstCharException("Check exception on GetString by missing key", [&]() { complexMap.GetString(16); });
}

void ExceptionOnAddingDuplicateValue() {
	ComplexMap<int, int> complexMap;

	complexMap.AddValue(16, 222);
	AssertConstCharException("Check exception on AddValue", [&]() { complexMap.AddValue(16, 333); });

	int array1[] = { 4, 5, 6, 7 };
	int array2[] = { 74, 73, 72 };
	complexMap.AddArray(142, array1, sizeof(array1) / sizeof(int));
	AssertConstCharException("Check exception on AddArray", [&]() { complexMap.AddArray(142, array2, sizeof(array2) / sizeof(int)); });

	complexMap.AddString(993, "321321");
	AssertConstCharException("Check exception on AddString", [&]() { complexMap.AddString(993, "123123"); });
}

void ReplaceOnAddingDuplicateValue() {
	//ComplexMap<int, int> complexMap;

	//complexMap.AddValueOrReplace(16, 222);
	//complexMap.AddValueOrReplace(16, 333);
	//AssertGetValue(complexMap, 16, 333);

	//int array1[] = { 4, 5, 6, 7 };
	//int array2[] = { 74, 73, 72 };
	//complexMap.AddArrayOrReplace(142, array1, sizeof(array1) / sizeof(int));
	//complexMap.AddArrayOrReplace(142, array2, sizeof(array2) / sizeof(int));
	//AssertGetArray(complexMap, 142, array2, sizeof(array2) / sizeof(int));

	//complexMap.AddStringOrReplace(993, "321321");
	//complexMap.AddStringOrReplace(993, "123123");
	//AssertGetString(complexMap, 993, "123123");
}

void ExceptionOnGetInvalidType() {
	int size;
	ComplexMap<int, int> complexMap;

	complexMap.AddValue(16, 222);
	AssertConstCharException("Check exception on GetArray instead of GetValue", [&]() { complexMap.GetArray(16, &size); });
	AssertConstCharException("Check exception on GetString instead of GetValue", [&]() { complexMap.GetString(16); });

	int array1[] = { 4, 5, 6, 7 };
	complexMap.AddArray(142, array1, sizeof(array1) / sizeof(int));
	AssertConstCharException("Check exception on GetValue instead of GetArray", [&]() { complexMap.GetValue(142); });
	AssertConstCharException("Check exception on GetString instead of GetArray", [&]() { complexMap.GetString(142); });

	complexMap.AddString(993, "321321");
	AssertConstCharException("Check exception on GetValue instead of GetString", [&]() { complexMap.GetValue(993); });
	AssertConstCharException("Check exception on GetArray instead of GetString", [&]() { complexMap.GetArray(993, &size); });
}

void TryAddMethods() {
	ComplexMap<int, int> complexMap;

	complexMap.AddValue(16, 222);
	int array1[] = { 4, 5, 6, 7 };
	int array2[] = { 8, 9, 10 };
	complexMap.AddArray(142, array1, sizeof(array1) / sizeof(int));
	complexMap.AddString(993, "123123");

	AssertTryAddValue(complexMap, 16, 333, false);
	AssertTryAddValue(complexMap, 22, 444, true);
	AssertGetValue(complexMap, 16, 222);
	AssertGetValue(complexMap, 22, 444);

	AssertTryAddArray(complexMap, 142, array1, sizeof(array1) / sizeof(int), false);
	AssertTryAddArray(complexMap, 166, array2, sizeof(array2) / sizeof(int), true);
	AssertGetArray(complexMap, 142, array1, sizeof(array1) / sizeof(int));
	AssertGetArray(complexMap, 166, array2, sizeof(array2) / sizeof(int));

	AssertTryAddString(complexMap, 993, "123123", false);
	AssertTryAddString(complexMap, 111, "1231232", true);
	AssertGetString(complexMap, 993, "123123");
	AssertGetString(complexMap, 111, "1231232");
}

void TryGetMethods() {
	ComplexMap<int, int> complexMap;

	complexMap.AddValue(16, 222);
	int array1[] = { 4, 5, 6, 7 };
	complexMap.AddArray(142, array1, sizeof(array1) / sizeof(int));
	complexMap.AddString(993, "123123");

	AssertTryGetValue(complexMap, 16, true, 222);
	AssertTryGetValue(complexMap, 22, false, 0);
	AssertTryGetValue(complexMap, 142, false, 0);
	AssertTryGetValue(complexMap, 993, false, 0);

	AssertTryGetArray(complexMap, 142, true, array1, sizeof(array1) / sizeof(int));
	AssertTryGetArray(complexMap, 22, false, array1, 0);
	AssertTryGetArray(complexMap, 166, false, array1, 0);
	AssertTryGetArray(complexMap, 993, false, array1, 0);

	AssertTryGetString(complexMap, 993, true, "123123");
	AssertTryGetString(complexMap, 16, false, "");
	AssertTryGetString(complexMap, 142, false, "");
	AssertTryGetString(complexMap, 111, false, "");
}

void RemoveTempMemory() {
	ComplexMap<int, int> complexMap;

	int* array1 = new int[4]{ 4, 5, 6, 7 };
	complexMap.AddArray(142, array1, 4);
	delete[] array1;
	int* array1ForTest = new int[4]{ 4, 5, 6, 7 };
	AssertGetArray(complexMap, 142, array1ForTest, 4);
	AssertTryGetArray(complexMap, 142, true, array1ForTest, 4);

	char* line1 = new char[7]{ '1', '2', '3', '1', '2', '3', 0 };
	complexMap.AddString(993, line1);
	delete[] line1;
	AssertGetString(complexMap, 993, "123123");
	AssertTryGetString(complexMap, 993, true, "123123");
}

void main() {
	SimpleTest();
	SimpleTestWithOtherTypes();
	ExceptionOnGetMissingKeys();
	ExceptionOnAddingDuplicateValue();
	ReplaceOnAddingDuplicateValue();
	ExceptionOnGetInvalidType();
	TryAddMethods();
	TryGetMethods();
	RemoveTempMemory();

	system("pause>>void");
}