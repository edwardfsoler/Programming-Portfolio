#pragma once
#include "ASTExprNode.h"
#include "ASTFunctionCall.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTActualParams : public ASTFunctionCall
		{
		public:
			void addChild(ASTExprNode* newChild);
			void addChild(ASTActualParams* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}
