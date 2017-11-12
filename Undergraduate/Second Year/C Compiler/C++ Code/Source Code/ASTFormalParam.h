#pragma once
#include "ASTNode.h"
#include "ASTFormalParams.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTFormalParam : public ASTNode
		{
		public:
			void addChild(ASTIdentifierNode* newChild);
			void addChild(ASTTypeNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}