#pragma once
#include "ASTNode.h"
#include <string>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTIdentifierNode : public ASTNode
		{
		public:
			std::string stringVal;
			virtual void accept(Visitor* v) { 
				v->visit(this); 
			}
		};
	}
}