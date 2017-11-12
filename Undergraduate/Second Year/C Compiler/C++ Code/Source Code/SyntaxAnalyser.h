#pragma once
#include <map>
#include <string>
#include "ASTProgramNode.h"
#include "Visitor.h"

namespace  MyCompiler
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

		class SyntaxAnalyser: public Visitor
		{
			std::map<std::string, int> v2t;
		public:
			virtual void visit(ASTActualParams*);
			virtual void visit(ASTAdditiveOp*);
			virtual void visit(ASTAssignmentNode*);
			virtual void visit(ASTBoolLiteral*);
			virtual void visit(ASTExprNode*);
			virtual void visit(ASTFactorNode*);
			virtual void visit(ASTFormalParam*);
			virtual void visit(ASTFormalParams*);
			virtual void visit(ASTFunctionCall*);
			virtual void visit(ASTFunctionDeclNode*);
			virtual void visit(ASTIdentifierNode*);
			virtual void visit(ASTIfNode*);
			virtual void visit(ASTIntegerLiteral*);
			virtual void visit(ASTLiteralNode*);
			virtual void visit(ASTMultiplicativeOp*);
			virtual void visit(ASTProgramNode*);
			virtual void visit(ASTRealLiteral*);
			virtual void visit(ASTRelationalOp*);
			virtual void visit(ASTReturnNode*);
			virtual void visit(ASTSimpleExpression*);
			virtual void visit(ASTStatementNode*);
			virtual void visit(ASTStringLiteral*);
			virtual void visit(ASTSubExprNode*);
			virtual void visit(ASTTermNode*);
			virtual void visit(ASTTypeNode*);
			virtual void visit(ASTUnaryNode*);
			virtual void visit(ASTVariableDeclNode*);
			virtual void visit(ASTWhileNode*);
			virtual void visit(ASTWriteNode*);
			virtual void visit(ASTBlock*);
		};
	}
}