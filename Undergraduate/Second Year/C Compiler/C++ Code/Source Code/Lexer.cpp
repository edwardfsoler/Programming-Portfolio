#include "stdafx.h"
#include "Lexer.h"
#include "Token.h"
#include <vector>
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

//function to output all tokens on screen for testing purposes
void MyCompiler::FrontEnd::Lexer::tokenTester()
{
	//loop through vector outputting all token data
	for (unsigned i = 0; i < tokenVector.size(); i++)
	{
		std::cout << tokenVector[i].getType() << setw(10) << tokenVector[i].getVal() << setw(10) << tokenVector[i].getName() << endl;
	}
}

//function to get the next token from the token list held by the Lexer
MyCompiler::FrontEnd::Token MyCompiler::FrontEnd::Lexer::getNextToken()
{
	//define new temporary token to hold the next token
	MyCompiler::FrontEnd::Token temptoken = tokenVector.front();

	//delete the token at the front of the list
	tokenVector.erase(tokenVector.begin());

	//return the token which was at the front of the list
	return temptoken;
}

//function to look at the next token from the token list held by the Lexer without deleting it
MyCompiler::FrontEnd::Token MyCompiler::FrontEnd::Lexer::lookNextToken()
{
	//define new temporary token to hold the next token
	MyCompiler::FrontEnd::Token temptoken = tokenVector.front();

	//return the token which is at the front of the list
	return temptoken;
}

//function to assign strings to their appropriate tokens
void MyCompiler::FrontEnd::Lexer::stringHandler(string temp, MyCompiler::FrontEnd::Token* tok)
{
	//string values are tested and a token is filled with the appropriate data
	if (temp.compare("if") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("if");
	}
	else if (temp.compare("else") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("else");
	}
	else if (temp.compare("return") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("return");
	}
	else if (temp.compare("while") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("while");
	}
	else if (temp.compare("int") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Type);
		tok->setVal(0);
		tok->setName("int");
	}
	else if (temp.compare("real") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Type);
		tok->setVal(0);
		tok->setName("real");
	}
	else if (temp.compare("bool") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Type);
		tok->setVal(0);
		tok->setName("bool");
	}
	else if (temp.compare("string") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Type);
		tok->setVal(0);
		tok->setName("string");
	}
	else if (temp.compare("true") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_BooleanLiteral);
		tok->setVal(0);
		tok->setName("true");
	}
	else if (temp.compare("false") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_BooleanLiteral);
		tok->setVal(0);
		tok->setName("false");
	}
	else if (temp.compare("and") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_MultiplicativeOp);
		tok->setVal(0);
		tok->setName("and");
	}
	else if (temp.compare("or") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_AdditiveOp);
		tok->setVal(0);
		tok->setName("or");
	}
	else if (temp.compare("not") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName("not");
	}
	else if (temp.compare("set") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_AssignmentOp);
		tok->setVal(0);
		tok->setName("set");
	}
	else if (temp.compare("var") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("var");
	}
	else if (temp.compare("def") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("def");
	}
	else if (temp.compare("write") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Keyword);
		tok->setVal(0);
		tok->setName("write");
	}
	else
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Identifier);
		tok->setVal(0);
		tok->setName(temp);
	}
}

//function to assign characters to their appropriate tokens
void MyCompiler::FrontEnd::Lexer::charHandler(string temp, MyCompiler::FrontEnd::Token* tok)
{
	//assess each character, creating a token with the respective data
	if (temp.compare(";") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName(";");
	}
	else if (temp.compare(".") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName(".");
	}
	else if (temp.compare("(") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName("(");
	}
	else if (temp.compare(")") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName(")");
	}
	else if (temp.compare("{") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName("{");
	}
	else if (temp.compare("}") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName("}");
	}
	else if (temp.compare(",") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName(",");
	}
	else if (temp.compare(":") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_Punctuation);
		tok->setVal(0);
		tok->setName(":");
	}
	else if (temp.compare("=") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_AssignmentOp);
		tok->setVal(0);
		tok->setName("=");
	}
	else if (temp.compare("<") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName("<");
	}
	else if (temp.compare(">") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName(">");
	}
	else if (temp.compare("==") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName("==");
	}
	else if (temp.compare("!=") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName("!=");
	}
	else if (temp.compare("<=") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName("<=");
	}
	else if (temp.compare(">=") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_RelationalOp);
		tok->setVal(0);
		tok->setName(">=");
	}
	else if (temp.compare("+") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_AdditiveOp);
		tok->setVal(0);
		tok->setName("+");
	}
	else if (temp.compare("-") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_AdditiveOp);
		tok->setVal(0);
		tok->setName("-");
	}
	else if (temp.compare("*") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_MultiplicativeOp);
		tok->setVal(0);
		tok->setName("*");
	}
	else if (temp.compare("/") == 0)
	{
		tok->setType(MyCompiler::FrontEnd::TOK_MultiplicativeOp);
		tok->setVal(0);
		tok->setName("/");
	}
	//illegal expression has been input if character is not caught
	else
	{
		cout << "Error: '" + temp + "' is an illegal character" << endl;
	}
}

//function to seperate strings/characters from each other and handle them respectively
void MyCompiler::FrontEnd::Lexer::getTokens(string input) {
	//store value of size of string bearing the input
	int size = input.size();
	int test = 0;
	int tempi = 0;
	char current = 0;
	char lookahead = 0;
	std::string temp;

	//create token which shall temporarily hold data to be passed onto the vector
	MyCompiler::FrontEnd::Token tok;

	//loop through the string
	for (int i = 0; i < size - 1; i++)
	{
		//store copy of current character
		current = input.at(i);

		//if end of string hasn't been reached, then lookahead at the next character
		if (i < size - 2)
		{
			lookahead = input.at(i + 1);
		}
		else
		{
			lookahead = NULL;
		}

		//if current character is a digit, create and initialise digit token
		if (isdigit(current))
		{
			//extract integer value from character ascii value
			tempi = current - '0';
			tok.setType(MyCompiler::FrontEnd::TOK_Digit);
			tok.setVal(tempi);
			tok.setName("");

		}
		//if current character is a space, then skip it
		else if (isspace(current))
		{
			continue;
		}
		//if current character and next character are forward slashes '//', then skip the rest of the list as we have a comment
		else if (current == 47 && lookahead == 47)
		{
			while (input.at(i++) != '\n');

			i--;
			continue;
		}
		//if current character is a forward slash and next character is a star '/*', then skip characters until the next occurence of '*/' is found
		else if (current == 47 && lookahead == 42)
		{
			i += 2;
			while (!((input.at(i) == 42) && (input.at(i + 1) == 47)))
			{
				i++;
			}
			i++;

			continue;
		}
		//if current character is open quotes '"', then store the rest of the characters until the next quote marks '"'
		//and store them as a string literal
		else if (current == 34)
		{
			i++;
			temp = "";
			while (input.at(i) != '\"')
			{
				//only allow printable characters to be part of the string literal
				if (input.at(i) >= 32 && input.at(i) <= 126)
				{
					temp += input.at(i);
					i += 1;
				}
				else
				{
					cout << "Error: '" << input.at(i) << "' is not a legal expression" << endl;
				}
			}

			tok.setType(MyCompiler::FrontEnd::TOK_StringLiteral);
			tok.setVal(0);
			tok.setName(temp);
		}
		//if the current character is an alphabetic character or a underscore '_', then group all characters of such type and pass
		//them on to the string handler
		else if (isalpha(current) || current == 95)
		{
			temp = "";
			
			while (isalpha(input.at(i)) || isdigit(input.at(i)) || input.at(i) == 95) 
			{
				temp += input.at(i);
				i++;
			}

			i--;
			stringHandler(temp, &tok);
		}
		//if character has not yet been caught, then it is either a single character token or an illegal character
		else
		{
			temp = current;

			//catch cases of double characters such as '==', '<=', '>=' and '!='
			if ((current == lookahead) && (current == '='))
			{
				temp += lookahead;
				i++;
			}
			else if ((current == '!') && (lookahead == '='))
			{
				temp += lookahead;
				i++;
			}
			else if ((current == '<') && (lookahead == '='))
			{
				temp += lookahead;
				i++;
			}
			else if ((current == '>') && (lookahead == '='))
			{
				temp += lookahead;
				i++;
			}

			//pass character(s) on to the char handler
			charHandler(temp, &tok);
		}

		//push newly formed token onto the back of the vector
		this->tokenVector.push_back(tok);
	}

	//once all of the characters have been tokenised, create an End Of File token and push it onto the vector
	tok.setType(MyCompiler::FrontEnd::TOK_EOF);
	tok.setVal(0);
	tok.setName("$");

	this->tokenVector.push_back(tok);
}

//function to read text from file into a string variable
std::string MyCompiler::FrontEnd::Lexer::readFromFile(string filename) {
	string line;
	string text;
	fstream myfile;

	//open file of given filename
	myfile.open(filename, ios::in);

	//error handling if file is not found
	if (!myfile.is_open())
	{
		cout << "Unable to open file " << filename << endl;
		return "FAILED";
	}

	//read the text line by iine until the end of the file has been reached
	while (getline(myfile, line))
	{
		text += line + "\n";
	}

	//close the file stream and return the string
	myfile.close();
	return text;
}