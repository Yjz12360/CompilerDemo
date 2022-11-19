#define _CRT_SECURE_NO_WARNINGS

#include "Scanner.h"



Scanner::Scanner() :ch(' '), buffer("")
{
	pFile = fopen("program.txt", "r");
}

void Scanner::readCh()
{
	fscanf(pFile, "%c", &ch);
}

bool Scanner::hasNext()
{
	return !feof(pFile);
}

Token Scanner::getNextToken()
{
	buffer.clear();
	if (!hasNext())
		return Token(ERROR, "");
	while (isBlank(ch))
		readCh();

	if (isCharacter(ch) || ch == '_') {
		readWord();
		if (isKeyWord(buffer))
			return Token(KEY, buffer);
		else
			return Token(ID, buffer);
	}
	else if (isDigit(ch)) {
		readNum();
		return Token(NUM, buffer);
	}
	else {
		readSymbol();
		if (buffer == "//") {
			while (ch != '\n')
				readCh();
			return getNextToken();
		}
		else if (buffer == "/*") {
			Token igTok = getNextToken();
			while (igTok.getName()!="*/") {
				if (igTok.getTag() == ERROR)return igTok;
				igTok = getNextToken();
			}
			return getNextToken();
		}
		if (isInvalidSymbol(buffer))
			return Token(INVALID, buffer);
		else
			return Token(SYMBOL, buffer);
	}


}

void Scanner::readNum()
{
	while (isDigit(ch)) {
		buffer += ch;
		readCh();
	}
	if (ch == '.') {
		do {
			buffer += ch;
			readCh();
		} while (isDigit(ch));
	}
}

void Scanner::readWord()
{
	while (isCharacter(ch) || isDigit(ch) || ch == '_') {
		buffer += ch;
		readCh();
	}
}

void Scanner::readSymbol()
{
	buffer += ch;
	if (ch == '>' || ch == '<' || ch == '='||ch=='!') {
		readCh();
		if (ch == '=') {
			buffer += ch;
			readCh();
		}
	}
	else if (ch == '|') {
		readCh();
		if (ch == '|') {
			buffer += ch;
			readCh();
		}
	}
	else if (ch == '&') {
		readCh();
		if (ch == '&') {
			buffer += ch;
			readCh();
		}
	}
	else if (ch == '/') {
		readCh();
		if (ch == '/'||ch=='*') {
			buffer += ch;
			readCh();
		}
	}
	else if (ch == '*') {
		readCh();
		if (ch == '/') {
			buffer += ch;
			readCh();
		}
	}
	else {
		readCh();
	}
}

Scanner::~Scanner()
{
	fclose(pFile);
}
