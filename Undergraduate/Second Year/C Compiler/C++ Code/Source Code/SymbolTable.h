#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdbool.h>
#include <stack>
#include "ASTBlock.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class SymbolTable {
		public:
			//enumeration of possible variable types
			enum ptype { i, r, b, s };

			//symbol table variable entry struct
			struct varval {
				//variable type
				ptype t;

				//variable values for respective type
				union {
					int i;
					float f;
					bool b;
				};
				std::string s;
			};

			//symbol table function entry struct
			struct funcval {
				//return type of function
				varval t;
				//vector of parameter variables
				std::vector<varval> params;
				//vector of parameter names
				std::vector<std::string> paramNames;
				//pointer to body of function
				ASTBlock* body;
			};

			//symbol table entry struct
			struct stentry {
				//boolean defining whether entry is function definition or variable
				bool func = false;
				//function type variable
				funcval fv;
				//variable
				varval vv;
			};
			//stack of map from entry names to values acting as a stack of scopes
			std::stack<std::map<std::string, stentry>> scopeStack;

			//function to create a new scope
			bool createScope();

			//function to delete the top scope
			bool deleteScope();

			//function to insert an entry into the top scope
			bool insertInScope(std::string, stentry);
			
			//function to test the type of a given entry
			bool isType(ptype, std::string);

			//function to test the return type of a given function entry
			bool isTypeReturn(ptype, std::string);

			//function to test the parameter types of a given function entry
			bool isTypeParameters(std::vector<ptype>, std::string);

			//function to test whether a given entry is within any scope
			bool inScope(std::string);

			//function to test whether a given entry is within the top most scope
			bool inCurrentScope(std::string);

			//function to get the type of a given entry
			ptype getType(std::string);

			//function to get an entry from the symbol table
			std::map<std::string, stentry>::iterator getEntry(std::string);
		};
	}
}