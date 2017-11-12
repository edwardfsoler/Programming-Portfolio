#include "stdafx.h"
#include "ASTFormalParams.h"
#include "ASTFormalParam.h"

using namespace std;

void MyCompiler::BackEnd::ASTFormalParams::addChild(ASTFormalParam* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFormalParams::addChild(ASTFormalParams* newChild)
{
	this->children.push_back(newChild);
}