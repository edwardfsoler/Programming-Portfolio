#include "stdafx.h"
#include "ASTIfNode.h"
#include "ASTBlock.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTIfNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTIfNode::addChild(ASTBlock* newChild)
{
	this->children.push_back(newChild);
}