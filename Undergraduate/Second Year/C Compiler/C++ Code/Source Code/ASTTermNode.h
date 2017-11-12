#pragma once
#include "ASTNode.h"
#include "ASTSimpleExpression.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class ASTTermNode : public ASTSimpleExpression {
		public:
			MyCompiler::BackEnd::SymbolTable::varval exprVal;
			void addChild(ASTNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}