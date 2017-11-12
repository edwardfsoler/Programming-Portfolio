#include "stdafx.h"
#include "SemanticAnalyserVisitor.h"
#include "SymbolTable.h"
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
#include <exception>
#include <iostream>

//function to visit program nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTProgramNode* p)
{
	//create a new scope
	st.createScope();
	//visit all child nodes of the program node
	for (int i = 0; i < p->children.size(); i++)
	{
		try
		{
			p->children.at(i)->accept(this);
		}
		//catch parsing error
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Compilation stopped" << std::endl;
			break;
		}
	}
	//delete the top most scope 
	st.deleteScope();
}

void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTBlock* b)
{
	//create a new scope
	st.createScope();
	//visit all child ndoes of the block node
	for (int i = 0; i < b->children.size(); i++)
	{
		b->children.at(i)->accept(this);
	}
	//delete the top most scope
	st.deleteScope();
}

//function to visit statement nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTStatementNode* s)
{
	//visit child node
	s->children.at(0)->accept(this);
}

//function to visit while nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTWhileNode* w)
{
	//visit child nodes
	for (int i = 0; i < w->children.size(); i++)
	{
		w->children.at(i)->accept(this);
	}
}

//function to visit if nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTIfNode* in)
{
	//cast first child of visit node to expression node  type
	MyCompiler::BackEnd::ASTExprNode* e = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(in->children.at(0));
	//visit first child of node
	in->children.at(0)->accept(this);

	//if the expression within the if node evaluates to true, enter the first block
	if (e->exprVal.t == MyCompiler::BackEnd::SymbolTable::b && e->exprVal.b == true)
	{
		in->children.at(1)->accept(this);
	}
	//if the expression evaluates to false and an else block exists, enter the else block
	else if (in->children.size() == 3)
	{
		in->children.at(2)->accept(this);
	}
}

//function to visit return nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTReturnNode* r)
{
	//visit child node	
	r->children.at(0)->accept(this);
	if (returnType != tempValue.t)
	{
		std::cout << "Inconsistent return type" << std::endl;
		throw std::exception("Type Error");
	}
}

//function to visit write nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTWriteNode* w)
{	
	//visit child node
	w->children.at(0)->accept(this);
}

//function to visit function declaration nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTFunctionDeclNode* f)
{
	//create new symbol table entry variable
	MyCompiler::BackEnd::SymbolTable::stentry entry;
	
	//set variable type to function
	entry.func = true;

	//cast third child of function declaration to ASTTypeNode so as to access the stringVal
	MyCompiler::BackEnd::ASTTypeNode* ta = dynamic_cast<MyCompiler::BackEnd::ASTTypeNode*>(f->children.at(f->children.size()-2));
	
	//store copy of stringVal for function return type
	std::string temp = ta->stringVal;

	//test value of function return type, and appropriately set return type of entry
	if (temp.compare("real") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::r;
		returnType = MyCompiler::BackEnd::SymbolTable::r;
	}
	else if (temp.compare("int") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::i;
		returnType = MyCompiler::BackEnd::SymbolTable::i;
	}
	else if (temp.compare("string") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::s;
		returnType = MyCompiler::BackEnd::SymbolTable::s;
	}
	else if (temp.compare("bool") == 0)
	{
		entry.fv.t.t = MyCompiler::BackEnd::SymbolTable::b;
		returnType = MyCompiler::BackEnd::SymbolTable::b;
	}

	//if the function declaration contains formal parameters, then handle them
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
			
			//store current parameter in a vector
			entry.fv.params.push_back(*v);
			//store current parameter name in a vector
			entry.fv.paramNames.push_back(ident->stringVal);
		}
	}

	//cast first child of function declaration to type identifier
	MyCompiler::BackEnd::ASTIdentifierNode* fname = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(f->children.at(0));

	//cast body pointer
	MyCompiler::BackEnd::ASTBlock* body = dynamic_cast<MyCompiler::BackEnd::ASTBlock*>(f->children.at(f->children.size() - 1));
	
	//store body pointer to be called by a function call
	entry.fv.body = body;

	//insert function declaration into scope by returning name of function, and entry variable, which holds the return type and the parameter types
	st.insertInScope(fname->stringVal, entry);
}

//function to visit variable declaration nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTVariableDeclNode* v)
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

	//visit the expression being assign to the current variable
	v->children.at(2)->accept(this);

	//if the expression type and the variable type do not match, return an error
	if (curtype != st2->vv.t)
	{
		std::cout << "Semantic Error: Expression of type " << curtype << " cannot be assignend to variable '" << name->stringVal << "'" << std::endl;
		throw std::exception("Type Error");
	}
	else
	{
		//assign variable the correct type and value
		if (tempValue.t == MyCompiler::BackEnd::SymbolTable::b)
		{
			st2->vv.b = tempValue.b;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::i)
		{
			st2->vv.i= tempValue.i;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
		{
			st2->vv.f = tempValue.f;
		}
		else if (tempValue.t == MyCompiler::BackEnd::SymbolTable::s)
		{
			st2->vv.s = tempValue.s;
		}

		//insert variable into symbol table
		st.insertInScope(name->stringVal, *st2);
	}
}

//function to visit assignment nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTAssignmentNode* a)
{
	//cast first child of assignment to identifier
	MyCompiler::BackEnd::ASTIdentifierNode* i = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(a->children.at(0));

	//cast second child of assignment to expression
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
		}
		else
		{
			//store iterator to access variable within the symbol table
			std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator temp = st.getEntry(i->stringVal);

			//assign the variable in the symbol table the new value
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
	else
	{
		std::cout << "Semantic error: Variable '" << i->stringVal << "' not in scope" << std::endl;
		throw std::exception("Out of Scope Error");
	}
}

//function to visit relational operator nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTRelationalOp* r)
{
	//assign value according to operator type for later testing
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
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTExprNode* e)
{
	//store the current number of child ndoes
	int max = e->children.size();

	//loop through children of expression node
	for (int i = 0; i < max; i+=2)
	{
		//on first iteration, visit the child node
		if (i == 0)
		{
			e->children.at(i)->accept(this);
		}

		//store copy of node type
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		//store copy of node value
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		//store copy of node value for later access
		e->exprVal = tempValue;
		//declare new variable to store copy of relational operator type
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another simple expression is in this expression
		if (i%2 == 0 && max > i + 2)
		{
			//visit the relational operator
			e->children.at(i + 1)->accept(this);
			//store copy of relational operator type
			tempOp = tempValue;

			//visit next simple expression
			e->children.at(i+2)->accept(this);
			
			//if expressions on either side of the operator are not integers or real, and are not the same, then error
			if (curtype != temptype || curtype == MyCompiler::BackEnd::SymbolTable::s)
			{
				std::cout << "Simple Expressions do not match in type" << std::endl;
				throw std::exception("Type Error");
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
						std::cout << "Semantic Error: Invalid type for relational operation" <<  std::endl;
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
						throw std::exception("Type Error");
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
				//store new copy of expression value
				e->exprVal.t = tempValue.t;
				//store copy of expression type
				curtype = MyCompiler::BackEnd::SymbolTable::b;
			}
		}
	}
}


//function to visit additive operator nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTAdditiveOp* a)
{
	//store value for identification of operator
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
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTSimpleExpression* se)
{
	//store number of children of simple expression node
	int max = se->children.size();

	//loop through each child node
	for (int i = 0; i < max; i += 2)
	{
		//on first cycle, visit the child node
		if (i == 0)
		{
			se->children.at(i)->accept(this);
		}

		//store copy of child node type
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		//store copy of child node value
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		//store another copy of the child node value
		se->exprVal = tempValue;
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another term is in this expression, enter
		if (i % 2 == 0 && max > i + 2)
		{
			//visit the additive operator
			se->children.at(i + 1)->accept(this);
			//store copy of additive operator type
			tempOp = tempValue;

			//visit next term node
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
					else if(tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
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
				//update final result of expression
				se->exprVal = tempValue;
			}
		}
	}
}

//function to visit multiplicative operator nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTMultiplicativeOp* a)
{
	//store value for identification of operator
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

//function to visit term node
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTTermNode* t)
{
	//store copy of number of children nodes
	int max = t->children.size();
	
	//loop through child nodes
	for (int i = 0; i < max; i += 2)
	{
		//on first cycle, visit child node
		if (i == 0)
		{
			t->children.at(i)->accept(this);
		}
		
		//store cope of child node type
		MyCompiler::BackEnd::SymbolTable::ptype temptype = curtype;
		//store copy of child node value
		MyCompiler::BackEnd::SymbolTable::varval tempval = tempValue;
		//store another copy of the child node value
		t->exprVal = tempValue;
		MyCompiler::BackEnd::SymbolTable::varval tempOp;

		//if another simple expression is in this expression, enter
		if (i % 2 == 0 && max > i + 2)
		{
			//visit multiplicative operator node
			t->children.at(i + 1)->accept(this);
			//store value of node
			tempOp = tempValue;

			//visit next factor node
			t->children.at(i + 2)->accept(this);

			//if the two operands are not of type int or real and are not the same type, then error
			if (curtype != temptype || curtype == MyCompiler::BackEnd::SymbolTable::s)
			{
				std::cout << "Factors do not match in type" << std::endl;
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
					if (tempValue.t != MyCompiler::BackEnd::SymbolTable::i && tempValue.t != MyCompiler::BackEnd::SymbolTable::r)
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
				//update value of expression result
				t->exprVal = tempValue;
			}
		}
	}
}

//function to visit factor nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTFactorNode* f)
{
	//visit child node
	f->children.at(0)->accept(this);
}

//function to visit unary node
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTUnaryNode* u)
{
	//visit child node
	u->children.at(0)->accept(this);

	//handle operator value
	if (u->stringValue.compare("+") == 0)
	{
		//if following node is a string or boolean, then return an error
		if (tempValue.t == MyCompiler::BackEnd::SymbolTable::s || tempValue.t == MyCompiler::BackEnd::SymbolTable::r)
		{
			std::cout << "Error: Invalid type for unary operation" << std::endl;
			throw std::exception("Type Error");
		}
	}
	else if (u->stringValue.compare("-") == 0)
	{
		//update value for integer or real literal
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
		//update value for boolean literal
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
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTSubExprNode* s)
{
	//visit child node
	s->children.at(0)->accept(this);
}

//function to visit function call nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTFunctionCall* f)
{
	//cast first child to identifier node
	MyCompiler::BackEnd::ASTIdentifierNode* l = dynamic_cast<MyCompiler::BackEnd::ASTIdentifierNode*>(f->children.at(0));

	//if function exists in scope, then enter, otherwise return an error
	if (!st.inScope(l->stringVal))
	{
		std::cout << "Semantic Error: Call to function '" << l->stringVal << "' yet no function defined within this scope" << std::endl;
		throw std::exception("Type Error");
	}
	else
	{
		//if parameters are present within function call, enter
		if (f->children.size() == 2)
		{
			//create vector to store types and values of actual parameters
			std::vector<MyCompiler::BackEnd::SymbolTable::ptype> paramTypes;
			std::vector<MyCompiler::BackEnd::SymbolTable::varval> paramValues;

			//loop through actual parameters
			for (int i = 0; i < f->children.at(1)->children.size(); i++)
			{
				//cast actual parameter to expression node type
				MyCompiler::BackEnd::ASTExprNode* param = dynamic_cast<MyCompiler::BackEnd::ASTExprNode*>(f->children.at(1)->children.at(i));
				//visit parameter
				param->accept(this);

				//store type and value of current parameter
				paramTypes.push_back(curtype);
				paramValues.push_back(tempValue);
			}

			//if the actual parameters and the formal parameters do not match, then return an error
			if (!st.isTypeParameters(paramTypes, l->stringVal))
			{
				std::cout << "Semantic Error: Types of actual and formal parameters for function '" << l->stringVal << "' do not match" << std::endl;
			}
			else
			{
				//store itierator to point to existing formal parameters within the symbol table
				std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator fp = st.getEntry(l->stringVal);

				//assign parameter values to formal parameters
				for (int i = 0; i < paramValues.size(); i++)
				{
					//create new temporary symbol table entry variable and set its type to normal variable
					MyCompiler::BackEnd::SymbolTable::stentry* paramEntry = new MyCompiler::BackEnd::SymbolTable::stentry;
					paramEntry->func = false;

					//handle parameter type and value
					if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::b)
					{
						fp->second.fv.params.at(i).b = paramValues.at(i).b;
						paramEntry->vv.t = MyCompiler::BackEnd::SymbolTable::b;
						paramEntry->vv.b = paramValues.at(i).b;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::i)
					{
						fp->second.fv.params.at(i).i = paramValues.at(i).i;
						paramEntry->vv.t = MyCompiler::BackEnd::SymbolTable::i;
						paramEntry->vv.i = paramValues.at(i).i;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::r)
					{
						fp->second.fv.params.at(i).f = paramValues.at(i).f;
						paramEntry->vv.t = MyCompiler::BackEnd::SymbolTable::r;
						paramEntry->vv.f = paramValues.at(i).f;
					}
					else if (paramValues.at(i).t == MyCompiler::BackEnd::SymbolTable::s)
					{
						fp->second.fv.params.at(i).s = paramValues.at(i).s;
						paramEntry->vv.t = MyCompiler::BackEnd::SymbolTable::s;
						paramEntry->vv.s = paramValues.at(i).s;
					}
					//insert the updated function variable into the symbol table
					st.insertInScope(fp->second.fv.paramNames.at(i), *paramEntry);
				}
			}
		}
		//return type of function for later use in previous recursive level
		curtype = st.getType(l->stringVal);
	}
}

//function to visit an identifier node
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTIdentifierNode* i)
{
	//if the current identifier is not within the symbol table, then return an error
	if (!st.inScope(i->stringVal))
	{
		std::cout << "Identifier '" << i->stringVal << "' is not in this scope" << std::endl;
	}
	else
	{
		//store iterator to point to existing identifier in the symbol table
		std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator tempE = st.getEntry(i->stringVal);
		
		//store copy of the identifier type
		curtype = st.getType(i->stringVal);
		tempValue.t = curtype;
		
		//store copy of theh value of the current identifier
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

//function to visit litieral nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTLiteralNode* l)
{
	//visit child node
	l->children.at(0)->accept(this);
}

//function to visit boolean literal nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTBoolLiteral* b)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::b;
	tempValue.t = curtype;
	tempValue.b = b->boolVal;
}

//function to visit integer literal nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTIntegerLiteral* i)
{
	//store copy of literal type and value
	curtype = MyCompiler::BackEnd::SymbolTable::i;
	tempValue.t = curtype;
	tempValue.i = i->numberVal;
}

//function to visit real literal nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTRealLiteral* r)
{
	//store copy of type and value
	curtype = MyCompiler::BackEnd::SymbolTable::r;
	tempValue.t = curtype;
	tempValue.f = r->numberVal;
}

//function to visit string literal nodes
void MyCompiler::BackEnd::SemanticAnalyserVisitor::visit(MyCompiler::BackEnd::ASTStringLiteral* s)
{
	//store copy of type and value
	curtype = MyCompiler::BackEnd::SymbolTable::s;
	tempValue.t = curtype;
	tempValue.s = s->stringVal;
}