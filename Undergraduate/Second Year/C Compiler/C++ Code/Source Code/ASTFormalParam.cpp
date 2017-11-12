#include "stdafx.h"
#include "ASTTypeNode.h"
#include "ASTIdentifierNode.h"
#include "ASTFormalParam.h"

using namespace std;

void MyCompiler::BackEnd::ASTFormalParam::addChild(ASTIdentifierNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFormalParam::addChild(ASTTypeNode* newChild)
{
	this->children.push_back(newChild);
}