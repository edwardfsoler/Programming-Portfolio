#pragma once
#include <string>
#include "ASTSimpleExpression.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTAdditiveOp : public ASTSimpleExpression
		{
		public:
			std::string stringVal;
			virtual void accept(Visitor* v) { v->visit(this); }
		};
	}
}