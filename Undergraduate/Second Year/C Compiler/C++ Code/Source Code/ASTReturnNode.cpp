#include "stdafx.h"
#include "ASTReturnNode.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTReturnNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}