#pragma once
#include "ASTLiteralNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTStringLiteral : public ASTLiteralNode
		{
		public:
			std::string stringVal;
			virtual void accept(Visitor* v) {
				v->visit(this);
			}
		};
	}
}