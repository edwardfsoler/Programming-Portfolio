#pragma once
#include "ASTNode.h"
#include "ASTStatementNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTIfNode : public ASTStatementNode
		{
		public:
			void addChild(ASTExprNode* newChild);
			void addChild(ASTBlock* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}