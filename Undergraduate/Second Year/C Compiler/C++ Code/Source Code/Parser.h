#pragma once
#include "stdafx.h"
#include "Token.h"
#include "ASTNode.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class Parser
		{
		public:
			//function to parse tokens fed by lexer
			ASTProgramNode* parse(std::string);

			//functions to parse AST Nodes of a respective type
			void parseProgramNode(ASTNode*);
			void parseStatementNode(ASTStatementNode*);
			void parseVariableNode(ASTVariableDeclNode*);
			void parseAssignmentNode(ASTAssignmentNode*);
			void parseWriteNode(ASTWriteNode*);
			void parseReturnNode(ASTReturnNode*);
			void parseFunctionDeclNode(ASTFunctionDeclNode*);
			void parseIfNode(ASTIfNode*);
			void parseWhileNode(ASTWhileNode*);
			void parseBlockNode(ASTBlock*);
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