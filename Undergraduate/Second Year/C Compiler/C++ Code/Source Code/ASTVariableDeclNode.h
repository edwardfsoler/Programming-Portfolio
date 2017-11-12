#pragma once
#include "ASTNode.h"
#include "ASTStatementNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTVariableDeclNode : public ASTStatementNode
		{
		public:
			void addChild(ASTNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}