#pragma once
#include"Scanner.h"
#include"Assignment.h"
#include"Definition.h"
#include"Statement.h"
#include"SymbolTable.h"
#include"ForLoop.h"
#include<stack>
#include<map>
//�﷨������

class Parser {
private:
	//�ʷ�������
	Scanner scanner;

	//look���ڱ��浱ǰ��ȡ����Token
	Token look;

	//�������϶��·���LL(1)�ķ��Ƶ�
	//stack���ڱ��浱ǰջ��Ԫ��
	//string���ַ�Ϊ��д��ĸ��ʾ����ֹ��������Ϊ��ֹ��
	stack<string>tokenStack;

	//pairǰһ��string��ʾ����ֹ������һ��string��ʾ��ǰ��ȡ����Token����ֹ����
	//vector��ʾѡȡ���Ƶ���ʽ
	map<pair<string, string>, vector<string>>parsingTable;
	void addDeduce(string VN, string VT, vector<string>deduce);

	//���ڱ�����Щ����ֹ�������Ƶ�����
	vector<string>nullableSymbol;
	bool isNullable(string str);

	//�﷨������ȡ���������ݵ���䣨e. int i;��ʱ���ô�����ʱ���������Ϣ
	Definition tempDef;
	void addData(Definition def);

	//�﷨������ȡ����ֵ��䣨e. i = 1;��ʱ���ô������������Ϣ
	Assignment tempAsg;
	bool assign(Assignment asg);

	//�������б���
	vector<SymbolTable>allData;


	//id���Ƶ��������͵�ӳ��
	//e. "i"-->"int"
	//�﷨������ʱ��ʹ��
	map<string, string>typeData;

	//����id��ȡ��ֵ
	Token getValue(string idName);

	//���������idȷ������������
	string getType(string id);

	//����EXPRESSION��Token���
	vector<Token>tempExpr;
	//���ʽ����
	//����һ�����мӼ��˳���������Ƚ���������߼�����������ŵı��ʽ
	//��������ȼ��� ���� > �˳� > �Ӽ� > �Ƚ������ > �߼������
	//�ⲿֻ��Ҫ���õ�һ��simplify�������ɣ������Ӻ���Ϊ���ڲ�����
	//���ս���õ�һ����ֵ�򲼶�ֵ��0��1����Token
	Token simplify(vector<Token>ex);
	//���ʽ������Ӻ���
	//�����ǻ���һ�����мӼ��˳����㣬�Ƚ���������߼���������������ţ��ı��ʽ
	Token simplify_Sub1(vector<Token>ex);
	//���ʽ����ĵڶ����Ӻ���
	//�����ǻ���һ��ֻ���мӼ����㣬�Ƚ���������߼�������ı��ʽ
	Token simplify_Sub2(vector<Token>ex);
	//���ʽ����ĵ������Ӻ���
	//�����ǻ���һ��ֻ���бȽ���������߼�������ı��ʽ
	Token simplify_Sub3(vector<Token>ex);
	//���ʽ����ĵ��Ĳ��Ӻ���
	//�����ǻ���һ��ֻ�����߼�������ı��ʽ
	Token simplify_Sub4(vector<Token>ex);

	//����ȫ������ı���
	vector<Definition>allVar;
	
	//ָʾ���������Ƿ��Ѷ�ȡ���
	bool endOfFile;

	//��ǰ�����Ƕ�׽ṹ
	//string��ʾ�ṹ���ͣ�e. if���ͣ�IF����while���ͣ�WHILE���ȣ�
	//int��������ṹ����ʱtokenStack����ȣ�size�������ڽṹend���ж�
	stack<pair<string,int>>structure;
	//�������Ƕ�׽ṹ������vector��
	vector<STATEMENT>stuStatement;

	//���������������еĶ������͸�ֵ���ͱ��ʽ
	vector<Definition>allDef;
	vector<Assignment>allAssign;
	vector<vector<Token>>allExpr;//ֻ������������ѭ������ִ���������ʽ����ֵ����еı��ʽ����������
	vector<string>allPrint;//string����id

	//�﷨����
	int analyse();

	//������������벻ͬ�������֮��������ӳ��
	void process();
	map<int, int>gotoTable;
	map<int, int>exprTable;
	map<int, int>defTable;
	map<int, int>asgTable;
	map<int, int>printTable;

	//��������󣬡����С������ļ�
	void execute();


public:
	Parser();
	bool match(Token tok);
	bool match(string str);
	bool nextToken();

	void compile();

	void printVar(string idName);

};