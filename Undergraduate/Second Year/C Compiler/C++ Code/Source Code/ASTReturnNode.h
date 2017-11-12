#pragma once
#include "Visitor.h"
#include "ASTStatementNode.h"
#include "ASTExprNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTReturnNode : public ASTStatementNode
		{
		public:
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}