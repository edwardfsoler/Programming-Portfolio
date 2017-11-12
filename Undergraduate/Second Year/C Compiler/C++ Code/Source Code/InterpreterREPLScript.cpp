#include "stdafx.h"
#include "InterpreterREPLScript.h"
#include "ASTActualParams.h"
#include "ASTAdditiveOp.h"
#include "ASTAssignmentNode.h"
#include "ASTBoolLiteral.h"
#include "ASTExprNode.h"
#include "ASTFactorNode.h"
#include "ASTFormalParam.h"
#include "ASTFormalParams.h"
#include "ASTFunctionCall.h"
#include "ASTFunctionDeclNode.h"
#include "ASTIdentifierNode.h"
#include "ASTIfNode.h"
#include "ASTIntegerLiteral.h"
#include "ASTLiteralNode.h"
#include "ASTMultiplicativeOp.h"
#include "ASTProgramNode.h"
#include "ASTRealLiteral.h"
#include "ASTRelationalOp.h"
#include "ASTReturnNode.h"
#include "ASTSimpleExpression.h"
#include "ASTStatementNode.h"
#include "ASTStringLiteral.h"
#include "ASTSubExprNode.h"
#include "ASTTermNode.h"
#include "ASTTypeNode.h"
#include "ASTUnaryNode.h"
#include "ASTVariableDeclNode.h"
#include "ASTWhileNode.h"
#include "ASTWriteNode.h"
#include "ASTBlock.h"
#include <string>
#include <iostream>

//function to visit program nodes
void MyCompiler::BackEnd::InterpreterREPLScript::visit(MyCompiler::BackEnd::ASTProgramNode* p)
{
	if (p->children.size() > 1)
	{
		std::cout << "Loaded scripts may only hold one function definition" << std::endl;
	}
	else
	{
		p->children.at(0)->accept(this);
	}
}

//function to visit statement nodes
void MyCompiler::BackEnd::InterpreterREPLScript::visit(MyCompiler::BackEnd::ASTStatementNode* p)
{
	if (p->children.size() > 1)
	{
		std::cout << "Loaded scripts may only hold one function definition" << std::endl;
	}
	else
	{
		p->children.at(0)->accept(this);
	}
}

//function to visit function declaration nodes
void MyCompiler::BackEnd::InterpreterREPLScript::visit(MyCompiler::BackEnd::ASTFunctionDeclNode* f)
{
	//create new symbol table entry variable
	MyCompiler::BackEnd::SymbolTable::stentry entry;

	//set variable type to function
	entry.func = true;

	//cast third child of function declaration to ASTTypeNode so as to access the stringVal
	MyCompiler::BackEnd::ASTTypeNode* ta = dynamic_cast<MyCompiler::BackEnd::ASTTypeNode*>(f->children.at(f->children.size() - 2));

	//store copy of stringVal for function return type
	std::string temp = ta->stringVal;

	//test value of function return type, and appropriately set return type of entry
	if (temp.compare("real") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::r;
	}
	else if (temp.compare("int") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::i;
	}
	else if (temp.compare("string") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::s;
	}
	else if (temp.compare("bool") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::b;
	}

	//if function declaration contains formal parameters, enter
	if (f->children.size() == 4)
	{
		//cast second child of function declaration (parameters)
		MyCompiler::BackEnd::ASTFormalParams* ta2 = dynamic_cast<MyCompiler::BackEnd::ASTFormalParams*>(f->children.at(1));

		//loop through the formal parameters of the function
		for (int i = 0; i < ta2->children.size(); i++)
		{
			//cast child of formal parameters to formal parameter
			MyCompiler::BackEnd::ASTFormalParam* ta3 = dynamic_cast<MyCompiler::BackEnd::ASTFormalParam*>(ta2->children.at(i));

			//cast first child of each formal parameter to identifier node
			MyCompiler::BackEnd::ASTIdentifierNode* ident = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(ta3->children.at(0));

			//cast second child of each formal parameter to type node
			MyCompiler::BackEnd::ASTTypeNode* identity = dynamic_cast<MyCompiler::BackEnd::ASTTypeNode*>(ta3->children.at(1));

			//create new variable val for each parameter
			MyCompiler::BackEnd::SymbolTable::varval* v = new MyCompiler::BackEnd::SymbolTable::varval();

			//set variable type for varval v
			if (identity->stringVal.compare("real") == 0)
			{
				v->t = MyCompiler::BackEnd::SymbolTable::r;
			}
			else if (identity->stringVal.compare("int") == 0)
			{
				v->t = MyCompiler::BackEnd::SymbolTable::i;
			}
			else if (identity->stringVal.compare("string") == 0)
			{
				v->t = MyCompiler::BackEnd::SymbolTable::s;
			}
			else if (identity->stringVal.compare("bool") == 0)
			{
				v->t = MyCompiler::BackEnd::SymbolTable::b;
			}

			//push varval for current parameter onto parameter vector
			entry.fv.params.push_back(*v);
			//push parameter name for current parameter onto vector
			entry.fv.paramNames.push_back(ident->stringVal);
		}
	}

	//cast first child of cuntion declaration to type identifier
	MyCompiler::BackEnd::ASTIdentifierNode* fname = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(f->children.at(0));

	//store body of function to be run on function call
	MyCompiler::BackEnd::ASTBlock* body = dynamic_cast<MyCompiler::BackEnd::ASTBlock*>(f->children.at(f->children.size() - 1));
	entry.fv.body = body;

	//insert function declaration into scope by returning name of function, and entry variable, which holds the return type and the parameter types
	st.insertInScope(fname->stringVal, entry);
}

//function to visit identifier nodes
void MyCompiler::BackEnd::InterpreterREPLScript::visit(MyCompiler::BackEnd::ASTIdentifierNode* i)
{
	//if current node is not in the symbol table, then return error
	if (!st.inScope(i->stringVal))
	{
		std::cout << "Identifier '" << i->stringVal << "' is not in this scope" << std::endl;
	}
	else
	{
		//store iterator to point to literal entry in symbol table
		std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator tempE = st.getEntry(i->stringVal);

		//store copy of entry type and value
		curtype = st.getType(i->stringVal);
		tempValue.t = curtype;

		//store copy of literal value according to type
		if (curtype == MyCompiler::BackEnd::SymbolTable::b)
		{
			tempValue.b = tempE->second.vv.b;
		}
		else if (curtype == MyCompiler::BackEnd::SymbolTable::i)
		{
			tempValue.i = tempE->second.vv.i;
		}
		else if (curtype == MyCompiler::BackEnd::SymbolTable::r)
		{
			tempValue.f = tempE->second.vv.f;
		}
		else if (curtype == MyCompiler::BackEnd::SymbolTable::s)
		{
			tempValue.s = tempE->second.vv.s;
		}
	}
}