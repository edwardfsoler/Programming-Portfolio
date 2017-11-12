#pragma once
#include "ASTNode.h"
#include "ASTStatementNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTProgramNode: public ASTNode
		{
		public:
			ASTProgramNode(){}
			void addChild(ASTStatementNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}
