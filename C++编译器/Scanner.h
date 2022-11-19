#pragma once
//词法分析器
#include<fstream>
#include<map>
#include<vector>
using namespace std;
#include"Token.h"
#include"GeneralFunction.h"



class Scanner {
private:
	//ch用于保存当前读取到的字符
	char ch;
	
	//buffer用于临时保存当前读取的Token的名称
	string buffer;

	//从文件中读取源代码
	FILE* pFile;
public:
	Scanner();

	//读取一个字符
	void readCh();

	//判断源代码文件是否已全部读取完毕
	bool hasNext();

	//词法分析得到下一个Token
	Token getNextToken();
	void readNum();
	void readWord();
	void readSymbol();

	~Scanner();
};