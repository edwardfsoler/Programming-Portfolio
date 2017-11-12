#include "stdafx.h"
#include "ASTWriteNode.h"
#include "ASTExprNode.h"

using namespace std;


void MyCompiler::BackEnd::ASTWriteNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}