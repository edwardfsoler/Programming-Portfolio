#pragma once
#include "ASTLiteralNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTIntegerLiteral : public ASTLiteralNode
		{
		public:
			float numberVal;
			virtual void accept(Visitor* v) {
				v->visit(this);
			}
		};
	}
}