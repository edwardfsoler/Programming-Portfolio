#pragma once
#include "ASTStatementNode.h"
#include "ASTExprNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTWriteNode : public ASTStatementNode
		{
		public:
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v)
			{
				v->visit(this);
			}
		};
	}
}