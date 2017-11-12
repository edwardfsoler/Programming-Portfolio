#pragma once
#include "ASTNode.h"
#include "Visitor.h"
#include "SymbolTable.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTExprNode : public ASTNode
		{
		public:
			MyCompiler::BackEnd::SymbolTable::varval exprVal;
			void addChild(ASTSimpleExpression* newChild);
			void addChild(ASTRelationalOp* newChild);
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}