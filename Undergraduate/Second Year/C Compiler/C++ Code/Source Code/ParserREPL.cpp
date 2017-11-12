#include "stdafx.h"
#include <math.h>
#include <vector>
#include <iostream>
#include "ParserREPL.h"
#include "Token.h"
#include "Lexer.h"
#include "ASTProgramNode.h"
#include "ASTVariableDeclNode.h"
#include "ASTAssignmentNode.h"
#include "ASTWriteNode.h"
#include "ASTReturnNode.h"
#include "ASTFunctionDeclNode.h"
#include "ASTIfNode.h"
#include "ASTWhileNode.h"
#include "ASTBlock.h"
#include "ASTIdentifierNode.h"
#include "ASTTypeNode.h"
#include "ASTExprNode.h"
#include "ASTRelationalOp.h"
#include "ASTFormalParam.h"
#include "ASTFormalParams.h"
#include "ASTSimpleExpression.h"
#include "ASTAdditiveOp.h"
#include "ASTTermNode.h"
#include "ASTFactorNode.h"
#include "ASTMultiplicativeOp.h"
#include "ASTUnaryNode.h"
#include "ASTSubExprNode.h"
#include "ASTFunctionCall.h"
#include "ASTActualParams.h"
#include "ASTBoolLiteral.h"
#include "ASTStringLiteral.h"
#include "ASTIntegerLiteral.h"
#include "ASTRealLiteral.h"

using namespace std;

MyCompiler::FrontEnd::Lexer lex2;
MyCompiler::FrontEnd::Token tok2;

//function to perform parsing of given script
MyCompiler::BackEnd::ASTStatementNode* MyCompiler::BackEnd::ParserREPL::parse(string input)
{
	input += ';';
	//empty vector of tokens
	lex2.tokenVector.empty();

	//call Lexer function to tokenise script
	lex2.getTokens(input);

	//declare and initialise program nodoe function to act as the root of the program in the syntax tree
	MyCompiler::BackEnd::ASTStatementNode* root = new MyCompiler::BackEnd::ASTStatementNode();

	try
	{
		//consume first token
		tok2 = lex2.lookNextToken();
		//parse statement
		parseStatementNode(root);

		if (lex2.tokenVector.size() != 1)
		{
			throw exception("Invalid Statement");
		}
	}
	//catch parsing error
	catch (exception e)
	{
		lex2.tokenVector.empty();
		cout << e.what() << endl;
		return nullptr;
	}
	
	//consumer EOF token
	tok2 = lex2.getNextToken();

	//on completion of parsing, return the root node to be used later on
	return root;
}

//function to parse statement node
void MyCompiler::BackEnd::ParserREPL::parseStatementNode(ASTStatementNode* st)
{
	//store copy of the current token
	string curr = tok2.getName();

	//handle variable declaration statement
	if (curr.compare("var") == 0)
	{
		tok2 = lex2.getNextToken();
		//create new variable declaration node, add it to statement child nodes and parse it
		MyCompiler::BackEnd::ASTVariableDeclNode* v1 = new MyCompiler::BackEnd::ASTVariableDeclNode();
		st->addChild(v1);
		parseVariableNode(v1);

		//if the following token is not a ';' then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of variable declaration");
		}
	}
	//handle assignment declaration
	else if (curr.compare("set") == 0)
	{
		tok2 = lex2.getNextToken();
		//creat new assignment node, add it to the statement child nodes and parse it
		MyCompiler::BackEnd::ASTAssignmentNode* v1 = new MyCompiler::BackEnd::ASTAssignmentNode();
		st->addChild(v1);
		parseAssignmentNode(v1);

		//if the following token is not a ';', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of assignment statement");
		}
	}
	//handle write statement
	else if (curr.compare("write") == 0)
	{
		tok2 = lex2.getNextToken();
		//create write statement node, add it to child nodes of statement and parse it
		MyCompiler::BackEnd::ASTWriteNode* v1 = new MyCompiler::BackEnd::ASTWriteNode();
		st->addChild(v1);
		parseWriteNode(v1);

		//if next token is not a ';', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of write statement");
		}
	}
	//handle expression
	else if (tok2.getType() == MyCompiler::FrontEnd::TOK_AdditiveOp || tok2.getType() == MyCompiler::FrontEnd::TOK_BooleanLiteral || tok2.getType() == MyCompiler::FrontEnd::TOK_Digit || tok2.getType() == MyCompiler::FrontEnd::TOK_StringLiteral || tok2.getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//create expression node, add it to the statement children and parse it
		MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
		st->addChild(e1);
		parseExpressionNode(e1);

		//if the following token is not a ';' then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of variable declaration");
		}
	}
	//invalid statement syntax
	else
	{
		throw exception("Invalid start to statement");
	}
}

//function to parse variable declarations
void MyCompiler::BackEnd::ParserREPL::parseVariableNode(ASTVariableDeclNode* vd)
{
	//if first node in the variable declaration is not an identifier, then return an error
	if ((tok2 = lex2.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Identifier)
	{
		throw exception("Invalid Variable Declaration");
	}
	else
	{
		//create identifier node, add it to variable node as child and parse it
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok2.getName();
		vd->addChild(i1);

		//if the next token is not a ':', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(":") != 0)
		{
			throw exception("Invalid Variable Declaration");
		}
		else
		{
			//if the next token is not a type specifier, then return an error
			if ((tok2 = lex2.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Type)
			{
				throw exception("Invalid Variable Declaration");
			}
			else
			{
				//create type specifier node, add it as child of variable node and set its valye to the value of the token
				MyCompiler::BackEnd::ASTTypeNode* t1 = new MyCompiler::BackEnd::ASTTypeNode();
				t1->stringVal = tok2.getName();
				vd->addChild(t1);

				//if the next token is not a '=', then return an error
				if ((tok2 = lex2.getNextToken()).getName().compare("=") != 0)
				{
					throw exception("Invalid Variable Declaration");
				}
				else
				{
					//create an expression node, add it as a child of the variable node and parse it
					MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
					vd->addChild(e1);
					parseExpressionNode(e1);
				}
			}
		}
	}
}

//function to parse assignment nodes
void MyCompiler::BackEnd::ParserREPL::parseAssignmentNode(ASTAssignmentNode* a1)
{
	//if the first token is not an identifier, then return an error
	if ((tok2 = lex2.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Identifier)
	{
		throw exception("Invalid Assignment Statement");
	}
	else
	{
		//create a new identifier node, add it as child of assignment node and set its value to that of the token
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok2.getName();
		a1->addChild(i1);

		//if the next token is not '=', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare("=") != 0)
		{
			throw exception("Invalid Assignment Statement");
		}
		else
		{
			//create a new expression node, add it as a child of the assignment node and parse it
			MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
			a1->addChild(e1);
			parseExpressionNode(e1);
		}
	}
}

//function to parse write nodes
void MyCompiler::BackEnd::ParserREPL::parseWriteNode(ASTWriteNode* w1)
{
	//create a new expression node, add it as a child of the write node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	w1->addChild(e1);
	parseExpressionNode(e1);
}

//function to parse an expression node
void MyCompiler::BackEnd::ParserREPL::parseExpressionNode(ASTExprNode* e1)
{
	//create a new simple expression node, add it as a child of the expression node and parse it
	MyCompiler::BackEnd::ASTSimpleExpression* s1 = new MyCompiler::BackEnd::ASTSimpleExpression();
	e1->addChild(s1);
	parseSimpleExpression(s1);

	//while the next node is a relational operator, loop
	while ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_RelationalOp)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//create a new relationalOp node, add it as a child of the expression node and set its value to the value of the token
		MyCompiler::BackEnd::ASTRelationalOp* r1 = new MyCompiler::BackEnd::ASTRelationalOp();
		r1->stringVal = tok2.getName();
		e1->addChild(r1);

		//handle another simple expression following the realtionalOp node
		MyCompiler::BackEnd::ASTSimpleExpression* e2 = new MyCompiler::BackEnd::ASTSimpleExpression();
		e1->addChild(e2);
		parseSimpleExpression(e2);
	}
}

//function to parse formal parameters
void MyCompiler::BackEnd::ParserREPL::parseFormalParams(ASTFormalParams* f1)
{
	//create a new formal parameter node, add it as a child of the formal parameters node and parse it
	MyCompiler::BackEnd::ASTFormalParam* fp1 = new MyCompiler::BackEnd::ASTFormalParam();
	f1->addChild(fp1);
	parseFormalParam(fp1);

	//accept more formal parameter nodes while the following node is a ','
	while ((tok2 = lex2.lookNextToken()).getName().compare(",") == 0)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//handle another formal parameter node
		MyCompiler::BackEnd::ASTFormalParam* f2 = new MyCompiler::BackEnd::ASTFormalParam();
		f1->addChild(f2);
		parseFormalParam(f2);
	}
}

//function to parse formal parameter nodes
void MyCompiler::BackEnd::ParserREPL::parseFormalParam(ASTFormalParam* f1)
{
	//if the next token is not an identifier, then return an error
	if ((tok2 = lex2.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//create a nenw identifier node, add it as a child of the formal parameter and assign its value to the token value
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok2.getName();
		f1->addChild(i1);

		//if the next token is not a ':', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare(":") == 0)
		{
			//if the next token is not a type specifier, then return an error
			if ((tok2 = lex2.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Type)
			{
				//create a new type node, add it as a child of the formal parameter node and assign its value to the token value
				MyCompiler::BackEnd::ASTTypeNode* t1 = new MyCompiler::BackEnd::ASTTypeNode();
				t1->stringVal = tok2.getName();
				f1->addChild(t1);
			}
			else
			{
				throw exception("Invalid Formal Parameter");
			}
		}
		else
		{
			throw exception("Invalid Formal Parameter");
		}
	}
	else
	{
		throw exception("Invalid Formal Parameter");
	}
}

//function to parse a simple expression 
void MyCompiler::BackEnd::ParserREPL::parseSimpleExpression(ASTSimpleExpression* s1)
{
	//create a new term node, add it as a child of the simple expression and parse it
	MyCompiler::BackEnd::ASTTermNode* t1 = new MyCompiler::BackEnd::ASTTermNode();
	s1->addChild(t1);
	parseTermNode(t1);

	//while the next token is an additive operator, then accept another simple expression
	while ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_AdditiveOp)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//create a new additive op, add it as a child of the simple expression and set its value to the value of the token
		MyCompiler::BackEnd::ASTAdditiveOp* a1 = new MyCompiler::BackEnd::ASTAdditiveOp();
		a1->stringVal = tok2.getName();
		s1->addChild(a1);

		//hanlde another simple expression node
		MyCompiler::BackEnd::ASTTermNode* s2 = new MyCompiler::BackEnd::ASTTermNode();
		s1->addChild(s2);
		parseTermNode(s2);
	}
}

//function to parse a term node
void MyCompiler::BackEnd::ParserREPL::parseTermNode(ASTTermNode* t1)
{
	//create a new factor node, add it as a child of the term node and parse it
	MyCompiler::BackEnd::ASTFactorNode* f1 = new MyCompiler::BackEnd::ASTFactorNode();
	t1->addChild(f1);
	parseFactorNode(f1);

	//while the next node is a multiplicative operator, accept another factor
	while ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_MultiplicativeOp)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//create a new multiplicative operator, add it as a child of the term node, and set its value to the token value
		MyCompiler::BackEnd::ASTMultiplicativeOp* m1 = new MyCompiler::BackEnd::ASTMultiplicativeOp();
		m1->stringVal = tok2.getName();
		t1->addChild(m1);

		//handle another factor node
		MyCompiler::BackEnd::ASTFactorNode* t2 = new MyCompiler::BackEnd::ASTFactorNode();
		t1->addChild(t2);
		parseFactorNode(t2);
	}
}

//function to parse a factor
void MyCompiler::BackEnd::ParserREPL::parseFactorNode(ASTFactorNode* f1)
{
	//consume the next token
	tok2 = lex2.lookNextToken();

	//if the next token is a '+', '-', or a 'not', then enter
	if (tok2.getName().compare("+") == 0 || tok2.getName().compare("-") == 0 || tok2.getName().compare("not") == 0)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//create a new unary node, add it as a child of the factor node, set its value to the token value and parse it
		MyCompiler::BackEnd::ASTUnaryNode* u1 = new MyCompiler::BackEnd::ASTUnaryNode();
		u1->stringValue = tok2.getName();
		f1->addChild(u1);
		parseUnaryNode(u1);
	}
	//if th next token is a nested expression, then enter
	else if (tok2.getName().compare("(") == 0)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//create a new sub expression, add it as a child of the factor node and parse it
		MyCompiler::BackEnd::ASTSubExprNode* s1 = new MyCompiler::BackEnd::ASTSubExprNode();
		f1->addChild(s1);
		parseSubExpressionNode(s1);
	}
	//if the next token is a literal, then enter
	else if (tok2.getType() == MyCompiler::FrontEnd::TOK_StringLiteral || tok2.getType() == MyCompiler::FrontEnd::TOK_BooleanLiteral || tok2.getType() == MyCompiler::FrontEnd::TOK_Digit)
	{
		//create a new literal node, add it as a child of the factor and parse it
		MyCompiler::BackEnd::ASTLiteralNode* l1 = new MyCompiler::BackEnd::ASTLiteralNode();
		f1->addChild(l1);
		parseLiteralNode(l1);
	}
	//if the next token is an identifier, then enter
	else if (tok2.getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//if the token is a function call, then enter
		if (lex2.tokenVector.at(1).getName().compare("(") == 0)
		{
			//create a new function call node, add it as a child of the factor node and parse it
			MyCompiler::BackEnd::ASTFunctionCall* fc1 = new MyCompiler::BackEnd::ASTFunctionCall();
			f1->addChild(fc1);
			parseFunctionCall(fc1);
		}
		//if the node is a variable identifier, then enter
		else
		{
			//consume the next token
			tok2 = lex2.getNextToken();

			//create a new identifier node, add it as a child of the factor node and set its value to the value of the token
			MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
			i1->stringVal = tok2.getName();
			f1->addChild(i1);
		}
	}
	else
	{
		throw exception("Invalid Factor Statement");
	}

}

//function to parse a unary node
void MyCompiler::BackEnd::ParserREPL::parseUnaryNode(ASTUnaryNode* u1)
{
	//create a new expression node, add it as a child of the unary node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	u1->addChild(e1);
	parseExpressionNode(e1);
}

//function to parse a sub expression
void MyCompiler::BackEnd::ParserREPL::parseSubExpressionNode(ASTSubExprNode* s1)
{
	//create a new expression node, add it as a child of the sub expression node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	s1->addChild(e1);
	parseExpressionNode(e1);

	//if the next token is not a ')', then return an error
	if ((tok2 = lex2.getNextToken()).getName().compare(")") != 0)
	{
		throw exception("Invalid SubExpression statement");
	}
}

//function to parse a function call
void MyCompiler::BackEnd::ParserREPL::parseFunctionCall(ASTFunctionCall* f1)
{
	//if the next token is not an identifier, then return an error
	if ((tok2 = lex2.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//create a new identifier node, add it as a child of the function call and set its value to the token value
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok2.getName();
		f1->addChild(i1);

		//if the next string is not a '(', then return an error
		if ((tok2 = lex2.getNextToken()).getName().compare("(") == 0)
		{
			//if the next token is a ')', then the function call has no parameters
			if ((tok2 = lex2.lookNextToken()).getName().compare(")") == 0)
			{
				//consume the next token
				tok2 = lex2.getNextToken();
			}
			//handle function call parameters
			else
			{
				//create new actual parameters node, add it as a child of the function call and parse it
				MyCompiler::BackEnd::ASTActualParams* a1 = new MyCompiler::BackEnd::ASTActualParams();
				f1->addChild(a1);
				parseActualParams(a1);
			}
		}
		else
		{
			throw exception("Invalid Function Call");
		}
	}
	else
	{
		throw exception("Invalid Function Call");
	}
}

//function to parse actual parameters
void MyCompiler::BackEnd::ParserREPL::parseActualParams(ASTActualParams* a1)
{
	//create new expression node, add it as a child of the actual parameters and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	a1->addChild(e1);
	parseExpressionNode(e1);

	//while the next token is a ',', accept another parameter
	while ((tok2 = lex2.lookNextToken()).getName().compare(",") == 0)
	{
		//consume the next token
		tok2 = lex2.getNextToken();

		//handle another parameter
		MyCompiler::BackEnd::ASTExprNode* a2 = new MyCompiler::BackEnd::ASTExprNode();
		a1->addChild(a2);
		parseExpressionNode(a2);
	}
	//consume the next token
	tok2 = lex2.getNextToken();
}

//function to parse a literal node
void MyCompiler::BackEnd::ParserREPL::parseLiteralNode(ASTLiteralNode* l1)
{
	//if the next node is a boolean literal, enter
	if ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_BooleanLiteral)
	{
		//create a new boolean node
		MyCompiler::BackEnd::ASTBoolLiteral* b1 = new MyCompiler::BackEnd::ASTBoolLiteral();

		//set the node value to the token value
		if (tok2.getName().compare("true") == 0)
		{
			b1->boolVal = true;
		}
		else if (tok2.getName().compare("false") == 0)
		{
			b1->boolVal = false;
		}

		//add the node as a child of the literal node
		l1->addChild(b1);

		//consume the next token
		tok2 = lex2.getNextToken();
	}
	//if the next node is a string literal
	else if ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_StringLiteral)
	{
		//create a new string literal node, add it as a child of the literal node and set its value to the token value
		MyCompiler::BackEnd::ASTStringLiteral* s1 = new MyCompiler::BackEnd::ASTStringLiteral();
		s1->stringVal = tok2.getName();
		l1->addChild(s1);
		//consume the next token
		tok2 = lex2.getNextToken();
	}
	//if the next token is a digit, then enter
	else if ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_Digit)
	{
		int s1 = 0, temp = 0;
		float s2 = 0.0;
		bool test = true;
		bool test2 = true;
		int count2 = 0;

		//while the following token is a digit or a '.', loop
		while ((tok2 = lex2.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_Digit || ((tok2 = lex2.lookNextToken()).getName().compare(".") == 0))
		{
			//consume the next token
			tok2 = lex2.getNextToken();

			//if the next token is a '.', then start storing decimal value of real literal
			if (tok2.getName().compare(".") == 0)
			{
				test = false;
				//skip current cycle
				continue;
			}

			//if a period has not yet been encountered, enter
			if (test)
			{
				//move previous digits one place to the left and add the new digit and the numeric in the number
				s1 *= 10;
				s1 += tok2.getVal();
			}
			//if a period has already been encountered, enter
			else
			{
				if (test2)
				{
					if (tok2.getVal() == 0)
					{
						count2++;
					}
					else
					{
						test2 = false;
						s2 *= 10;
						s2 += tok2.getVal();
					}
				}
				else
				{
					s2 *= 10;
					s2 += tok2.getVal();
				}
			}
		}

		//bring s2 to decimal value 
		while (s2 >= 1)
		{
			s2 /= 10;
		}

		//compensate for any 0s read beforre first non-zero decimal value
		for (int i = count2; i > 0; i--)
		{
			s2 /= 10;
		}


		//if a '.' was read, enter
		if (test == false)
		{
			//combine s1 and s2 to form real literal
			float ans = s1 + s2;

			//creat real literal node, add as child of literal node and store value
			MyCompiler::BackEnd::ASTRealLiteral* r1 = new MyCompiler::BackEnd::ASTRealLiteral();
			r1->numberVal = ans;
			l1->addChild(r1);
		}
		//period was not read
		else
		{
			//create new integer node, add it as a child of the literal node and stor value
			MyCompiler::BackEnd::ASTIntegerLiteral* i1 = new MyCompiler::BackEnd::ASTIntegerLiteral();
			i1->numberVal = s1;
			l1->addChild(i1);
		}
	}
	//inavlid literal
	else
	{
		throw exception("Invalid Literal Statement");
	}
}