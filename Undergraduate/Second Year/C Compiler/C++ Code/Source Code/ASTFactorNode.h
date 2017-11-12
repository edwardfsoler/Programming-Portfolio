#pragma once
#include "ASTNode.h"
#include "ASTTermNode.h"
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTFactorNode : public ASTTermNode
		{
		public:
			void addChild(ASTUnaryNode* newChild);
			void addChild(ASTSubExprNode* newChild);
			void addChild(ASTFunctionCall* newChild);
			void addChild(ASTLiteralNode* newChild);
			void addChild(ASTIdentifierNode* newChild);
			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}