#include "stdafx.h"
#include "ASTNode.h"
#include "ASTIdentifierNode.h"
#include "ASTDeclarationNode.h"


using namespace std;

MyCompiler::BackEnd::ASTIdentifierNode* MyCompiler::BackEnd::ASTDeclarationNode::getIdentifier()
{
	return this->identifier;
}