#include "stdafx.h"
#include "ASTNode.h"
#include "ASTAssignmentNode.h"
#include "ASTIdentifierNode.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTAssignmentNode::addChild(ASTNode* newChild)
{
	this->children.push_back(newChild);
}