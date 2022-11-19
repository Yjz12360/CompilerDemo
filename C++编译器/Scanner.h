#pragma once
//�ʷ�������
#include<fstream>
#include<map>
#include<vector>
using namespace std;
#include"Token.h"
#include"GeneralFunction.h"



class Scanner {
private:
	//ch���ڱ��浱ǰ��ȡ�����ַ�
	char ch;
	
	//buffer������ʱ���浱ǰ��ȡ��Token������
	string buffer;

	//���ļ��ж�ȡԴ����
	FILE* pFile;
public:
	Scanner();

	//��ȡһ���ַ�
	void readCh();

	//�ж�Դ�����ļ��Ƿ���ȫ����ȡ���
	bool hasNext();

	//�ʷ������õ���һ��Token
	Token getNextToken();
	void readNum();
	void readWord();
	void readSymbol();

	~Scanner();
};