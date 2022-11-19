#pragma once
#include<string>
#include<iostream>
#include"Tag.h"
using namespace std;
class Token{
private:
	TAG tag;
	string name;
public:
	Token();
	Token(TAG t, string s);
	TAG getTag();
	string getName();


	void display();
};