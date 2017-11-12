#include "stdafx.h"
#include "ASTSubExprNode.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTSubExprNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}