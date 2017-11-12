// CompilerTester.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Lexer.h"
#include "Parser.h"
#include "ParserREPL.h"
#include "PrintInfoVisitor.h"
#include "Interpreter.h"
#include "InterpreterREPL.h"
#include "InterpreterREPLScript.h"
#include "SemanticAnalyserVisitor.h"

#include <iostream>
#include <iomanip>

using namespace std;

void main() 
{
	//definition of objects for use when parsing, testing and interpreting the given script/commands
	MyCompiler::BackEnd::Parser p;
	MyCompiler::BackEnd::ParserREPL pr;
	MyCompiler::BackEnd::ASTProgramNode* pt;
	MyCompiler::BackEnd::ASTStatementNode* pt2;
	MyCompiler::BackEnd::PrintInfoVisitor* piv = new MyCompiler::BackEnd::PrintInfoVisitor();
	MyCompiler::BackEnd::SemanticAnalyserVisitor* st = new MyCompiler::BackEnd::SemanticAnalyserVisitor();
	MyCompiler::BackEnd::Interpreter* i = new MyCompiler::BackEnd::Interpreter();
	MyCompiler::BackEnd::InterpreterREPL* ir = new MyCompiler::BackEnd::InterpreterREPL();
	MyCompiler::BackEnd::InterpreterREPLScript* irs = new MyCompiler::BackEnd::InterpreterREPLScript();
	MyCompiler::BackEnd::SymbolTable::stentry* ans = new MyCompiler::BackEnd::SymbolTable::stentry;
	MyCompiler::BackEnd::SymbolTable* s = new MyCompiler::BackEnd::SymbolTable();
	map<string, MyCompiler::BackEnd::SymbolTable::stentry> m;
	map<string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator stit;

	int choice = 0;
	int temp1 = 0, temp2 = 0;
	bool repeat = true;
	bool func = true;
	string filename;
	string input;
	do
	{
		//main menu
		cout << "---------------- MiniLang Compiler ----------------" << endl;
		cout << "1) Standard MiniLang Compiler" << endl;
		cout << "2) MiniLang REPL" << endl;
		cout << "3) Quit" << endl << endl;
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "Please enter the name of the text file you would like to compiler: ";
			//read in name of file to compiler
			cin >> filename;
			cout << "---------------- Syntax Errors ----------------" << endl << endl;
			//call parse function to perform lexical analysis and parse the tokens
			pt = p.parse(filename);
			if (pt == NULL)
			{
				continue;
			}
			cout << "---------------- Program Abstract Syntax Tree in XML format ----------------" << endl << endl;
			//visit each node within the created syntax tree, outputing each node in XML format
			piv->visit(pt);

			//perform semantic analysis on the produced syntax tree
			st->visit(pt);
			cout << "---------------- Output ----------------" << endl << endl;
			//interpret the produced syntax tree, executing the code
			i->visit(pt);
			break;
		case 2:
			cout << "---------------- REPL ----------------" << endl;
			cout << "Type '#quit' to return to the main menu" << endl;
			cout << "Type '#st' to view the symbol table contents" << endl;
			cout << "Type '#load' to load an external script" << endl;
			cout << "Variable 'ans' stores the last computed value" << endl;
			
			cin.ignore();

			//create REPL scope
			ir->st.createScope();
			do
			{
				cout << "MiniLang>";
				//read user input
				getline(cin, input);
				//handle exit from REPL
				if (input.compare("#quit") == 0)
				{
					break;
				}
				//handle loading of script
				else if (input.find("#load") == 0)
				{
					temp1 = input.find_first_of('"') + 1;
					temp2 = input.find_last_of('"');
					filename = input.substr(temp1, temp2 - temp1);

					if (filename.length() == 0 || filename.compare("#load") == 0)
					{
						cout << "Invalid file name" << endl;
						continue;
					}
					pt = p.parse(filename);
					if (pt != nullptr)
					{
						//create scope for function interpretation
						irs->st.createScope();
						//interpret function declaration
						irs->visit(pt);

						for (stit = irs->st.scopeStack.top().begin(); stit != irs->st.scopeStack.top().end(); stit++)
						{
							ir->st.scopeStack.top().insert(std::pair<string, MyCompiler::BackEnd::SymbolTable::stentry>(stit->first, stit->second));
						}

						//delete scope for function interpretation
						irs->st.deleteScope();
					}
					continue;
				}
				//handle outputting of symbol table
				else if (input.compare("#st") == 0)
				{
					//if symbol table is empty, enter
					if (ir->st.scopeStack.top().size() == 0)
					{
						cout << "Symbol table is empty" << endl;
					}
					else
					{
						cout << "Variable Value" << setw(10) << "Function?" << setw(10) << "Type" << setw(10) << "Value" << endl;
						for (stit = ir->st.scopeStack.top().begin(); stit != ir->st.scopeStack.top().end(); stit++)
						{
							//output symbol table entry name
							cout << stit->first << setw(10);
							if (stit->second.func == true)
							{
								cout << "Yes" << setw(10);

								if (stit->second.fv.t.t == MyCompiler::BackEnd::SymbolTable::b)
								{
									cout << "Boolean" << setw(10);
									cout << "-" << endl;
								}
								else if (stit->second.fv.t.t == MyCompiler::BackEnd::SymbolTable::i)
								{
									cout << "Integer" << setw(10);
									cout << "-" << endl;
								}
								else if (stit->second.fv.t.t == MyCompiler::BackEnd::SymbolTable::r)
								{
									cout << "Real" << setw(10);
									cout << "-" << endl;
								}
								else if (stit->second.fv.t.t == MyCompiler::BackEnd::SymbolTable::s)
								{
									cout << "String" << setw(10);
									cout << "-" << endl;
								}
							}
							else
							{
								cout << "No" << setw(10);

								if (stit->second.vv.t == MyCompiler::BackEnd::SymbolTable::b)
								{
									cout << "Boolean" << setw(10);
									if (stit->second.vv.b == 1)
									{
										cout << "true" << endl;
									}
									else
									{
										cout << "false" << endl;
									}
								}
								else if (stit->second.vv.t == MyCompiler::BackEnd::SymbolTable::i)
								{
									cout << "Integer" << setw(10) << stit->second.vv.i << endl;
								}
								else if (stit->second.vv.t == MyCompiler::BackEnd::SymbolTable::r)
								{
									cout << "Real" << setw(10) << stit->second.vv.f << endl;
								}
								else if (stit->second.vv.t == MyCompiler::BackEnd::SymbolTable::s)
								{
									cout << "String" << setw(10) << stit->second.vv.s << endl;
								}
							}
						}
					}
					continue;
				}
				//parse input
				pt2 = NULL;
				pt2 = pr.parse(input);
				if (pt2 != nullptr)
				{
					//interpret code
					ir->visit(pt2);
				}
			} while (true);

			//delete the REPL scope
			ir->st.deleteScope();
			break;
		case 3:
			//exit do-while loop
			repeat = false;
			break;
		default:
			cout << "Invalid Input: Please enter a value between 1 and 2" << endl;
			break;
		}
		//repeat until quit option selected
	} while (repeat);
}