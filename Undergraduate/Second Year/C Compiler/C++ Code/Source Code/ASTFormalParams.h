#pragma once
#include "ASTFormalParam.h"
#include "ASTFunctionDeclNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTFormalParams : public ASTNode
		{
		public:
			void addChild(ASTFormalParams* newChild);
			void addChild(ASTFormalParam* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}