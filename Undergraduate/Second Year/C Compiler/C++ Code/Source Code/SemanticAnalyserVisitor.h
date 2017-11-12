#pragma once
#include "Visitor.h"
#include "SymbolTable.h"

namespace MyCompiler
{
	namespace BackEnd
	{
		class SemanticAnalyserVisitor : public Visitor {
		protected:
			//variable to store current node type
			SymbolTable::ptype curtype;
			//variable to store current node value
			SymbolTable::varval tempValue;
			//store return type of function for semantic testing
			SymbolTable::ptype returnType;
		public:
			//symbol table variable to manipulate symbol table
			SymbolTable st;

			//visitor functions for each node type
			virtual void visit(ASTActualParams*) {}
			virtual void visit(ASTAdditiveOp*);
			virtual void visit(ASTAssignmentNode*);
			virtual void visit(ASTBoolLiteral*);
			virtual void visit(ASTExprNode*);
			virtual void visit(ASTFactorNode*);
			virtual void visit(ASTFormalParam*){}
			virtual void visit(ASTFormalParams*) {}
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
			virtual void visit(ASTTypeNode*){}
			virtual void visit(ASTUnaryNode*);
			virtual void visit(ASTVariableDeclNode*);
			virtual void visit(ASTWhileNode*);
			virtual void visit(ASTWriteNode*);
			virtual void visit(ASTBlock*);
		};
	}
}