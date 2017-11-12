#pragma once
#include "ASTStatementNode.h"
#include "ASTIdentifierNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class ASTDeclarationNode : public ASTStatementNode
		{
		protected:
			MyCompiler::BackEnd::ASTIdentifierNode* identifier;
		public:

			MyCompiler::BackEnd::ASTIdentifierNode* getIdentifier();
		};
	}
}