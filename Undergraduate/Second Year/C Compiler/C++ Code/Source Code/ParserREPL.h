#pragma once
#include "stdafx.h"
#include "Token.h"
#include "ASTNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class ParserREPL
		{
		public:
			//function to parse tokens fed by lexer
			ASTStatementNode* parse(std::string);

			//functions to parse AST Nodes of a respective type
			void parseStatementNode(ASTStatementNode*);
			void parseVariableNode(ASTVariableDeclNode*);
			void parseAssignmentNode(ASTAssignmentNode*);
			void parseWriteNode(ASTWriteNode*);
			void parseExpressionNode(ASTExprNode*);
			void parseFormalParams(ASTFormalParams*);
			void parseFormalParam(ASTFormalParam*);
			void parseSimpleExpression(ASTSimpleExpression*);
			void parseTermNode(ASTTermNode*);
			void parseFactorNode(ASTFactorNode*);
			void parseUnaryNode(ASTUnaryNode*);
			void parseSubExpressionNode(ASTSubExprNode*);
			void parseFunctionCall(ASTFunctionCall*);
			void parseActualParams(ASTActualParams*);
			void parseLiteralNode(ASTLiteralNode*);
		};
	}
}