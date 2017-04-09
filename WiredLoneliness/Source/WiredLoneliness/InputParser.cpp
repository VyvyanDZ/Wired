#include "WiredLoneliness.h"
#include "InputParser.h"

bool InputParser::isPunc(char c)
{
	return delimeter.find(c) != string::npos;
}

void InputParser::cleanString(string & str)
{
	int len = str.length();
	string tmp = "";

	char prevChar = 0;

	for (int i = 0; i < len; ++i) {
		if ((isSpace(str[i]) && isSpace(prevChar)) || !isPunc(str[i])) {
			tmp += str[i];
			prevChar = str[i];
		}
		else if (isSpace(prevChar) && isPunc(str[i])) {
			tmp += ' ';
			prevChar = ' ';
		}
	}
	str = tmp;
}

void InputParser::copy(char * array[], vector<string>& v, size_t arraySize)
{
	for (int i = 0; i < arraySize; ++i) {
		if (array[i] != NULL) {
			v.push_back(array[i]);
		}
		else {
			break;
		}
	}
}

size_t InputParser::replace(string &str, string substring1, string substring2)
{
	size_t pos = str.find(substring1);

	if (pos != string::npos) {
		str.erase(pos, substring1.length());
		str.insert(pos, substring2);
	}
	return pos;
}

void InputParser::trimRight(string & str, string delimeter)
{
	size_t pos = str.find_last_not_of(delimeter);
	if (pos != string::npos) {
		str.erase(pos + 1, str.length());
	}
}

void InputParser::trimLeft(string & str, string delimeter)
{
	size_t pos = str.find_first_not_of(delimeter);
	if (pos != string::npos) {
		str.erase(0, pos);
	}
}

void InputParser::trimLeftRight(string & str, string delimeter)
{
	trimLeft(str, delimeter);
	trimRight(str, delimeter);
}

bool InputParser::isSpace(char c)
{
	return c == ' ';
}

void InputParser::tokenize(const string str, vector<string>& v)
{
	string buffer;

	for (int i = 0; i < str.length(); ++i) {
		if (!isPunc(str[i]) && !isSpace(str[i])) {
			buffer += str[i];
		}
		else if (!buffer.empty()) {
			v.push_back(buffer);
			buffer.erase();
		}
	}
	if ((v.empty() && !buffer.empty()) || !buffer.empty()) {
		v.push_back(buffer);
	}
}

void InputParser::lowercase(string & str)
{
	int len = str.length();

	for (int i = 0; i < len; ++i) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] -= 'a' - 'A';
		}
	}
}

