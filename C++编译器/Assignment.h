#pragma once
#include<string>
using namespace std;

struct Assignment {
	string type;
	string idName;
	vector<Token>expression;
	Assignment() :type(""), idName(""){}
	Assignment(string t, string id, string dt) :type(t), idName(id) {}
	void Clr() { type = ""; idName = "";  expression.clear(); }
};