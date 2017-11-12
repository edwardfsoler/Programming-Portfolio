#include "stdafx.h"
#include "Interpreter.h"
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
#include <exception>

//function to visit program nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTProgramNode* p)
{
	//create a new scope
	st.createScope();

	//visit all child nodes
	for (int i = 0; i < p->children.size(); i++)
	{
		try
		{
			p->children.at(i)->accept(this);
		}
		catch(std::exception e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Compilation stopped" << std::endl;
			break;
		}
	}

	//delete the tope most scope
	st.deleteScope();
}

//function to visit block nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTBlock* b)
{
	//create a new scope
	st.createScope();

	//visit all child nodes
	for (int i = 0; i < b->children.size(); i++)
	{
		b->children.at(i)->accept(this);
	}

	//delet current scope
	st.deleteScope();
}

//function to visit statment nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTStatementNode* s)
{
	//visit child node
	s->children.at(0)->accept(this);
}

//function to visit while nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTWhileNode* w)
{
	//casting of first child to expression node
	MyCompiler::BackEnd::ASTExprNode* e = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(w->children.at(0));
	//visit first child
	w->children.at(0)->accept(this);

	//while expression is evaluated to true, run block of while loop
	while (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::b && e->exprVal.b == true)
	{
		//visit block node
		w->children.at(1)->accept(this);
		
		//retest while expression
		w->children.at(0)->accept(this);
	}
}

//function to visit if nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTIfNode* in)
{
	//casting of first child to expression node
	MyCompiler::BackEnd::ASTExprNode* e = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(in->children.at(0));
	//visit first child
	in->children.at(0)->accept(this);

	//if the expression of the if node evaluates to true, enter first block
	if(e->exprVal.t == MyCompiler::BackEnd::SymbolTable::b && e->exprVal.b == true)
	{
		in->children.at(1)->accept(this);
	}
	//otherwise, enter else block if present
	else if (in->children.size() == 3)
	{
		in->children.at(2)->accept(this);
	}
}

//function to visit retsurn nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTReturnNode* r)
{
	//visit child node
	r->children.at(0)->accept(this);
}

//function to visit write nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTWriteNode* w)
{
	//visit first child
	w->children.at(0)->accept(this);
	//cast first child to expression node
	MyCompiler::BackEnd::ASTExprNode* e = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(w->children.at(0));

	//access value of respective type in varible, and output on screen
	if (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::b)
	{
		if (e->exprVal.b == 1)
		{
			std::cout << "true" << std::endl;
		}
		else
		{
			std::cout << "false" << std::endl;
		}
	}
	else if (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::i)
	{
		std::cout << e->exprVal.i << std::endl;
	}
	else if (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::r)
	{
		std::cout << e->exprVal.f << std::endl;
	}
	else if (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::s)
	{
		std::cout << e->exprVal.s << std::endl;
	}
}

//function to visit function declaration nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTFunctionDeclNode* f)
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

//function to visit variable declaration nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTVariableDeclNode* v)
{
	//cast first child of variable declaration to identifier node
	MyCompiler::BackEnd::ASTIdentifierNode* name = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(v->children.at(0));

	//cast second child of variable declaration to type node
	MyCompiler::BackEnd::ASTTypeNode* type = dynamic_cast<MyCompiler::BackEnd::ASTTypeNode*>(v->children.at(1));

	//create new variable val
	MyCompiler::BackEnd::SymbolTable::varval* v2 = new MyCompiler::BackEnd::SymbolTable::varval();

	//set variable type for varval v2
	if (type->stringVal.compare("real") == 0)
	{
		v2->t = MyCompiler::BackEnd::SymbolTable::r;
	}
	else if (type->stringVal.compare("int") == 0)
	{
		v2->t = MyCompiler::BackEnd::SymbolTable::i;
	}
	else if (type->stringVal.compare("string") == 0)
	{
		v2->t = MyCompiler::BackEnd::SymbolTable::s;
	}
	else if (type->stringVal.compare("bool") == 0)
	{
		v2->t = MyCompiler::BackEnd::SymbolTable::b;
	}

	//create new stentry variable and set it to type variable and assign the varval to the type of the variable declaration
	MyCompiler::BackEnd::SymbolTable::stentry* st2 = new MyCompiler::BackEnd::SymbolTable::stentry();
	st2->func = false;
	st2->vv = *v2;

	//visit the expression node
	v->children.at(2)->accept(this);

	//if the variable and expression type do not match, return an error
	if (curtype != st2->vv.t)
	{
		std::cout << "Semantic Error: Expression of type " << curtype << " cannot be assignend to variable '" << name->stringVal << "'" << std::endl;
		throw std::exception("Type Error");
	}
	else
	{
		//assign respective value to respective variable, by type
		if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
		{
			st2->vv.b = tempValue.b;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
		{
			st2->vv.i = tempValue.i;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
		{
			st2->vv.f = tempValue.f;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::s)
		{
			st2->vv.s = tempValue.s;
		}

		//insert entry into symbol table
		st.insertInScope(name->stringVal, *st2);
	}
}

//function to visit assignment nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTAssignmentNode* a)
{
	//cast first child of assignment to identifier
	MyCompiler::BackEnd::ASTIdentifierNode* i = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(a->children.at(0));

	//cast secpnd child of assignment to expression
	MyCompiler::BackEnd::ASTExprNode* e = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(a->children.at(1));

	//test to see whether variable has been defined within current scope
	if (st.inScope(i->stringVal))
	{
		//visit expression node to retrieve type
		a->children.at(1)->accept(this);

		//test whether expression matches variable type
		if (!st.isType(curtype, i->stringVal))
		{
			std::cout << "Semantic error: Variable '" << i->stringVal << "' cannot be assigned a value of type " << curtype << std::endl;
			throw std::exception("Type Error");
		}
		else
		{
			//store iterator to point to the variable node entry in the symbol table
			std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator temp = st.getEntry(i->stringVal);

			//assign respective value to respective variable, by type
			if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
			{
				temp->second.vv.b = tempValue.b;
			}
			else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
			{
				temp->second.vv.i = tempValue.i;
			}
			else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
			{
				temp->second.vv.f = tempValue.f;
			}
			else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::s)
			{
				temp->second.vv.s = tempValue.s;
			}
		}
	}
	//variable not in symbol table
	else
	{
		std::cout << "Semantic error: Variable '" << i->stringVal << "' not in scope" << std::endl;
		throw std::exception("Type Error");
	}
}

//function to visit relational operator nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTRelationalOp* r)
{
	//assign value respective of relational operator type
	if (r->stringVal.compare("<") == 0)
	{
		tempValue.i = 1;
	}
	else if (r->stringVal.compare(">") == 0)
	{
		tempValue.i = 2;
	}
	else if (r->stringVal.compare("==") == 0)
	{
		tempValue.i = 3;
	}
	else if (r->stringVal.compare("!=") == 0)
	{
		tempValue.i = 4;
	}
	else if (r->stringVal.compare("<=") == 0)
	{
		tempValue.i = 5;
	}
	else if (r->stringVal.compare(">=") == 0)
	{
		tempValue.i = 6;
	}
}

//function to visit expression nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTExprNode* e)
{
	//store size of children nodes
	int max = e->children.size();

	//loop through children nodes
	for (int i = 0; i < max; i += 2)
	{
		//on first cycle, visit child node
		if (i == 0)
		{
			e->children.at(i)->accept(this);
		}

		//store copy of child type and value
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		e->exprVal = tempValue;
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another simple expression is in this expression
		if (i % 2 == 0 && max > i + 2)
		{
			//visit relationalOperator
			e->children.at(i + 1)->accept(this);
			tempOp = tempValue;

			//visit next simple expression
			e->children.at(i + 2)->accept(this);

			//if expressions on either side of the operator are not integers or real, and are not the same, then error
			if (curtype != temptype || curtype == MyCompiler::BackEnd::SymbolTable::s)
			{
				std::cout << "Simple Expressions do not match in type" << std::endl;
			}
			else
			{
				//handle result of operation according to relational operator
				switch (tempOp.i)
				{
				case 1:
					// <

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempValue.i > tempval.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempValue.f > tempval.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 2:
					// >
					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempval.i > tempValue.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempval.f > tempValue.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 3:
					// == 

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempValue.i == tempval.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempValue.f == tempval.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 4:
					// !=

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempValue.i != tempval.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempValue.f != tempval.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
					}

					break;
				case 5:
					// <=

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempValue.i >= tempval.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempValue.f >= tempval.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 6:
					// >=

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempValue.i <= tempval.i)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempValue.f <= tempval.f)
						{
							tempValue.b = true;
							e->exprVal.b = true;
						}
						else
						{
							tempValue.b = false;
							e->exprVal.b = false;
						}
					}
					else
					{
						std::cout << "Semantic Error: Invalid type for relational operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				}
				//set outcome of relational operation to be that of boolean
				tempValue.t = MyCompiler::BackEnd::SymbolTable::b;
				e->exprVal.t = tempValue.t;
				curtype = MyCompiler::BackEnd::SymbolTable::b;
			}
		}
	}
}

//function to visit addtive operator nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTAdditiveOp* a)
{
	//assign respective value according to operator type
	if (a->stringVal.compare("+") == 0)
	{
		tempValue.i = 1;
	}
	else if (a->stringVal.compare("-") == 0)
	{
		tempValue.i = 2;
	}
	else if (a->stringVal.compare("or") == 0)
	{
		tempValue.i = 3;
	}
}

//function to visit simple expression nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTSimpleExpression* se)
{
	//store size of child nodes
	int max = se->children.size();

	//loop through children 
	for (int i = 0; i < max; i += 2)
	{
		//on first cycle, visit child node
		if (i == 0)
		{
			se->children.at(i)->accept(this);
		}

		//store copy of child node value and type
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		se->exprVal = tempValue;
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another term is in this expression
		if (i % 2 == 0 && max > i + 2)
		{
			//visit aditive op
			se->children.at(i + 1)->accept(this);
			tempOp = tempValue;

			//visit next child node
			se->children.at(i + 2)->accept(this);

			//if the two terms are not equal or are of type string or boolean, then error
			if (curtype != temptype || curtype == MyCompiler::BackEnd::SymbolTable::s)
			{
				std::cout << "Terms do not match in type" << std::endl;
			}
			else
			{
				//set outcome of relational operation to be that of boolean
				tempValue.t = curtype;

				//handle result of operation according to relational operator
				switch (tempOp.i)
				{
				case 1:
					// +

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						tempValue.i = tempValue.i + tempval.i;
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						tempValue.f = tempValue.f + tempval.f;
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 2:
					// -
					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						tempValue.i = tempval.i - tempValue.i;
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						tempValue.f = tempval.f - tempValue.f;
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 3:
					// or 

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
					{
						if (tempValue.b == true || tempval.b == true)
						{
							tempValue.b = true;
						}
						else
						{
							tempValue.b = false;
						}
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				}
				//store copy of expression result
				se->exprVal = tempValue;
			}
		}
	}
}

//function to visit multiplicative operator nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTMultiplicativeOp* a)
{
	//assign respective value according to operator type
	if (a->stringVal.compare("*") == 0)
	{
		tempValue.i = 1;
	}
	else if (a->stringVal.compare("/") == 0)
	{
		tempValue.i = 2;
	}
	else if (a->stringVal.compare("and") == 0)
	{
		tempValue.i = 3;
	}
}

//function to visit term nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTTermNode* t)
{
	//store size of children
	int max = t->children.size();

	//loop through children
	for (int i = 0; i < max; i += 2)
	{
		//on first cycle, visit child node
		if (i == 0)
		{
			t->children.at(i)->accept(this);
		}

		//store copy of child type and value
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		t->exprVal = tempValue;
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another simple expression is in this expression
		if (i % 2 == 0 && max > i + 2)
		{
			//visit multiplicative oeprator node
			t->children.at(i + 1)->accept(this);
			tempOp = tempValue;

			//visit next child node
			t->children.at(i + 2)->accept(this);

			//if the two operands are not of type int or real and are not the same type, then error
			if (curtype != temptype || curtype == MyCompiler::BackEnd::SymbolTable::s)
			{
				std::cout << "Factors do not match in type" << std::endl;
				throw std::exception("Type Error");
			}
			else
			{
				//set outcome of relational operation to be that of boolean
				tempValue.t = curtype;

				//handle result of operation according to relational operator
				switch (tempOp.i)
				{
				case 1:
					// *

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						tempValue.i = tempValue.i * tempval.i;
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						tempValue.f = tempValue.f * tempval.f;
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}


					break;
				case 2:
					// /
					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
					{
						if (tempval.i == 0)
						{
							std::cout << "Error: Attempt to divide by 0" << std::endl;
							throw std::exception("Divide by 0 error");
						}
						else
						{
							tempValue.i = tempval.i / tempValue.i;
						}
					}
					else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
					{
						if (tempval.f == 0.0)
						{
							std::cout << "Error: Attempt to divide by 0" << std::endl;
							throw std::exception("Divide by 0 error");
						}
						else
						{
							tempValue.f = tempval.f / tempValue.f;
						}
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}

					break;
				case 3:
					// and 

					if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
					{
						if (tempValue.b == true && tempval.b == true)
						{
							tempValue.b = true;
						}
						else
						{
							tempValue.b = false;
						}
					}
					else
					{
						std::cout << "Error: Invalid type for this operation" << std::endl;
						throw std::exception("Type Error");
					}
					break;
				}
				//store copy of expression result
				t->exprVal = tempValue;
			}
		}
	}
}

//function to visit factor nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTFactorNode* f)
{
	//visit child node
	f->children.at(0)->accept(this);

	if (returnType != tempValue.t)
	{
		std::cout << "Inconsistent return type" << std::endl;
		throw std::exception("Type Error");
	}
}

//function to visit unary node
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTUnaryNode* u)
{
	//visit first child
	u->children.at(0)->accept(this);

	//handle respective operator type
	if (u->stringValue.compare("+") == 0)
	{
		//if operator is not a real or integer literal, then return an error
		if (tempValue.t != MyCompiler::BackEnd::SymbolTable::i && tempValue.t != MyCompiler::BackEnd::SymbolTable::r)
		{
			std::cout << "Error: Invalid type for unary operation" << std::endl;
			throw std::exception("Type Error");
		}
	}
	else if (u->stringValue.compare("-") == 0)
	{
		//perform respective operation of literal values
		if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
		{
			tempValue.i *= -1;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
		{
			tempValue.f *= -1;
		}
		else
		{
			std::cout << "Error: Invalid type for unary operation" << std::endl;
			throw std::exception("Type Error");
		}
	}
	else if (u->stringValue.compare("not") == 0)
	{
		//perform respective oeprator on boolean literal
		if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
		{
			if (tempValue.b == true)
			{
				tempValue.b = false;
			}
			else
			{
				tempValue.b = true;
			}
		}
		else
		{
			std::cout << "Error: Invalid type for unary operation" << std::endl;
			throw std::exception("Type Error");
		}
	}
}

//function to visit sub expression nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTSubExprNode* s)
{
	//visit child node
	s->children.at(0)->accept(this);
}

//function to visit function call nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTFunctionCall* f)
{
	//cast first child to identifier node
	MyCompiler::BackEnd::ASTIdentifierNode* l = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(f->children.at(0));

	//if function exists in scope, then enter, otherwise return an error
	if (!st.inScope(l->stringVal))
	{
		std::cout << "Semantic Error: Call to function '" << l->stringVal << "' yet no function defined within this scope" << std::endl;
		throw std::exception("Function Call Error");
	}
	else
	{
		//store iterator to point to literal entry in symbol table
		std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator fp = st.getEntry(l->stringVal);

		//if function call contains actual parameters, enter
		if (f->children.size() == 2)
		{
			//create vector to store types and values of actual parameters
			std::vector<MyCompiler::BackEnd::SymbolTable::ptype> paramTypes;
			std::vector<MyCompiler::BackEnd::SymbolTable::varval> paramValues;

			//loop through actual parameters
			for (int i = 0; i < f->children.at(1)->children.size(); i++)
			{
				//find type of each parameter and store in the vector
				MyCompiler::BackEnd::ASTExprNode* param = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(f->children.at(1)->children.at(i));
				param->accept(this);

				//store copy of parameter type and value
				paramTypes.push_back(curtype);
				paramValues.push_back(tempValue);
			}

			//if the actual parameters and the formal parameters do not match, then return an error
			if (!st.isTypeParameters(paramTypes, l->stringVal))
			{
				std::cout << "Semantic Error: Types of actual and formal parameters for function '" << l->stringVal << "' do not match" << std::endl;
				throw std::exception("Type Error");
			}
			else
			{
				std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator ito;
				//assign parameter values to formal parameters
				for (int i = 0; i < paramValues.size(); i++)
				{
					//create new temporary symbol table entry
					MyCompiler::BackEnd::SymbolTable::stentry* newParam = new MyCompiler::BackEnd::SymbolTable::stentry;
					//set its type to non-function
					newParam->func = false;

					//handle type of current actual parameter, and store copy of the respective value
					if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::b)
					{
						fp->second.fv.params.at(i).b = paramValues.at(i).b;
						newParam->vv.t = MyCompiler::BackEnd::SymbolTable::b;
						newParam->vv.b = paramValues.at(i).b;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::i)
					{
						fp->second.fv.params.at(i).i = paramValues.at(i).i;
						newParam->vv.t = MyCompiler::BackEnd::SymbolTable::i;
						newParam->vv.i = paramValues.at(i).i;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::r)
					{
						fp->second.fv.params.at(i).f = paramValues.at(i).f;
						newParam->vv.t = MyCompiler::BackEnd::SymbolTable::r;
						newParam->vv.f = paramValues.at(i).f;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::s)
					{
						fp->second.fv.params.at(i).s = paramValues.at(i).s;
						newParam->vv.t = MyCompiler::BackEnd::SymbolTable::s;
						newParam->vv.s = paramValues.at(i).s;
					}
					//if function has previously been called, just update variable values in symbol table
					ito = st.scopeStack.top().find(fp->second.fv.paramNames.at(i));
					if (ito != st.scopeStack.top().end())
					{
						ito->second = *newParam;
					}
					//otherwise insert new entry
					else
					{
						st.insertInScope(fp->second.fv.paramNames.at(i), *newParam);
					}
				}
			}
		}
		//execute body of function 
		fp->second.fv.body->accept(this);
		//return type of function for later use in previous recursive level
		curtype = st.getType(l->stringVal);
	}
}

//function to visit identifier nodes
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTIdentifierNode* i)
{
	//if current node is not in the symbol table, then return error
	if (!st.inScope(i->stringVal))
	{
		std::cout << "Identifier '" << i->stringVal << "' is not in this scope" << std::endl;
		throw std::exception("Out of scope error");
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

//function to visit literal node
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTLiteralNode* l)
{
	//visit child node
	l->children.at(0)->accept(this);
}

//function to visit boolean literal
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTBoolLiteral* b)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::b;
	tempValue.t = curtype;
	tempValue.b = b->boolVal;
}

//function to visit integer literal
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTIntegerLiteral* i)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::i;
	tempValue.t = curtype;
	tempValue.i = i->numberVal;
}

//function to cisit real literal
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTRealLiteral* r)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::r;
	tempValue.t = curtype;
	tempValue.f = r->numberVal;
}

//function to visit string literal
void MyCompiler::BackEnd::Interpreter::visit(MyCompiler::BackEnd::ASTStringLiteral* s)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::s;
	tempValue.t = curtype;
	tempValue.s = s->stringVal;
}