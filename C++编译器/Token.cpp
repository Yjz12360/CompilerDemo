#include "Token.h"

Token::Token()
{
}

Token::Token(TAG t, string s):tag(t),name(s)
{
}

TAG Token::getTag()
{
	return tag;
}

string Token::getName()
{
	return name;
}

void Token::display()
{
	switch (tag) {
	case ID:cout << "ID"; break;
	case NUM:cout << "NUM"; break;
	case KEY:cout << "KEY"; break;
	case SYMBOL:cout << "SYMBOL"; break;
	case INVALID:cout << "INVALID"; break;
	}
	cout << "\t" << name << endl;
}
