#include "stdafx.h"
#include "ASTNode.h"
#include "ASTVariableDeclNode.h"
#include "ASTIdentifierNode.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTVariableDeclNode::addChild(ASTNode* newChild)
{
	this->children.push_back(newChild);
}