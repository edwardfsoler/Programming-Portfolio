#include "stdafx.h"
#include "ASTFunctionCall.h"
#include "ASTActualParams.h"
#include "ASTIdentifierNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTFunctionCall::addChild(ASTActualParams* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFunctionCall::addChild(ASTIdentifierNode* newChild)
{
	this->children.push_back(newChild);
}