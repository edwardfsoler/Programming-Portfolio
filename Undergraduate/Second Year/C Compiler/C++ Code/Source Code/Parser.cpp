#include "stdafx.h"
#include <math.h>
#include <vector>
#include <iostream>
#include "Parser.h"
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

MyCompiler::FrontEnd::Lexer lex;
MyCompiler::FrontEnd::Token tok;

//function to perform parsing of given script
MyCompiler::BackEnd::ASTProgramNode* MyCompiler::BackEnd::Parser::parse(string filename)
{
	//read text data from the given file into a string variable
	string text = lex.readFromFile(filename);

	if (text.compare("FAILED") != 0)
	{
		//call Lexer function to tokenise script
		lex.getTokens(text);
	}
	else
	{
		return NULL;
	}

	//declare and initialise program nodoe function to act as the root of the program in the syntax tree
	MyCompiler::BackEnd::ASTProgramNode* root = new MyCompiler::BackEnd::ASTProgramNode();
	
	//accept statements until the EOF pointer is found
	while ((tok = lex.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_EOF)
	{
		try
		{
			//create new statement node to be added as a new child to the root node
			MyCompiler::BackEnd::ASTStatementNode * s1 = new MyCompiler::BackEnd::ASTStatementNode();
			//assign new statement node to be child of root node
			root->addChild(s1);
			//parse statement node
			parseStatementNode(s1);
		}
		//catch parsing error
		catch (exception e)
		{
			cout << e.what() << endl;
			std::cout << "Compilation stopped" << std::endl;
			break;
		}
	}
	//on completion of parsing, return the root node to be used later on
	return root;
}

//function to parse program node
void MyCompiler::BackEnd::Parser::parseProgramNode(ASTNode* root)
{
	//create new statement node
	MyCompiler::BackEnd::ASTStatementNode * s1 = new MyCompiler::BackEnd::ASTStatementNode();
	//assign statement to root node as child
	root->addChild(s1);
	//parse statement node
	parseStatementNode(s1);
}

//function to parse statement node
void MyCompiler::BackEnd::Parser::parseStatementNode(ASTStatementNode* st)
{
	//store copy of the current token
	string curr = tok.getName();

	//handle variable declaration statement
	if (curr.compare("var") == 0)
	{
		//create new variable declaration node, add it to statement child nodes and parse it
		MyCompiler::BackEnd::ASTVariableDeclNode* v1 = new MyCompiler::BackEnd::ASTVariableDeclNode();
		st->addChild(v1);
		parseVariableNode(v1);

		//if the following token is not a ';' then return an error
		if ((tok = lex.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of variable declaration");
		}
	}
	//handle assignment declaration
	else if (curr.compare("set") == 0)
	{
		//creat new assignment node, add it to the statement child nodes and parse it
		MyCompiler::BackEnd::ASTAssignmentNode* v1 = new MyCompiler::BackEnd::ASTAssignmentNode();
		st->addChild(v1);
		parseAssignmentNode(v1);

		//if the following token is not a ';', then return an error
		if ((tok = lex.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of assignment statement");
		}
	}
	//handle write statement
	else if (curr.compare("write") == 0)
	{
		//create write statement node, add it to child nodes of statement and parse it
		MyCompiler::BackEnd::ASTWriteNode* v1 = new MyCompiler::BackEnd::ASTWriteNode();
		st->addChild(v1);
		parseWriteNode(v1);

		//if next token is not a ';', then return an error
		if ((tok = lex.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of write statement");
		}
	}
	//handle return statement
	else if (curr.compare("return") == 0)
	{
		//create return node, add it to the child nodes of statement and parse it
		MyCompiler::BackEnd::ASTReturnNode* v1 = new MyCompiler::BackEnd::ASTReturnNode();
		st->addChild(v1);
		parseReturnNode(v1);

		//if following token is not a ';', then return an error
		if ((tok = lex.getNextToken()).getName().compare(";") != 0)
		{
			throw exception("Missing ';' at end of return statement");
		}
	}
	//handle function definition statement
	else  if (curr.compare("def") == 0)
	{
		//create function definition node, add it to the statement child nodes and parse it
		MyCompiler::BackEnd::ASTFunctionDeclNode* v1 = new MyCompiler::BackEnd::ASTFunctionDeclNode();
		st->addChild(v1);
		parseFunctionDeclNode(v1);
	}
	//handle if statement
	else if (curr.compare("if") == 0)
	{
		//create if statement node, add it to the statement children and parse it
		MyCompiler::BackEnd::ASTIfNode* v1 = new MyCompiler::BackEnd::ASTIfNode();
		st->addChild(v1);
		parseIfNode(v1);
	}
	//handle while statement
	else  if (curr.compare("while") == 0)
	{
		//create while node, add it to the statement children and parse it
		MyCompiler::BackEnd::ASTWhileNode* v1 = new MyCompiler::BackEnd::ASTWhileNode();
		st->addChild(v1);
		parseWhileNode(v1);
	}
	//handle block statement
	else if (curr.compare("{") == 0)
	{
		//create block node, add it to the statement children and parse it
		MyCompiler::BackEnd::ASTBlock* v1 = new MyCompiler::BackEnd::ASTBlock();
		st->addChild(v1);
		parseBlockNode(v1);
	}
	//invalid statement syntax
	else
	{
		throw exception("Invalid start to statement");
	}
}

//function to parse variable declarations
void MyCompiler::BackEnd::Parser::parseVariableNode(ASTVariableDeclNode* vd)
{
	//if first node in the variable declaration is not an identifier, then return an error
	if ((tok = lex.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Identifier)
	{
		throw exception("Invalid Variable Declaration");
	}
	else
	{
		//create identifier node, add it to variable node as child and parse it
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok.getName();
		vd->addChild(i1);

		//if the next token is not a ':', then return an error
		if ((tok = lex.getNextToken()).getName().compare(":") != 0)
		{
			throw exception("Invalid Variable Declaration");
		}
		else
		{
			//if the next token is not a type specifier, then return an error
			if ((tok = lex.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Type)
			{
				throw exception("Invalid Variable Declaration");
			}
			else
			{
				//create type specifier node, add it as child of variable node and set its valye to the value of the token
				MyCompiler::BackEnd::ASTTypeNode* t1 = new MyCompiler::BackEnd::ASTTypeNode();
				t1->stringVal = tok.getName();
				vd->addChild(t1);

				//if the next token is not a '=', then return an error
				if ((tok = lex.getNextToken()).getName().compare("=") != 0)
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
void MyCompiler::BackEnd::Parser::parseAssignmentNode(ASTAssignmentNode* a1)
{
	//if the first token is not an identifier, then return an error
	if ((tok = lex.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Identifier)
	{
		throw exception("Invalid Assignment Statement");
	}
	else
	{
		//create a new identifier node, add it as child of assignment node and set its value to that of the token
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok.getName();
		a1->addChild(i1);

		//if the next token is not '=', then return an error
		if ((tok = lex.getNextToken()).getName().compare("=") != 0)
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
void MyCompiler::BackEnd::Parser::parseWriteNode(ASTWriteNode* w1)
{
	//create a new expression node, add it as a child of the write node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	w1->addChild(e1);
	parseExpressionNode(e1);
}
 
//function to parse return nodes
void MyCompiler::BackEnd::Parser::parseReturnNode(ASTReturnNode* r1)
{
	//vcreate a new expression node, add it as a child of the return node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	r1->addChild(e1);
	parseExpressionNode(e1); 
}

//function to parse function declaration nodes
void MyCompiler::BackEnd::Parser::parseFunctionDeclNode(ASTFunctionDeclNode* f1)
{
	//if the first token is not an identifier, then return an error
	if ((tok = lex.getNextToken()).getType() != MyCompiler::FrontEnd::TOK_Identifier)
	{
		throw exception("Invalid Function Declaration");
	}
	else
	{
		//create a new identifier node, add it as a child of the function declaration node and parse it
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok.getName();
		f1->addChild(i1);

		//if the next node is not a '(', then return an error
		if ((tok = lex.getNextToken()).getName().compare("(") != 0)
		{
			throw exception("Invalid Function Declaration");
		}
		else
		{
			//if the next node is an identifier (parameter(s)), then create a new formal parameters node, add it as a child
			//of the function declaration node and parse it
			if ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_Identifier)
			{
				MyCompiler::BackEnd::ASTFormalParams* fp1 = new MyCompiler::BackEnd::ASTFormalParams();
				f1->addChild(fp1);
				parseFormalParams(fp1);
			}
			
			//if the next token is not a ')', then return an error
			if ((tok = lex.getNextToken()).getName().compare(")") == 0)
			{
				//if the next token is not a ':', then return an error
				if ((tok = lex.getNextToken()).getName().compare(":") == 0)
				{
					//if the next token is not a type specifier, then return an error
					if ((tok = lex.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Type)
					{
						//create a new type specifier node, add it as a child of the function declaration node
						//and set its value to that of the token
						MyCompiler::BackEnd::ASTTypeNode* tn1 = new MyCompiler::BackEnd::ASTTypeNode();
						tn1->stringVal = tok.getName();
						f1->addChild(tn1);

						//if the next token is not a '{', then return an error
						if ((tok = lex.lookNextToken()).getName().compare("{") == 0)
						{
							//create a new block node, addd it as a child of the function declaration node and parse it
							MyCompiler::BackEnd::ASTBlock* b1 = new MyCompiler::BackEnd::ASTBlock();
							f1->addChild(b1);
							//consume the next token before parsing the block statement
							tok = lex.getNextToken();
							parseBlockNode(b1);
						}
						else
						{
							throw exception("Invalid Function Declaration");
						}
					}
					else
					{
						throw exception("Invalid Function Declaration");
					}
				}
				else
				{
					throw exception("Invalid Function Declaration");
				}
			}
			else
			{
				throw exception("Invalid Function Declaration");
			}
		}
	}
}

//function to parse if nodes
void MyCompiler::BackEnd::Parser::parseIfNode(ASTIfNode* i1)
{
	//if the first token is not a '(', then return an error
	if ((tok = lex.getNextToken()).getName().compare("(") == 0)
	{
		//create a new expression node, add it as a child of the if node and parse it
		MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
		i1->addChild(e1);
		parseExpressionNode(e1);

		//if the next token is not a ')', then return an error
		if ((tok = lex.getNextToken()).getName().compare(")") == 0)
		{
			//if the next token is not a '{', then return an error
			if ((tok = lex.lookNextToken()).getName().compare("{") == 0)
			{
				//create a new block node, add it as a child of the if node and parse it
				MyCompiler::BackEnd::ASTBlock* b1 = new MyCompiler::BackEnd::ASTBlock();
				i1->addChild(b1);
				//consume a token before parsing the block node
				tok = lex.getNextToken();
				parseBlockNode(b1);

				//if the if statement also has an else statement, then parse the else statement
				if ((tok = lex.lookNextToken()).getName().compare("else") == 0)
				{
					tok = lex.getNextToken();
					//if the next token following the else token is not a '{', then return an error
					if ((tok = lex.lookNextToken()).getName().compare("{") == 0)
					{
						//create a new block node, assign it as a child to the if node and parse it
						MyCompiler::BackEnd::ASTBlock* b2 = new MyCompiler::BackEnd::ASTBlock();
						i1->addChild(b2);
						//consumer a token before parsing the block node
						tok = lex.getNextToken();
						parseBlockNode(b2);
					}
					else
					{
						throw exception("Invalid if statement");
					}
				}
			}
			else
			{
				throw exception("Invalid if statement");
			}
		}
		else
		{
			throw exception("Invalid if statement");
		}
	}
	else
	{
		throw exception("Invalid if statement");
	}
}

//function to parse a while node
void MyCompiler::BackEnd::Parser::parseWhileNode(ASTWhileNode* w1)
{
	//if the next token is not a '(', then return an error
	if ((tok = lex.getNextToken()).getName().compare("(") == 0)
	{
		//create a new expression node, add it as a child of the while node and parse it
		MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
		w1->addChild(e1);
		parseExpressionNode(e1);

		//if the next token is not a ')', then return an error
		if ((tok = lex.getNextToken()).getName().compare(")") == 0)
		{
			//if the next token is not a '{', then return an error
			if ((tok = lex.lookNextToken()).getName().compare("{") == 0)
			{
				//create a new block node, add it as a child of the while node and parse it
				MyCompiler::BackEnd::ASTBlock* b1 = new MyCompiler::BackEnd::ASTBlock();
				w1->addChild(b1);
				//consume the enxt token before parsing the block node
				tok = lex.getNextToken();
				parseBlockNode(b1);
			}
			else
			{
				throw exception("Invalid while statement");
			}
		}
		else
		{
			throw exception("Invalid while statement");
		}
	}
	else
	{
		throw exception("Invalid while statement");
	}
}

//function to parse a block node
void MyCompiler::BackEnd::Parser::parseBlockNode(ASTBlock* b1)
{
	//create and handle statement until the end of the block node is reached
	while ((tok = lex.getNextToken()).getName().compare("}") != 0)
	{
		//create a new statement node, add it as a child of the block node and parse it
		MyCompiler::BackEnd::ASTStatementNode* s1 = new MyCompiler::BackEnd::ASTStatementNode();
		b1->addChild(s1);
		parseStatementNode(s1);
	}
}

//function to parse an expression node
void MyCompiler::BackEnd::Parser::parseExpressionNode(ASTExprNode* e1)
{
	//create a new simple expression node, add it as a child of the expression node and parse it
	MyCompiler::BackEnd::ASTSimpleExpression* s1 = new MyCompiler::BackEnd::ASTSimpleExpression();
	e1->addChild(s1);
	parseSimpleExpression(s1);

	//while the next node is a relational operator, loop
	while ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_RelationalOp)
	{
		//consume the next token
		tok = lex.getNextToken();

		//create a new relationalOp node, add it as a child of the expression node and set its value to the value of the token
		MyCompiler::BackEnd::ASTRelationalOp* r1 = new MyCompiler::BackEnd::ASTRelationalOp();
		r1->stringVal = tok.getName();
		e1->addChild(r1);

		//handle another simple expression following the realtionalOp node
		MyCompiler::BackEnd::ASTSimpleExpression* e2 = new MyCompiler::BackEnd::ASTSimpleExpression();
		e1->addChild(e2);
		parseSimpleExpression(e2);
	}
}

//function to parse formal parameters
void MyCompiler::BackEnd::Parser::parseFormalParams(ASTFormalParams* f1)
{
	//create a new formal parameter node, add it as a child of the formal parameters node and parse it
	MyCompiler::BackEnd::ASTFormalParam* fp1 = new MyCompiler::BackEnd::ASTFormalParam();
	f1->addChild(fp1);
	parseFormalParam(fp1);

	//accept more formal parameter nodes while the following node is a ','
	while ((tok = lex.lookNextToken()).getName().compare(",") == 0)
	{
		//consume the next token
		tok = lex.getNextToken();

		//handle another formal parameter node
		MyCompiler::BackEnd::ASTFormalParam* f2 = new MyCompiler::BackEnd::ASTFormalParam();
		f1->addChild(f2);
		parseFormalParam(f2);
	}
}

//function to parse formal parameter nodes
void MyCompiler::BackEnd::Parser::parseFormalParam(ASTFormalParam* f1)
{
	//if the next token is not an identifier, then return an error
	if ((tok = lex.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//create a nenw identifier node, add it as a child of the formal parameter and assign its value to the token value
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok.getName();
		f1->addChild(i1);
		
		//if the next token is not a ':', then return an error
		if ((tok = lex.getNextToken()).getName().compare(":") == 0)
		{
			//if the next token is not a type specifier, then return an error
			if ((tok = lex.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Type)
			{
				//create a new type node, add it as a child of the formal parameter node and assign its value to the token value
				MyCompiler::BackEnd::ASTTypeNode* t1 = new MyCompiler::BackEnd::ASTTypeNode();
				t1->stringVal = tok.getName();
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
void MyCompiler::BackEnd::Parser::parseSimpleExpression(ASTSimpleExpression* s1)
{
	//create a new term node, add it as a child of the simple expression and parse it
	MyCompiler::BackEnd::ASTTermNode* t1 = new MyCompiler::BackEnd::ASTTermNode();
	s1->addChild(t1);
	parseTermNode(t1);

	//while the next token is an additive operator, then accept another simple expression
	while ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_AdditiveOp)
	{
		//consume the next token
		tok = lex.getNextToken();

		//create a new additive op, add it as a child of the simple expression and set its value to the value of the token
		MyCompiler::BackEnd::ASTAdditiveOp* a1 = new MyCompiler::BackEnd::ASTAdditiveOp();
		a1->stringVal = tok.getName();
		s1->addChild(a1);

		//hanlde another simple expression node
		MyCompiler::BackEnd::ASTTermNode* s2 = new MyCompiler::BackEnd::ASTTermNode();
		s1->addChild(s2);
		parseTermNode(s2);
	}
}

//function to parse a term node
void MyCompiler::BackEnd::Parser::parseTermNode(ASTTermNode* t1)
{
	//create a new factor node, add it as a child of the term node and parse it
	MyCompiler::BackEnd::ASTFactorNode* f1 = new MyCompiler::BackEnd::ASTFactorNode();
	t1->addChild(f1);
	parseFactorNode(f1);

	//while the next node is a multiplicative operator, accept another factor
	while ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_MultiplicativeOp)
	{
		//consume the next token
		tok = lex.getNextToken();

		//create a new multiplicative operator, add it as a child of the term node, and set its value to the token value
		MyCompiler::BackEnd::ASTMultiplicativeOp* m1 = new MyCompiler::BackEnd::ASTMultiplicativeOp();
		m1->stringVal = tok.getName();
		t1->addChild(m1);
		
		//handle another factor node
		MyCompiler::BackEnd::ASTFactorNode* t2 = new MyCompiler::BackEnd::ASTFactorNode();
		t1->addChild(t2);
		parseFactorNode(t2);
	}
}

//function to parse a factor
void MyCompiler::BackEnd::Parser::parseFactorNode(ASTFactorNode* f1)
{
	//consume the next token
	tok = lex.lookNextToken();

	//if the next token is a '+', '-', or a 'not', then enter
	if (tok.getName().compare("+") == 0 || tok.getName().compare("-") == 0 || tok.getName().compare("not") == 0)
	{
		//consume the next token
		tok = lex.getNextToken();

		//create a new unary node, add it as a child of the factor node, set its value to the token value and parse it
		MyCompiler::BackEnd::ASTUnaryNode* u1 = new MyCompiler::BackEnd::ASTUnaryNode();
		u1->stringValue = tok.getName();
		f1->addChild(u1);
		parseUnaryNode(u1);
	}
	//if th next token is a nested expression, then enter
	else if (tok.getName().compare("(") == 0)
	{
		//consume the next token
		tok = lex.getNextToken();

		//create a new sub expression, add it as a child of the factor node and parse it
		MyCompiler::BackEnd::ASTSubExprNode* s1 = new MyCompiler::BackEnd::ASTSubExprNode();
		f1->addChild(s1);
		parseSubExpressionNode(s1);
	}
	//if the next token is a literal, then enter
	else if (tok.getType() == MyCompiler::FrontEnd::TOK_StringLiteral || tok.getType() == MyCompiler::FrontEnd::TOK_BooleanLiteral || tok.getType() == MyCompiler::FrontEnd::TOK_Digit)
	{
		//create a new literal node, add it as a child of the factor and parse it
		MyCompiler::BackEnd::ASTLiteralNode* l1 = new MyCompiler::BackEnd::ASTLiteralNode();
		f1->addChild(l1);
		parseLiteralNode(l1);
	}
	//if the next token is an identifier, then enter
	else if (tok.getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//if the token is a function call, then enter
		if (lex.tokenVector.at(1).getName().compare("(") == 0)
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
			tok = lex.getNextToken();

			//create a new identifier node, add it as a child of the factor node and set its value to the value of the token
			MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
			i1->stringVal = tok.getName();
			f1->addChild(i1);
		}
	}
	else
	{
		throw exception("Invalid Factor Statement");
	}

}

//function to parse a unary node
void MyCompiler::BackEnd::Parser::parseUnaryNode(ASTUnaryNode* u1)
{
	//create a new expression node, add it as a child of the unary node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	u1->addChild(e1);
	parseExpressionNode(e1);
}

//function to parse a sub expression
void MyCompiler::BackEnd::Parser::parseSubExpressionNode(ASTSubExprNode* s1)
{
	//create a new expression node, add it as a child of the sub expression node and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	s1->addChild(e1);
	parseExpressionNode(e1);

	//if the next token is not a ')', then return an error
	if ((tok = lex.getNextToken()).getName().compare(")") != 0)
	{
		throw exception("Invalid SubExpression statement");
	}
}

//function to parse a function call
void MyCompiler::BackEnd::Parser::parseFunctionCall(ASTFunctionCall* f1)
{
	//if the next token is not an identifier, then return an error
	if ((tok = lex.getNextToken()).getType() == MyCompiler::FrontEnd::TOK_Identifier)
	{
		//create a new identifier node, add it as a child of the function call and set its value to the token value
		MyCompiler::BackEnd::ASTIdentifierNode* i1 = new MyCompiler::BackEnd::ASTIdentifierNode();
		i1->stringVal = tok.getName();
		f1->addChild(i1);

		//if the next string is not a '(', then return an error
		if ((tok = lex.getNextToken()).getName().compare("(") == 0)
		{
			//if the next token is a ')', then the function call has no parameters
			if ((tok = lex.lookNextToken()).getName().compare(")") == 0)
			{
				//consume the next token
				tok = lex.getNextToken();
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
void MyCompiler::BackEnd::Parser::parseActualParams(ASTActualParams* a1)
{
	//create new expression node, add it as a child of the actual parameters and parse it
	MyCompiler::BackEnd::ASTExprNode* e1 = new MyCompiler::BackEnd::ASTExprNode();
	a1->addChild(e1);
	parseExpressionNode(e1);

	//while the next token is a ',', accept another parameter
	while ((tok = lex.lookNextToken()).getName().compare(",") == 0) 
	{
		//consume the next token
		tok = lex.getNextToken();

		//handle another parameter
		MyCompiler::BackEnd::ASTExprNode* a2 = new MyCompiler::BackEnd::ASTExprNode();
		a1->addChild(a2);
		parseExpressionNode(a2);
	}
	//consume the next token
	tok = lex.getNextToken();
}

//function to parse a literal node
void MyCompiler::BackEnd::Parser::parseLiteralNode(ASTLiteralNode* l1)
{
	//if the next node is a boolean literal, enter
	if ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_BooleanLiteral)
	{
		//create a new boolean node
		MyCompiler::BackEnd::ASTBoolLiteral* b1 = new MyCompiler::BackEnd::ASTBoolLiteral();

		//set the node value to the token value
		if (tok.getName().compare("true") == 0)
		{
			b1->boolVal = true;
		}
		else if (tok.getName().compare("false") == 0)
		{
			b1->boolVal = false;
		}

		//add the node as a child of the literal node
		l1->addChild(b1);

		//consume the next token
		tok = lex.getNextToken();
	}
	//if the next node is a string literal
	else if ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_StringLiteral)
	{
		//create a new string literal node, add it as a child of the literal node and set its value to the token value
		MyCompiler::BackEnd::ASTStringLiteral* s1 = new MyCompiler::BackEnd::ASTStringLiteral();
		s1->stringVal = tok.getName();
		l1->addChild(s1);
		//consume the next token
		tok = lex.getNextToken();
	}
	//if the next token is a digit, then enter
	else if ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_Digit)
	{
		int s1 = 0, temp = 0;
		float s2 = 0.0;
		bool test = true;

		//while the following token is a digit or a '.', loop
		while ((tok = lex.lookNextToken()).getType() == MyCompiler::FrontEnd::TOK_Digit || ((tok = lex.lookNextToken()).getName().compare(".") == 0))
		{
			//consume the next token
			tok = lex.getNextToken();

			//if the next token is a '.', then start storing decimal value of real literal
			if (tok.getName().compare(".") == 0)
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
				s1 += tok.getVal();
			}
			//if a period has already been encountered, enter
			else
			{
				//same procedure as above
				s2 *= 10;
				s2 += tok.getVal();
			}
		}

		//bring s2 to decimal value 
		while (s2 >= 1)
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