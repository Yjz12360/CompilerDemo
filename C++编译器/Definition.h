#pragma once
#include<string>
using namespace std;

struct Definition {
	string type;
	string idName;
	Definition() :type(""), idName("") {}
	Definition(string t, string id) :type(t), idName(id) {}
	void Clr() { type = ""; idName = ""; }
};