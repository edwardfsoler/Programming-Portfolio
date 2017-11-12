#include "stdafx.h"
#include "ASTBlock.h"
#include "ASTWhileNode.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTWhileNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTWhileNode::addChild(ASTBlock* newChild)
{
	this->children.push_back(newChild);
}