#include "stdafx.h"
#include "ASTNode.h"
#include "ASTBlock.h"
#include "ASTStatementNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTBlock::addChild(ASTStatementNode* newChild)
{
	this->children.push_back(newChild);
}
