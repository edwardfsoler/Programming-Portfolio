#pragma once
#include "ASTFactorNode.h"
#include <string>

namespace MyCompiler
{
	namespace BackEnd
	{

		class ASTUnaryNode : public ASTFactorNode
		{
		public:
			std::string stringValue;
			void addChild(ASTExprNode* newChild);
			virtual void accept(Visitor* v) {
				v->visit(this); 
			}
		};
	}
}