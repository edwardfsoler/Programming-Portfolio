#include "stdafx.h"
#include "ASTSimpleExpression.h"
#include "ASTRelationalOp.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTExprNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTExprNode::addChild(ASTSimpleExpression* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTExprNode::addChild(ASTRelationalOp* newChild)
{
	this->children.push_back(newChild);
}