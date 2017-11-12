#pragma once
#include "ASTExprNode.h"
#include "ASTFactorNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTSubExprNode : public ASTFactorNode
		{
		public:
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}