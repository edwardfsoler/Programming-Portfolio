#pragma once
#include "ASTNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTStatementNode : public ASTNode
		{
		public:
			void addChild(ASTVariableDeclNode* newChild);
			void addChild(ASTAssignmentNode* newChild);
			void addChild(ASTWriteNode* newChild);
			void addChild(ASTReturnNode* newChild);
			void addChild(ASTFunctionDeclNode* newChild);
			void addChild(ASTIfNode* newChild);
			void addChild(ASTWhileNode* newChild);
			void addChild(ASTBlock* newChild);
			void addChild(ASTExprNode* newChild);

			virtual void accept(Visitor* v){ v->visit(this); }
		};
	}
}