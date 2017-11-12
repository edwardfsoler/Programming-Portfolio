#include "stdafx.h"
#include "ASTUnaryNode.h"
#include "ASTSubExprNode.h"
#include "ASTLiteralNode.h"
#include "ASTIdentifierNode.h"
#include "ASTFunctionCall.h"
#include "ASTFactorNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTFactorNode::addChild(ASTUnaryNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFactorNode::addChild(ASTSubExprNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFactorNode::addChild(ASTLiteralNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFactorNode::addChild(ASTIdentifierNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFactorNode::addChild(ASTFunctionCall* newChild)
{
	this->children.push_back(newChild);
}