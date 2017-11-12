#include "stdafx.h"
#include "Visitor.h"
#include "ASTActualParams.h"
#include "ASTAdditiveOp.h"
#include "ASTAssignmentNode.h"
#include "ASTBlock.h"
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
#include <string>
#include <iostream>

using namespace std;

//function to print actual parameter nodes in XML format
void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTActualParams* pnode)
{
	string tabs = "";
	//set indent width according to current indent value
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	//increment indent for child nodes
	indent++;
	
	//output node title opening tag
	cout << tabs << "<Actual Parameters>" << endl;
	
	//visit each child node of the current node
	for (int i = 0; i < pnode->children.size(); i++)
	{ 
		pnode->children.at(i)->accept(this);
	}
	//decrement indent again to return to the 
	indent--;

	//output node title closing tag
	cout << tabs << "</Actual Parameters>" << endl;
}

//all other print info visitors of non-terminal nodes follow the same principle, with only the node type changing

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTAdditiveOp* pnode)
{
	string tabs = ""; 
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}

	//output the value of the additive operator
	cout << tabs << "<Additive Op>" << pnode->stringVal << "</Additive Op>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTAssignmentNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Assignment>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Assignment>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTBoolLiteral* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Boolean Literal>";

	//output 'false' if the value of 0 is stored, true otherwise
	if (pnode->boolVal == 0) 
	{
		cout << "false";
	}
	else
	{
		cout << "true";
	}

	 cout  << "</Boolean Literal>" << endl;

	indent--;
	
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTExprNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Expression>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Expression>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTFactorNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Factor>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Factor>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTFormalParam* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Formal Parameter>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Formal Parameter>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTFormalParams* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Formal Parameters>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Formal Parameters>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTFunctionCall* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Function Call>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Function Call>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTFunctionDeclNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Function Declaration>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Function Declaration>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTIdentifierNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Identifier>" << pnode->stringVal << "</Identifier>" << endl;
	indent--;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTIfNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<If Statement>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</If Statement>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTIntegerLiteral* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Integer Literal>" << pnode->numberVal << "</Integer Literal>" << endl;
	indent--;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTLiteralNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Literal>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Literal>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTMultiplicativeOp* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Multiplicative Op>" << pnode->stringVal << "</Multiplicative Op>" << endl;
	
	indent--;
}

//starting visitor node
void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTProgramNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Program>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Program>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTRealLiteral* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Real Literal>" << pnode->numberVal << "</Real Literal>" << endl;
	
	indent--;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTRelationalOp* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Relational Op>" << pnode->stringVal << "</Relational Op>" << endl;
	indent--;
	
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTReturnNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Return>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Return>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTSimpleExpression* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Simple Expression>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Simple Expression>" << endl;
}


void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTStatementNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Statement>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Statement>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTStringLiteral* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<String Literal>" << pnode->stringVal << "</String Literal>" << endl;
	
	indent--;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTSubExprNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Sub-Expression>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Sub-Expression>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTTermNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Term>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Term>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTTypeNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Type>" << pnode->stringVal << "</Type>" << endl;
	indent--;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTUnaryNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Unary Op [" << pnode->stringValue << "]>" << endl;
	

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Unary Op>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTVariableDeclNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Variable Declaration>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Variable Declaration>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTWhileNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<While Loop>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</While Loop>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTWriteNode* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Write Statement>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Write Statement>" << endl;
}

void MyCompiler::BackEnd::PrintInfoVisitor::visit(ASTBlock* pnode)
{
	string tabs = "";
	for (int i = 0; i < indent; i++)
	{
		tabs.append("   ");
	}
	indent++;

	cout << tabs << "<Block>" << endl;

	for (int i = 0; i < pnode->children.size(); i++)
	{
		pnode->children.at(i)->accept(this);
	}
	indent--;
	cout << tabs << "</Block>" << endl;
}