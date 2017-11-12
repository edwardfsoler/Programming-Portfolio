#include "stdafx.h"
#include "ASTNode.h"
#include "ASTStatementNode.h"
#include "ASTVariableDeclNode.h"
#include "ASTAssignmentNode.h"
#include "ASTWriteNode.h"
#include "ASTReturnNode.h"
#include "ASTFunctionDeclNode.h"
#include "ASTIfNode.h"
#include "ASTWhileNode.h"
#include "ASTBlock.h"

#include <vector>

using namespace std;

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTVariableDeclNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTAssignmentNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTWriteNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTReturnNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTFunctionDeclNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTIfNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTWhileNode* newChild)
{
	this->children.push_back(newChild);
}

void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTBlock* newChild)
{
	this->children.push_back(newChild);
}

//for REPL Parser
void MyCompiler::BackEnd::ASTStatementNode::addChild(ASTExprNode* newChild)
{
	this->children.push_back(newChild);
}