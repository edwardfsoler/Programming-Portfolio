#pragma once
#include <vector>
#include "Visitor.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Visitor;

		class ASTNode
		{
		public:
			std::vector<MyCompiler::BackEnd::ASTNode*> children;
			virtual void accept(Visitor* v) = 0;
			virtual void addChild(ASTNode*) {};
		};
	}
}