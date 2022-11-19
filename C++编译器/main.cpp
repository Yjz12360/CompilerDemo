//C++编译器
//控制台程序
//输入：一段储存在文本文件中的代码
//输出：输入代码的运行结果
//实现的功能：赋值语句，if句型，if-else句型，while句型，for句型等

#include<fstream>
#include<iostream>
#include"Scanner.h"
#include"Token.h"
#include"Parser.h"
using namespace std;



int main() {
	Parser parser;

	parser.compile();

	system("pause");
}