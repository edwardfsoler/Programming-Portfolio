#include "stdafx.h"
#include "ASTNode.h"
#include "ASTTermNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTTermNode::addChild(ASTNode* newChild)
{
	this->children.push_back(newChild);
}