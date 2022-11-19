#include "SymbolTable.h"

bool SymbolTable::isUsed(string id)
{
	return getType(id) != NONE;
}

SymbolTable::SymbolTable()
{
}

bool SymbolTable::addData(string type, string id, string data)
{
	if (type == "int")return addData(INT, id, data);
	if (type == "float")return addData(FLOAT, id, data);
	return false;
}

bool SymbolTable::addData(TYPE type, string id, string data)
{
	if (isUsed(id))
		return false;
	switch (type) {
	case INT:intData.insert(pair<string, int>(id, stringToInt(data))); break;
	case FLOAT:floatData.insert(pair<string, float>(id, stringToFloat(data))); break;
	default:return false;
	}
	return true;
}

Token SymbolTable::search(string idName)
{
	string s;
	switch (getType(idName)) {
	case INT:s = intToString(intData.find(idName)->second); break;
	case FLOAT:s = floatToString(floatData.find(idName)->second); break;
	default:return Token();
	}
	return Token(NUM, s);
}

TYPE SymbolTable::getType(string idName)
{
	if (intData.find(idName) != intData.end())return INT;
	if (floatData.find(idName) != floatData.end())return FLOAT;
	return NONE;
}

bool SymbolTable::modifyData(string idName, string newData)
{
	TYPE t = getType(idName);
	switch (t) {
	case INT:intData.find(idName)->second = stringToInt(newData); break;
	case FLOAT:floatData.find(idName)->second = stringToFloat(newData); break;
	default:return false;
	}
	return true;
}

bool SymbolTable::deleteData(string idName)
{
	TYPE t = getType(idName);
	switch (t) {
	case INT:intData.erase(idName); break;
	case FLOAT:floatData.erase(idName); break;
	default:return false;
	}
	return true;
}


