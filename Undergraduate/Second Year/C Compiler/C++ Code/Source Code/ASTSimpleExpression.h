#pragma once
#include "ASTnode.h"
#include "ASTExprNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTSimpleExpression : public ASTExprNode
		{
		public:
			MyCompiler::BackEnd::SymbolTable::varval exprVal;
			void addChild(ASTTermNode* newChild);
			void addChild(ASTSimpleExpression* newChild);
			void addChild(ASTAdditiveOp* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}