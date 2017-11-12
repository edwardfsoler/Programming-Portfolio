#pragma once
#include "ASTStatementNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTFunctionDeclNode : public ASTStatementNode
		{

		public:
			void addChild(ASTFormalParams* newChild);
			void addChild(ASTTypeNode* newChild);
			void addChild(ASTBlock* newChild);
			void addChild(ASTIdentifierNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}