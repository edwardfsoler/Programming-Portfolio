#pragma once
#include "ASTNode.h"
#include "ASTStatementNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTAssignmentNode : public ASTStatementNode
		{
		public:
			void addChild(ASTNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}