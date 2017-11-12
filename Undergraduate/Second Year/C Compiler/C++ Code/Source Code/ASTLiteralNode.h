#pragma once
#include "ASTNode.h"
#include "ASTFactorNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTLiteralNode : public ASTFactorNode
		{
		public:
			void addChild(ASTBoolLiteral* newChild);
			void addChild(ASTStringLiteral* newChild);
			void addChild(ASTRealLiteral* newChild);
			void addChild(ASTIntegerLiteral* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}