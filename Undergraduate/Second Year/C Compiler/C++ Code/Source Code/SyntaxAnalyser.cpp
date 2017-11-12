#include "stdafx.h"
#include <string>
#include <iostream>
#include "Visitor.h"
#include "stdafx.h"
#include "Visitor.h"
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
#include "PrintInfoVisitor.h"
#include "SyntaxAnalyser.h"

using namespace std;

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTAssignmentNode* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

void MyCompiler::BackEnd::SyntaxAnalyser::visit(MyCompiler::BackEnd::ASTActualParams* pnode)
{
	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
}

