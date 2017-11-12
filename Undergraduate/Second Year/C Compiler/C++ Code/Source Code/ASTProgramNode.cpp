#include "stdafx.h"
#include <vector>
#include "ASTStatementNode.h"
#include "ASTProgramNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTProgramNode::addChild(ASTStatementNode* newChild)
{
	this->children.push_back(newChild);
}