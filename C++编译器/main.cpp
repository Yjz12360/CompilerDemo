//C++������
//����̨����
//���룺һ�δ������ı��ļ��еĴ���
//����������������н��
//ʵ�ֵĹ��ܣ���ֵ��䣬if���ͣ�if-else���ͣ�while���ͣ�for���͵�

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