#pragma once
#include "ASTTermNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTMultiplicativeOp : public ASTTermNode
		{
		public:
			std::string stringVal;
			virtual void accept(Visitor* v) {
				v->visit(this);
			}
		};
	}
}