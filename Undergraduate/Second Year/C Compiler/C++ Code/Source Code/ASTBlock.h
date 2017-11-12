#pragma once
#include "ASTNode.h"
#include "Visitor.h"
#include "ASTStatementNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTBlock : public ASTStatementNode
		{
		public:
				void addChild(ASTStatementNode* newChild);
				virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}