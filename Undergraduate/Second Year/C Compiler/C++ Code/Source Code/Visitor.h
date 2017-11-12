#pragma once
#include "stdafx.h"


namespace MyCompiler
{
	namespace BackEnd
	{
		class ASTActualParams;
		class ASTAdditiveOp;
		class ASTAssignmentNode;
		class ASTBoolLiteral;
		class ASTExprNode;
		class ASTFactorNode;
		class ASTFormalParam;
		class ASTFormalParams;
		class ASTFunctionCall;
		class ASTFunctionDeclNode;
		class ASTIdentifierNode;
		class ASTIfNode;
		class ASTIntegerLiteral;
		class ASTLiteralNode;
		class ASTMultiplicativeOp;
		class ASTProgramNode;
		class ASTRealLiteral;
		class ASTRelationalOp;
		class ASTReturnNode;
		class ASTSimpleExpression;
		class ASTStatementNode;
		class ASTStringLiteral;
		class ASTSubExprNode;
		class ASTTermNode;
		class ASTTypeNode;
		class ASTUnaryNode;
		class ASTVariableDeclNode;
		class ASTWhileNode;
		class ASTWriteNode;
		class ASTBlock;

		class Visitor
		{
		public:
			//visit functions for each node type
			virtual void visit(ASTActualParams*) = 0;
			virtual void visit(ASTAdditiveOp*) = 0;
			virtual void visit(ASTAssignmentNode*) = 0;
			virtual void visit(ASTBoolLiteral*) = 0;
			virtual void visit(ASTExprNode*) = 0;
			virtual void visit(ASTFactorNode*) = 0;
			virtual void visit(ASTFormalParam*) = 0;
			virtual void visit(ASTFormalParams*) = 0;
			virtual void visit(ASTFunctionCall*) = 0;
			virtual void visit(ASTFunctionDeclNode*) = 0;
			virtual void visit(ASTIdentifierNode*) = 0;
			virtual void visit(ASTIfNode*) = 0;
			virtual void visit(ASTIntegerLiteral*) = 0;
			virtual void visit(ASTLiteralNode*) = 0;
			virtual void visit(ASTMultiplicativeOp*) = 0;
			virtual void visit(ASTProgramNode*) = 0;
			virtual void visit(ASTRealLiteral*) = 0;
			virtual void visit(ASTRelationalOp*) = 0;
			virtual void visit(ASTReturnNode*) = 0;
			virtual void visit(ASTSimpleExpression*) = 0;
			virtual void visit(ASTStatementNode*) = 0;
			virtual void visit(ASTStringLiteral*) = 0;
			virtual void visit(ASTSubExprNode*) = 0;
			virtual void visit(ASTTermNode*) = 0;
			virtual void visit(ASTTypeNode*) = 0;
			virtual void visit(ASTUnaryNode*) = 0;
			virtual void visit(ASTVariableDeclNode*) = 0;
			virtual void visit(ASTWhileNode*) = 0;
			virtual void visit(ASTWriteNode*) = 0;
			virtual void visit(ASTBlock*) = 0;
		};
	}
}