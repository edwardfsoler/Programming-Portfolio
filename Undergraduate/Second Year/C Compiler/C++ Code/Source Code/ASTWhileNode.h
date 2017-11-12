#pragma once
#include "Visitor.h"
#include "ASTStatementNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTWhileNode : public ASTStatementNode
		{
		public:
			void addChild(ASTBlock* newChild);
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}