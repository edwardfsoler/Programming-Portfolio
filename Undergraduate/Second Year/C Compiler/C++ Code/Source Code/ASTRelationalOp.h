#pragma once
#include "ASTExprNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTRelationalOp : public ASTExprNode
		{
		public:
			std::string stringVal;
			virtual void accept(Visitor* v) {
				v->visit(this);
			}
		};
	}
}