#pragma once
#include<math.h>
#include"KeyWord.h"
#include"InvalidSymbol.h"
#include"Type.h"

const static bool isDigit(char ch) {
	return ch >= '0'&&ch <= '9';
}

const static bool isBlank(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\n';
}

const static bool isLowerCase(char ch) {
	return ch >= 'a'&&ch <= 'z';
}

const static bool isUpperCase(char ch) {
	return ch >= 'A'&&ch <= 'Z';
}

const static bool isCharacter(char ch) {
	return isLowerCase(ch) || isUpperCase(ch);
}

const static bool isKeyWord(string str) {
	for (int i = 0; i < numOfKeyWord; ++i)
		if (str == keyWord[i])
			return true;
	return false;
}

const static bool isInvalidSymbol(string str) {
	for (int i = 0; i < numOfInvalidSymbol; ++i)
		if (str == invalidSymbol[i])
			return true;
	return false;
}

const static bool isType(string str) {
	for (int i = 0; i < numOfType; ++i)
		if (str == type[i])
			return true;
	return false;
}

const static int stringToInt(string str) {
	if (str.empty())return 0;
	int mul = 1;
	if (str[0] == '-') {
		mul = -1;
		str = str.substr(1, str.size());
	}
	int split = 0;
	for (; split < str.size(); ++split)
		if (!isDigit(str[split]))
			break;
	str = str.substr(0, split);
	int sum = 0;
	for (int i = 0; i < str.size(); ++i) {
		if (!isDigit(str[i]))
			return 0;
		sum = sum * 10 + (str[i] - '0');
	}
	return sum*mul;
}

const static float stringToFloat(string str) {
	int pointPos = -1;
	for (int i = 0; i < str.size(); ++i) {
		if (i == 0 && str[i] == '-')continue;
		if (str[i] == '.') {
			if (pointPos == -1)
				pointPos = i;
			else
				return 0;
		}
		else if (!isDigit(str[i]))
			return 0;
	}
	if (pointPos == -1)
		return (float)stringToInt(str);
	int integral = stringToInt(str.substr(0, pointPos));
	int decimal = stringToInt(str.substr(pointPos + 1));
	return (float)(integral + decimal*pow(0.1, str.substr(pointPos + 1).size()));
}

const static string floatToString(float f) {
	char ch[20];
	sprintf_s(ch, "%f", f);
	return (string)ch;
}

const static string intToString(int n) {
	char ch[20];
	sprintf_s(ch, "%d", n);
	return (string)ch;
}

const static bool isComparsionOperator(string str) {
	return str == "<=" || str == ">=" || str == "==" || str == "!=" || str == ">" || str == "<";
}
