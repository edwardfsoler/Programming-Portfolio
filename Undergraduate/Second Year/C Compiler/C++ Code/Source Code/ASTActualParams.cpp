#include "stdafx.h"
#include "ASTActualParams.h"
#include "ASTExprNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTActualParams::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTActualParams::addChild(ASTActualParams* newChild)
{
	this->children.push_back(newChild);
}