#pragma once
#include<string>
#include<map>
#include"Type.h"
#include"Token.h"
#include"GeneralFunction.h"
using namespace std;

class SymbolTable {
private:
	map<string, int>intData;
	map<string, float>floatData;
	bool isUsed(string id);
public:
	SymbolTable();
	bool addData(string type, string id, string data);
	bool addData(TYPE type, string id, string data);
	Token search(string idName);
	TYPE getType(string idName);
	bool modifyData(string idName, string newData);
	bool deleteData(string idName);
};