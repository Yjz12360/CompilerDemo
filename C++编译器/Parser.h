#pragma once
#include"Scanner.h"
#include"Assignment.h"
#include"Definition.h"
#include"Statement.h"
#include"SymbolTable.h"
#include"ForLoop.h"
#include<stack>
#include<map>
//语法分析器

class Parser {
private:
	//词法分析器
	Scanner scanner;

	//look用于保存当前读取到的Token
	Token look;

	//采用自上而下分析LL(1)文法推导
	//stack用于保存当前栈内元素
	//string首字符为大写字母表示非终止符，其余为终止符
	stack<string>tokenStack;

	//pair前一个string表示非终止符，后一个string表示当前读取到的Token（终止符）
	//vector表示选取的推导方式
	map<pair<string, string>, vector<string>>parsingTable;
	void addDeduce(string VN, string VT, vector<string>deduce);

	//用于保存哪些非终止符可以推导出ε
	vector<string>nullableSymbol;
	bool isNullable(string str);

	//语法分析读取到定义数据的语句（e. int i;）时，用此来临时保存相关信息
	Definition tempDef;
	void addData(Definition def);

	//语法分析读取到赋值语句（e. i = 1;）时，用此来保存相关信息
	Assignment tempAsg;
	bool assign(Assignment asg);

	//保存所有变量
	vector<SymbolTable>allData;


	//id名称到数据类型的映射
	//e. "i"-->"int"
	//语法分析的时候使用
	map<string, string>typeData;

	//根据id获取其值
	Token getValue(string idName);

	//根据输入的id确定其数据类型
	string getType(string id);

	//保存EXPRESSION的Token组成
	vector<Token>tempExpr;
	//表达式化简
	//化简一个含有加减乘除运算符，比较运算符和逻辑运算符和括号的表达式
	//运算符优先级： 括号 > 乘除 > 加减 > 比较运算符 > 逻辑运算符
	//外部只需要调用第一个simplify函数即可，其他子函数为其内部调用
	//最终结果得到一个数值或布尔值（0或1）的Token
	Token simplify(vector<Token>ex);
	//表达式化简的子函数
	//作用是化简一个含有加减乘除运算，比较运算符和逻辑运算符（不含括号）的表达式
	Token simplify_Sub1(vector<Token>ex);
	//表达式化简的第二层子函数
	//作用是化简一个只含有加减运算，比较运算符和逻辑运算符的表达式
	Token simplify_Sub2(vector<Token>ex);
	//表达式化简的第三层子函数
	//作用是化简一个只含有比较运算符和逻辑运算符的表达式
	Token simplify_Sub3(vector<Token>ex);
	//表达式化简的第四层子函数
	//作用是化简一个只含有逻辑运算符的表达式
	Token simplify_Sub4(vector<Token>ex);

	//保存全部定义的变量
	vector<Definition>allVar;
	
	//指示待检测代码是否已读取完毕
	bool endOfFile;

	//当前程序的嵌套结构
	//string表示结构类型（e. if句型（IF），while句型（WHILE）等）
	//int保存这个结构出现时tokenStack的深度（size），用于结构end的判断
	stack<pair<string,int>>structure;
	//将程序的嵌套结构保存在vector中
	vector<STATEMENT>stuStatement;

	//保存待测代码中所有的定义语句和赋值语句和表达式
	vector<Definition>allDef;
	vector<Assignment>allAssign;
	vector<vector<Token>>allExpr;//只包括条件语句和循环语句的执行条件表达式，赋值语句中的表达式不保存在内
	vector<string>allPrint;//string保存id

	//语法分析
	int analyse();

	//建立代码语句与不同类型语句之间索引的映射
	void process();
	map<int, int>gotoTable;
	map<int, int>exprTable;
	map<int, int>defTable;
	map<int, int>asgTable;
	map<int, int>printTable;

	//经过编译后，“运行”代码文件
	void execute();


public:
	Parser();
	bool match(Token tok);
	bool match(string str);
	bool nextToken();

	void compile();

	void printVar(string idName);

};