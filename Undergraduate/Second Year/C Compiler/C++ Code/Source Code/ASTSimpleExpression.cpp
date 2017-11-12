#include "stdafx.h"
#include "ASTTermNode.h"
#include "ASTAdditiveOp.h"
#include "ASTSimpleExpression.h"

using namespace std;

void MyCompiler::BackEnd::ASTSimpleExpression::addChild(ASTTermNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTSimpleExpression::addChild(ASTSimpleExpression* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTSimpleExpression::addChild(ASTAdditiveOp* newChild)
{
	this->children.push_back(newChild);
}