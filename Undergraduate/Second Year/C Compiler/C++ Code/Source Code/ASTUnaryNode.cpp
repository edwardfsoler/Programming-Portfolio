#include "stdafx.h"
#include "ASTUnaryNode.h"
#include "ASTExprNode.h"
#include <string>

using namespace std;


void MyCompiler::BackEnd::ASTUnaryNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}