#include "stdafx.h"
#include "Token.h"
#include <set>
#include <iostream>

using namespace std;

//function to set the token name
void MyCompiler::FrontEnd::Token::setName(std::string n)
{
	this->name = n;
}

//function to get the token name
std::string MyCompiler::FrontEnd::Token::getName()
{
	return this->name;
}
 
//function to set the token value
void MyCompiler::FrontEnd::Token::setVal(int val)
{
	this->value = val;
}

//function to get the token value
int MyCompiler::FrontEnd::Token::getVal()
{
	return this->value;
}

//function to set the token type
void MyCompiler::FrontEnd::Token::setType(MyCompiler::FrontEnd::TOK_TYPE t)
{
	if (t <= 11)
	{
		this->type = t;
	}
	else
	{
		cout << "Error:: invalid type for terminal token" << endl;
	}
}

//function to get the token type
MyCompiler::FrontEnd::TOK_TYPE MyCompiler::FrontEnd::Token::getType()
{
	return this->type;
}
