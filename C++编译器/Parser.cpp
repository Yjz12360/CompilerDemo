#include "Parser.h"

void Parser::addDeduce(string VN, string VT, vector<string> deduce)
{
	parsingTable.insert(pair < pair<string, string>, vector<string>>(pair<string, string>(VN, VT), deduce));
}

bool Parser::isNullable(string str)
{
	for (int i = 0; i < nullableSymbol.size(); ++i)
		if (str == nullableSymbol[i])
			return true;
	return false;
}

void Parser::addData(Definition def)
{
	if (def.type == "int")
		allData[allData.size() - 1].addData(INT, def.idName, "");
	else if (def.type == "float")
		allData[allData.size() - 1].addData(FLOAT, def.idName, "");
}

bool Parser::assign(Assignment asg)
{
	for (int i = allData.size() - 1; i >= 0; --i) {
		Token tok = simplify(asg.expression);
		if (allData[i].modifyData(asg.idName, tok.getName()))
			return true;
	}
	return false;
}

Token Parser::getValue(string idName)
{
	for (int i = allData.size() - 1; i >= 0; --i) {
		Token tok = allData[i].search(idName);
		if (tok.getName() != "")
			return tok;
	}
	return Token();
}

string Parser::getType(string id)
{
	if (typeData.find(id) != typeData.end())
		return typeData.find(id)->second;
	else
		return "";
}

Token Parser::simplify(vector<Token> ex)
{
	//先把表达式里面的id转换成对应数值
	for (int i = 0; i < ex.size(); ++i) {
		if (ex[i].getTag() == ID) {
			ex[i] = getValue(ex[i].getName());
		}
	}
	//开始化简
	vector<vector<Token>>vvTok;
	vvTok.push_back(vector<Token>());
	for (int i = 0; i < ex.size(); ++i) {
		if (ex[i].getName() == "(") {
			vvTok.push_back(vector<Token>());
		}
		else if (ex[i].getName() == ")") {
			Token tok = simplify_Sub1(vvTok[vvTok.size() - 1]);
			vvTok.pop_back();
			vvTok[vvTok.size() - 1].push_back(tok);
		}
		else {
			vvTok[vvTok.size() - 1].push_back(ex[i]);
		}
	}
	return simplify_Sub1(vvTok[0]);
}

Token Parser::simplify_Sub1(vector<Token> ex)
{
	if (ex.empty())return Token();
	vector<Token>vTok;
	vTok.push_back(ex[0]);
	float op1, op2;
	for (int i = 1; i < ex.size(); ++i) {
		string op = ex[i - 1].getName();
		if (op == "*" || op == "/" || op == "%") {
			op1 = stringToFloat(vTok[vTok.size() - 2].getName());
			op2 = stringToFloat(ex[i].getName());
			vTok.pop_back();
			vTok.pop_back();
			if (op == "*")
				vTok.push_back(Token(NUM, floatToString(op1 * op2)));
			else if (op == "/")
				vTok.push_back(Token(NUM, floatToString(op1 / op2)));
			else if (op == "%")
				vTok.push_back(Token(NUM, intToString((int)op1 % (int)op2)));
		}
		else {
			vTok.push_back(ex[i]);
		}
	}
	return simplify_Sub2(vTok);
}

Token Parser::simplify_Sub2(vector<Token> ex)
{
	if (ex.empty())return Token();
	vector<Token>vTok;
	vTok.push_back(ex[0]);
	float op1, op2;
	for (int i = 1; i < ex.size(); ++i) {
		string op = ex[i - 1].getName();
		if (op == "+" || op == "-") {
			op1 = stringToFloat(vTok[vTok.size() - 2].getName());
			op2 = stringToFloat(ex[i].getName());
			vTok.pop_back();
			vTok.pop_back();
			if (op == "+")
				vTok.push_back(Token(NUM, floatToString(op1 + op2)));
			else if (op == "-")
				vTok.push_back(Token(NUM, floatToString(op1 - op2)));
		}
		else {
			vTok.push_back(ex[i]);
		}
	}
	//如果vTok只剩下一个元素，说明表达式没有比较运算符和逻辑运算符
	//即这个表达式是只有加减乘除和括号运算的算术表达式
	if (vTok.size() == 1)
		return vTok[0];
	else
		return simplify_Sub3(vTok);
}

Token Parser::simplify_Sub3(vector<Token> ex)
{
	if (ex.empty())return Token();
	vector<Token>vTok;
	vTok.push_back(ex[0]);
	float op1, op2;
	for (int i = 1; i < ex.size(); ++i) {
		string op = ex[i - 1].getName();
		if (isComparsionOperator(op)) {
			op1 = stringToFloat(vTok[vTok.size() - 2].getName());
			op2 = stringToFloat(ex[i].getName());
			vTok.pop_back();
			vTok.pop_back();
			if (op == ">=")
				vTok.push_back(Token(NUM, op1 >= op2 ? "1" : "0"));
			else if (op == "<=")
				vTok.push_back(Token(NUM, op1 <= op2 ? "1" : "0"));
			else if (op == "==")
				vTok.push_back(Token(NUM, op1 == op2 ? "1" : "0"));
			else if (op == "!=")
				vTok.push_back(Token(NUM, op1 != op2 ? "1" : "0"));
			else if (op == ">")
				vTok.push_back(Token(NUM, op1 > op2 ? "1" : "0"));
			else if (op == "<")
				vTok.push_back(Token(NUM, op1 < op2 ? "1" : "0"));
		}
		else {
			vTok.push_back(ex[i]);
		}
	}
	return simplify_Sub4(vTok);
}

Token Parser::simplify_Sub4(vector<Token> ex)
{
	if (ex.empty())return Token();
	vector<Token>vTok;
	vTok.push_back(ex[0]);
	int op1, op2;
	for (int i = 1; i < ex.size(); ++i) {
		string op = ex[i - 1].getName();
		if (op == "&&") {
			op1 = stringToInt(vTok[vTok.size() - 2].getName());
			op2 = stringToInt(ex[i].getName());
			vTok.pop_back();
			vTok.pop_back();
			vTok.push_back(Token(NUM, intToString(op1 && op2)));
		}
		else {
			vTok.push_back(ex[i]);
		}
	}
	int b = 0;
	for (int i = 0; i < vTok.size(); ++i) {
		if (i % 2 == 0) {
			b = b || stringToInt(vTok[i].getName());
		}
		else if (vTok[i].getName() != "||")
			return Token();
	}
	return Token(NUM, intToString(b));
}



void Parser::process()
{
	int asgCur = 0, defCur = 0, exprCur = 0, printCur = 0, forCur = 0;
	stack<int>stkBegin;
	stack<int>stkBreak;
	stack<int>stkContinue;
	//对For循环的语句分别处理
	vector<vector<int>>vecFor;//储存For循环结构相关的信息，5元数组依次表示Init,Begin,Repeat,First,End的位置
	int head;
	for (int i = 0; i < stuStatement.size(); ++i) {
		switch (stuStatement[i]) {
		case ASSIGNMENT:
			asgTable.insert(pair<int, int>(i, asgCur++));
			break;
		case DEFINITION:
			defTable.insert(pair<int, int>(i, defCur++));
			break;
		case PRINT_VAR:
			printTable.insert(pair<int, int>(i, printCur++));
			break;
		case BREAK:
			stkBreak.push(i);
			break;
		case CONTINUE:
			stkContinue.push(i);
			break;
		case IF_BEGIN:
		case WHILE_BEGIN:
			exprTable.insert(pair<int, int>(i, exprCur++));
		case DO_BEGIN:
		case ELSE_BEGIN:
			stkBegin.push(i);
			break;
		case DO_END:
			exprTable.insert(pair<int, int>(i, exprCur++));
		case IF_END:
		case ELSE_END:
		case WHILE_END:
			head = stkBegin.top();
			stkBegin.pop();
			if (stuStatement[i] == IF_END)
				gotoTable.insert(pair<int, int>(head, i + 1));
			else if (stuStatement[i] == ELSE_END)
				gotoTable.insert(pair<int, int>(head - 1, i + 1));
			else if (stuStatement[i] == WHILE_END) {
				gotoTable.insert(pair<int, int>(head, i + 1));
				gotoTable.insert(pair<int, int>(i, head));
			}
			else if (stuStatement[i] == DO_END)
				gotoTable.insert(pair<int, int>(i, head));
			//break语句，continue语句的跳转
			if (stuStatement[i] == WHILE_END || stuStatement[i] == DO_END) {
				while (!stkBreak.empty()) {
					gotoTable.insert(pair<int, int>(stkBreak.top(), i + 1));
					stkBreak.pop();
				}
				while (!stkContinue.empty()) {
					gotoTable.insert(pair<int, int>(stkContinue.top(), head));
					stkContinue.pop();
				}
			}
			break;
		case FOR_INIT:
			vecFor.push_back(vector<int>()); 
		case FOR_BEGIN: 
		case FOR_REPEAT:
			vecFor[vecFor.size() - 1].push_back(i);
			break;
		case FOR_FIRST:
			vecFor[vecFor.size() - 1].push_back(i);
			exprTable.insert(pair<int, int>(i, exprCur++));
			break;
		case FOR_END:
			vecFor[vecFor.size() - 1].push_back(i);
			gotoTable.insert(pair<int, int>(vecFor[vecFor.size() - 1][1], vecFor[vecFor.size() - 1][3]));
			gotoTable.insert(pair<int, int>(vecFor[vecFor.size() - 1][4], vecFor[vecFor.size() - 1][2]));
			gotoTable.insert(pair<int, int>(vecFor[vecFor.size() - 1][3], vecFor[vecFor.size() - 1][4] + 1));
			vecFor.pop_back();
			break;
		}
	}
}

void Parser::execute()
{
	int index;
	Token tok;
	int cur = 0;
	while (cur < stuStatement.size()) {
		STATEMENT stmt = stuStatement[cur];
		//变量符号表栈的压栈出栈
		if (cur > 0) {
			STATEMENT lastStmt = stuStatement[cur - 1];
			if (lastStmt == IF_BEGIN || lastStmt == ELSE_BEGIN || lastStmt == WHILE_BEGIN ||
				lastStmt == DO_BEGIN || lastStmt == FOR_INIT || lastStmt == FOR_FIRST)
				allData.push_back(SymbolTable());
		}
		if (stmt == IF_END || stmt == ELSE_END || stmt == WHILE_END ||
			stmt == DO_END || stmt == BREAK || stmt == CONTINUE || stmt == FOR_END)
			allData.pop_back();
		switch (stmt) {
		case ASSIGNMENT:
			index = asgTable.find(cur)->second;
			assign(allAssign[index]);
			break;
		case DEFINITION:
			index = defTable.find(cur)->second;
			addData(allDef[index]);
			break;
		case PRINT_VAR:
			index = printTable.find(cur)->second;
			printVar(allPrint[index]);
			break;
		case IF_BEGIN:
		case WHILE_BEGIN:
		case FOR_FIRST:
			index = exprTable.find(cur)->second;//
			tok = simplify(allExpr[index]);
			if (tok.getName() == "0") {
				cur = gotoTable.find(cur)->second;
				if (stmt == FOR_FIRST)
					allData.pop_back();
				continue;
			}
			break;
		case IF_END:
			if (gotoTable.find(cur) != gotoTable.end()) {
				cur = gotoTable.find(cur)->second;
				continue;
			}
			break;
		case FOR_BEGIN:
		case FOR_END:
		case WHILE_END:
		case BREAK:
		case CONTINUE:
			cur = gotoTable.find(cur)->second;
			continue;
		case RETURN:
			cur = stuStatement.size();
			break;
		case DO_END:
			index = exprTable.find(cur)->second;
			tok = simplify(allExpr[index]);
			if (tok.getName() == "0") {
				cur = gotoTable.find(cur)->second;
				continue;
			}
			break;
		}
		cur++;
	}
}

Parser::Parser()
{
	tokenStack.push("START");

	//文法G[S]:
	//START -->int id ( ) BLOCK
	//BLOCK -->{ NODE }
	//NODE  -->STMT NODE | ε
	//STMT  -->SEQ
	//SEQ   -->id = EXPR
	//SEQ   -->TYPE id ;
	//EXPR  -->num
	//...

	nullableSymbol.push_back("NODE");
	nullableSymbol.push_back("ELSE");
	nullableSymbol.push_back("DASG_");
	nullableSymbol.push_back("FASG_");
	nullableSymbol.push_back("CDEF_");
	nullableSymbol.push_back("EXPR_");

	addDeduce("START", "int", { "int", "id", "(",")","BLOCK" });
	addDeduce("BLOCK", "{", { "{", "NODE", "}" });
	addDeduce("BLOCK", "id", { "STMT" });
	addDeduce("BLOCK", "int", { "STMT" });
	addDeduce("BLOCK", "float", { "STMT" });
	addDeduce("BLOCK", "break", { "STMT" });
	addDeduce("BLOCK", "continue", { "STMT" });
	addDeduce("BLOCK", "return", { "STMT" });
	addDeduce("BLOCK", "if", { "STMT" });
	addDeduce("BLOCK", "while", { "STMT" });
	addDeduce("BLOCK", "do", { "STMT" });
	addDeduce("BLOCK", "for", { "STMT" });
	addDeduce("BLOCK", "printf", { "STMT" });
	addDeduce("NODE", "id", { "STMT","NODE" });
	addDeduce("NODE", "int", { "STMT","NODE" });
	addDeduce("NODE", "float", { "STMT","NODE" });
	addDeduce("NODE", "break", { "STMT","NODE" });
	addDeduce("NODE", "continue", { "STMT","NODE" });
	addDeduce("NODE", "return", { "STMT","NODE" });
	addDeduce("NODE", "if", { "STMT","NODE" });
	addDeduce("NODE", "while", { "STMT","NODE" });
	addDeduce("NODE", "do", { "STMT","NODE" });
	addDeduce("NODE", "for", { "STMT","NODE" });
	addDeduce("NODE", "printf", { "STMT","NODE" });
	addDeduce("STMT", "id", { "SEQ" });
	addDeduce("STMT", "int", { "SEQ" });
	addDeduce("STMT", "float", { "SEQ" });
	addDeduce("STMT", "break", { "SEQ" });
	addDeduce("STMT", "continue", { "SEQ" });
	addDeduce("STMT", "return", { "SEQ" });
	addDeduce("STMT", "printf", { "SEQ" });
	addDeduce("STMT", "if", { "IF" });
	addDeduce("STMT", "while", { "WHILE" });
	addDeduce("STMT", "do", { "DO" });
	addDeduce("STMT", "for", { "FOR" });
	addDeduce("IF", "if", { "if", "EXPR", "BLOCK", "ELSE" });
	addDeduce("ELSE", "else", { "else","BLOCK" });
	addDeduce("WHILE", "while", { "while", "EXPR", "BLOCK" });
	addDeduce("DO", "do", { "do", "BLOCK", "while", "EXPR", ";" });
	addDeduce("FOR", "for", { "for","(","DEF","EXPR",";","ASG","FASG_",")","BLOCK" });
	addDeduce("SEQ", "id", { "ASSIGN" });
	addDeduce("SEQ", "int", { "DEF" });
	addDeduce("SEQ", "float", { "DEF" });
	addDeduce("SEQ", "break", { "break", ";" });
	addDeduce("SEQ", "continue", { "continue", ";" });
	addDeduce("SEQ", "return", { "return", ";" });
	addDeduce("SEQ", "printf", { "PRINT" });
	addDeduce("PRINT", "printf", { "printf","(","id",")",";" });
	addDeduce("ASSIGN", "id", { "ASG",";" });
	addDeduce("ASG", "id", { "id","=","EXPR" });
	addDeduce("FASG_", ",", { ",","ASG","FASG_" });
	addDeduce("DEF", "int", { "TYPE", "id","DASG_","CDEF_",";" });
	addDeduce("DEF", "float", { "TYPE", "id","DASG_","CDEF_", ";" });
	addDeduce("TYPE", "int", { "int" });
	addDeduce("TYPE", "float", { "float" });
	addDeduce("DASG_", "=", { "=","EXPR" });
	addDeduce("CDEF_", ",", { ",","id","DASG_","CDEF_" });
	addDeduce("EXPR", "num", { "num","EXPR_" });
	addDeduce("EXPR", "id", { "id", "EXPR_" });
	addDeduce("EXPR", "(", { "(","EXPR",")","EXPR_" });
	addDeduce("EXPR_", "||", { "OP","EXPR","EXPR_" });
	addDeduce("EXPR_", "&&", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "==", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", ">=", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "<=", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", ">", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "<", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "!=", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "+", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "-", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "*", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "/", { "OP", "EXPR", "EXPR_" });
	addDeduce("EXPR_", "%", { "OP", "EXPR", "EXPR_" });
	addDeduce("OP", "||", { "||" });
	addDeduce("OP", "&&", { "&&" });
	addDeduce("OP", "==", { "==" });
	addDeduce("OP", ">=", { ">=" });
	addDeduce("OP", "<=", { "<=" });
	addDeduce("OP", ">", { ">" });
	addDeduce("OP", "<", { "<" });
	addDeduce("OP", "!=", { "!=" });
	addDeduce("OP", "+", { "+" });
	addDeduce("OP", "-", { "-" });
	addDeduce("OP", "*", { "*" });
	addDeduce("OP", "/", { "/" });
	addDeduce("OP", "%", { "%" });

	allData.push_back(SymbolTable());
	endOfFile = false;
	nextToken();
}

bool Parser::match(Token tok)
{
	if (tok.getTag() == NUM)
		return match("num");
	else if (tok.getTag() == ID)
		return match("id");
	return match(tok.getName());
}

bool Parser::match(string str)
{
	string top = tokenStack.top();

	//输出程序的句型结构及其嵌套
	//根据深度信息判断此时是否达到了某个句型结构的末端
	static int forFlag = 0;
	while (!structure.empty()&&tokenStack.size() == structure.top().second) {
		string s = structure.top().first;
		if (s == "IF")stuStatement.push_back(IF_END);
		if (s == "ELSE")stuStatement.push_back(ELSE_END);
		if (s == "WHILE")stuStatement.push_back(WHILE_END);
		if (s == "DO")stuStatement.push_back(DO_END);
		if (s == "FOR")stuStatement.push_back(FOR_END);
		structure.pop();
	}
	if (top == "IF"||top=="WHILE"||top=="DO"||top=="FOR") {
		structure.push(pair<string, int>(top, tokenStack.size() - 1));
		if (top == "IF")stuStatement.push_back(IF_BEGIN);
		if (top == "WHILE")stuStatement.push_back(WHILE_BEGIN);
		if (top == "DO")stuStatement.push_back(DO_BEGIN);
		if (top == "FOR") {
			stuStatement.push_back(FOR_INIT);
			forFlag = 1;
		}
	}
	if (top == "ELSE"&&look.getName() == "else") {
		if (structure.top().first == "IF") {
			structure.pop();
			structure.push(pair<string, int>("ELSE", tokenStack.size() - 1));
			stuStatement.push_back(IF_END);
			stuStatement.push_back(ELSE_BEGIN);
		}
	}
	if (forFlag==1) {
		if (str == ";") {
			stuStatement.push_back(FOR_BEGIN);
			stuStatement.push_back(FOR_REPEAT);
			forFlag = 2;
		}
			
	}
	else if (forFlag == 2) {
		if (str == ";")
			forFlag = 3;
	}
	else if (forFlag == 3) {
		if (top == "BLOCK") {
			stuStatement.push_back(FOR_FIRST);
			forFlag = 0;
		}
	}

	//定义语句的判断和处理
	//为了正确处理定义后立刻赋值（e. int i = 0 ;）的语句，定义变量的语句判别需放在赋值语句前面
	/*
	state 0:未读取到definition
	state 1:读取到非终止符DEF后
	state 2:读取到定义变量类型后（在这一步读取变量的id）
	state 3:读取id后
	*/
	static int stateDef = 0;
	if (stateDef == 0) {
		if (top == "DEF") {
			stuStatement.push_back(DEFINITION);
			stateDef = 1;
		}
	}
	else if (stateDef == 1) {
		if (isType(look.getName())) {
			tempDef.type = look.getName();
			stateDef = 2;
		}
		else
			stateDef = 0;
	}
	else if (stateDef == 2) {
		if (look.getTag() == ID) {
			tempDef.idName = look.getName();
			allDef.push_back(tempDef);
			typeData.insert(pair<string, string>(tempDef.idName, tempDef.type));
			stateDef = 3;
		}
	}
	else if (stateDef == 3) {
		if (look.getName() == ";") {
			tempDef.Clr();
			stateDef = 0;
		}
		else if (look.getName() == ",") {
			stuStatement.push_back(DEFINITION);
			stateDef = 2;
		}
	}

	//赋值语句的判断和处理
	/*
	state 0:未读取到assignment
	state 1:读取到id后
	state 2:读取到赋值符号 = 后（即处于读取表达式状态）
	*/
	static int stateAsg = 0;
	static int stkAsgEnd = -1;
	if (stateAsg ==0) {
		if (look.getTag() == ID) {
			tempAsg.idName = look.getName();
			tempAsg.type = getType(tempAsg.idName);
			stateAsg = 1;
		}
	}
	else if (stateAsg == 1) {
		if (look.getName() == "=") {
			stuStatement.push_back(ASSIGNMENT);//读取=后已可以确定此为赋值语句
			stkAsgEnd = tokenStack.size() - 2;
			stateAsg = 2;
		}
		else if (look.getTag() == ID)
			stateAsg = 1;
		else
			stateAsg = 0;
	}
	else if (stateAsg == 2) {
		if(stkAsgEnd==tokenStack.size()){
			if (tempAsg.type == "")//如果type值为空，可以判定这是一个定义后立刻赋值（e. int i = 0;）的语句
				tempAsg.type = tempDef.type;
			tempAsg.expression = tempExpr;
			allAssign.push_back(tempAsg);
			tempExpr.clear();
			tempAsg.Clr();
			stateAsg = 0;
		}
		else if (!isUpperCase(top[0]))
			if (look.getName() != "=")//注意不要把 = 读进表达式
				tempExpr.push_back(look);
	}

	//break语句，continue语句，return语句的判断和处理
	if (top == "break")
		stuStatement.push_back(BREAK);
	if (top == "continue")
		stuStatement.push_back(CONTINUE);
	if (top == "return")
		stuStatement.push_back(RETURN);
	

	//表达式的判断和处理
	static int stateExpr = 0;
	if (stateExpr == 0) {
		if (top == "IF" || top == "WHILE") {
			stateExpr = 1;
		}
	}
	else if (stateExpr == 1) {
		if (look.getName() == "if" || look.getName() == "while") {
			stateExpr = 2;
		}
		else
			stateExpr = 0;
	}
	else if (stateExpr == 2) {
		if (!isUpperCase(top[0])) {
			tempExpr.push_back(look);
			if (look.getName() == ")") {
				allExpr.push_back(tempExpr);
				tempExpr.clear();
				stateExpr = 0;
			}
		}
	}
	//for循环的循环条件读取
	static int stateForExpr = 0;
	if (stateForExpr == 0) {
		if (top == "FOR") {
			stateForExpr = 1;
		}
	}
	else if (stateForExpr == 1) {
		if (top == ";") {
			stateForExpr = 2;
		}
	}
	else if (stateForExpr == 2) {
		if (top == ";") {
			allExpr.push_back(tempExpr);
			tempExpr.clear();
			stateForExpr = 0;
		}
		else if (!isUpperCase(top[0]))
			tempExpr.push_back(look);
	}

	//printf语句的判断和处理
	/*
	state 0:未读取printf语句
	state 1:读取非终止符PRINT后
	state 2:读取Token printf后
	state 3:读取 ( 后 （读取id）
	*/
	static int statePrint = 0;
	if (statePrint == 0) {
		if (top == "PRINT") {
			stuStatement.push_back(PRINT_VAR);
			statePrint = 1;
		}
	}
	else if (statePrint == 1) {
		if (look.getName() == "printf") {
			statePrint = 2;
		}
		else
			statePrint = 0;
	}
	else if (statePrint == 2) {
		if (top == "(") {
			statePrint = 3;
		}
		else
			statePrint = 0;
	}
	else if (statePrint == 3) {
		if (top == "id") 
			allPrint.push_back(look.getName());
		statePrint = 0;
	}

	//LL(1)文法分析
	//非终止符推导
	if (isUpperCase(top[0])) {
		//从parsingTable查找应选取的推导
		map<pair<string, string>, vector<string>>::iterator it;
		it = parsingTable.find(pair<string, string>(top, str));
		if (it == parsingTable.end()) {
			if (isNullable(top)) {
				//推导VT-->ε
				tokenStack.pop();
			}
			else
				return false;
		}
		else {
			//执行推导
			tokenStack.pop();
			for (int i = it->second.size() - 1; i >= 0; --i)
				tokenStack.push(it->second[i]);
		}
		return true;
	}
	//终止符匹配
	if (top == str) {
		tokenStack.pop();
		if (!nextToken())
			endOfFile = true;
		return true;
	}
	else
		return false;

}

bool Parser::nextToken()
{
	if (scanner.hasNext()) {
		look = scanner.getNextToken();
		return true;
	}
	else
		return false;
}

int Parser::analyse()
{
	//程序分析函数
	//返回值：
	//0 ：程序正确
	//1 ：程序读取到非法字符（词法分析错误）
	//2 ：程序语法分析错误

	while (1) {
		if (endOfFile) {
			if (tokenStack.empty())
				return 0;
			else
				return 2;
		}
		if (look.getTag() == INVALID)
			return 1;
		if (!match(look))
			return 2;
	}
}

void Parser::compile()
{
	analyse();
	process();
	execute();
}

void Parser::printVar(string idName)
{
	cout << getValue(idName).getName() << endl;
}
