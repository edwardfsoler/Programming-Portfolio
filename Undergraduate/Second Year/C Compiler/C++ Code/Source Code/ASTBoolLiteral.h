#pragma once
#include "ASTLiteralNode.h"
#include "stdbool.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTBoolLiteral : public ASTLiteralNode
		{
		public:
			bool boolVal;
			virtual void accept(Visitor* v) {
				v->visit(this);
			}
		};
	}
}
