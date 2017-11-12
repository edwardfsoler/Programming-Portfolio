#include<vector>
#include "Token.h"
namespace MyCompiler
{
	namespace FrontEnd
	{
		//class Lexer which shall extract tokens from the text and feed them to the parser
		class Lexer
		{
		public:
			Lexer(void) {} //Lexer constructor
			~Lexer(void) {} //Lexer Destructor

			//vector to hold tokens
			std::vector<MyCompiler::FrontEnd::Token> tokenVector; 
			
			//function to read text data from file into a string
			std::string readFromFile(std::string filename); 
			
			//function to tokenise each word/phrase in the string text
			void getTokens(std::string input); 

			//function to assign strings to their respective tokens
			void stringHandler(std::string temp, MyCompiler::FrontEnd::Token* tok); 

			//function to assign characters to their respective tokens
			void charHandler(std::string temp, MyCompiler::FrontEnd::Token* tok);

			//function to get next token from 
			MyCompiler::FrontEnd::Token getNextToken();

			//function to get next token from 
			MyCompiler::FrontEnd::Token lookNextToken();

			//function to output tokens for testing purposes
			void tokenTester();
		};
	}
}