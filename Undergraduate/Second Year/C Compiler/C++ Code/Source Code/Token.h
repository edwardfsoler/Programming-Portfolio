#pragma once
#include <string>
#include <set>

namespace MyCompiler
{
	namespace FrontEnd
	{
		//enumeration of token types
		enum TOK_TYPE { TOK_Type, TOK_BooleanLiteral, TOK_Digit, TOK_StringLiteral, TOK_Identifier, TOK_MultiplicativeOp, TOK_AdditiveOp, TOK_RelationalOp, TOK_AssignmentOp, TOK_Keyword, TOK_Punctuation, TOK_EOF };
		class Token {
			//token attributes
			TOK_TYPE type;
			std::string name = ""; //token name
			int value = 0; //token value
		public:
			//function to set the token name
			void setName(std::string n);

			//function to retrieve the token name
			std::string getName();

			//function to set the token value
			void setVal(int val);

			//function to retrieve the token value
			int getVal();

			//function to set the token type
			void setType(TOK_TYPE type);

			//function to retireve the token type
			TOK_TYPE getType();
		};
	}
}