#pragma once
#include "ASTFactorNode.h"
#include "ASTNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTFunctionCall : public ASTFactorNode
		{
			public:
				void addChild(ASTActualParams* newChild);
				void addChild(ASTIdentifierNode* newChild);
				virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}