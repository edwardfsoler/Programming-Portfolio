#include "stdafx.h"
#include "ASTLiteralNode.h"
#include "ASTBoolLiteral.h"
#include "ASTIntegerLiteral.h"
#include "ASTRealLiteral.h"
#include "ASTStringLiteral.h"


void MyCompiler::BackEnd::ASTLiteralNode::addChild(ASTBoolLiteral* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTLiteralNode::addChild(ASTIntegerLiteral* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTLiteralNode::addChild(ASTRealLiteral* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTLiteralNode::addChild(ASTStringLiteral* newChild)
{
	this->children.push_back(newChild);
}