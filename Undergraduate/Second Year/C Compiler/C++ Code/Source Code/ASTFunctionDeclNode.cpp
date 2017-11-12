#include "stdafx.h"
#include "ASTFormalParams.h"
#include "ASTTypeNode.h"
#include "ASTIdentifierNode.h"
#include "ASTBlock.h"
#include "ASTFunctionDeclNode.h"

using namespace std;

void MyCompiler::BackEnd::ASTFunctionDeclNode::addChild(ASTFormalParams* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFunctionDeclNode::addChild(ASTTypeNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFunctionDeclNode::addChild(ASTBlock* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTFunctionDeclNode::addChild(ASTIdentifierNode* newChild)
{
	this->children.push_back(newChild);
}