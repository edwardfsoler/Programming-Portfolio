#include "stdafx.h"
#include "SymbolTable.h"
#include <vector>
#include <map>
#include <string>
#include <stdbool.h>
#include <iostream>

using namespace std;

//function to create a new scope
bool MyCompiler::BackEnd::SymbolTable::createScope()
{
	//store copy of number of scopes
	int size1 = this->scopeStack.size();
	//create new mapping
	map<string, stentry> m;
	//add mapping to the top of the stack
	this->scopeStack.push(m);

	//if operation was successful return true
	if (this->scopeStack.size() == size1 + 1)
	{
		return true;
	}
	else
	{
		cout << "Something went wrong while adding a scope" << endl;
		return false;
	}
}

//function to delete the top most scope
bool MyCompiler::BackEnd::SymbolTable::deleteScope()
{
	//store copy of number of scopes
	int size1 = this->scopeStack.size();
	//pop the top most scope
	this->scopeStack.pop();

	//if operation was successful return true
	if (this->scopeStack.size() == size1 - 1)
	{
		return true;
	}
	else
	{
		cout << "Something went wrong while deleting the outer scope" << endl;
		return false;
	}
}

//function to insert a given entry into the current scope
bool MyCompiler::BackEnd::SymbolTable::insertInScope(string name, stentry entry)
{
	//store copy oof size of current scope
	int size1 = this->scopeStack.top().size();
	//insert the new entry into the current scope
	this->scopeStack.top().insert(std::pair<string, stentry>(name, entry));

	//if operation was successful, return true
	if (this->scopeStack.top().size() == size1 + 1)
	{
		return true;
	}
	else
	{
		cout << "Semantic Error: Variable '" << name << "' has already been declared" << endl;
		return false;
	}
}

//function to test the type of a given entry
bool MyCompiler::BackEnd::SymbolTable::isType(ptype type, string name)
{
	//create temporary stack mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;
	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the given entry is in the current scope, and the type matches, then return true and exit loop
		if (this->scopeStack.top().find(name)->second.func == false && type == this->scopeStack.top().find(name)->second.vv.t)
		{
			found = true;
			break;
		}

		//if value not found at current level, store copy of current level and pop stack
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//restore stack to original state
	for (int j = 0; j < tempStack.size(); j++)
	{
		this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
	}

	//output result
	if (found)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function to test whether an entry is within any scope
bool MyCompiler::BackEnd::SymbolTable::inScope(string name)
{
	//create temporary mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;

	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the given entry is in this scope, return true and break from loop
		if (this->scopeStack.top().count(name) == 1)
		{
			found = true;
			break;
		}
		//if value not found at current level, store copy of current level and pop
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//restore stack to original state
	for (int j = 0; j < tempStack.size(); j++)
	{
		this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
	}

	//return result
	if (found == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function to test whether the given entry is in the current scope
bool MyCompiler::BackEnd::SymbolTable::inCurrentScope(string name)
{
	//if the given entry is in the current scope, return true
	if (this->scopeStack.top().count(name) == 1)
	{
		return true;
	}

	return false;
}

//function to get the type of a given entry
MyCompiler::BackEnd::SymbolTable::ptype MyCompiler::BackEnd::SymbolTable::getType(string name)
{
	//create temporary mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;
	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the given entry is within the current scope, enter
		if (inCurrentScope(name))
		{
			//if the entry is not a function, enter
			if (this->scopeStack.top().find(name)->second.func == false)
			{
				//retrieve type of entry
				MyCompiler::BackEnd::SymbolTable::ptype temp = this->scopeStack.top().find(name)->second.vv.t;
				
				//restore stack to original state
				for (int j = 0; j < tempStack.size(); j++)
				{
					this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
				}
				//return type
				return temp;
			}
			else
			{
				//retrieve return type of function entry
				MyCompiler::BackEnd::SymbolTable::ptype temp = this->scopeStack.top().find(name)->second.fv.t.t;

				//restore stack to original state
				for (int j = 0; j < tempStack.size(); j++)
				{
					this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
				}
				//return type
				return temp;
			}
		}

		//if value not found at current level, store copy of current level and pop
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//search has failed
	std::cout << "Search for type failed" << std::endl;
}

//function to retrieve a given entry
std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator MyCompiler::BackEnd::SymbolTable::getEntry(string name)
{
	//create temporary mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;
	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the given entry is within the current scope, enter
		if (inCurrentScope(name))
		{
			//if given entry is not a function, enter
			if (this->scopeStack.top().find(name)->second.func == false)
			{
				//store iterator to point to the given entry
				std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator temp = this->scopeStack.top().find(name);
				//restore stack to original state
				for (int j = 0; j < tempStack.size(); j++)
				{
					this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
				}
				//return entry
				return temp;
			}
			else
			{
				//store iterator to point to the given function entry
				std::map<std::string, MyCompiler::BackEnd::SymbolTable::stentry>::iterator temp = this->scopeStack.top().find(name);

				//restore stack to original state
				for (int j = 0; j < tempStack.size(); j++)
				{
					this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
				}
				//return entry
				return temp;
			}
		}
		//if value not found at current level, store copy of current level and pop
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//search failed
	std::cout << "Search for entry failed" << std::endl;
}

//function to test the return type of a function entry
bool MyCompiler::BackEnd::SymbolTable::isTypeReturn(ptype type, string name)
{
	//creation of temporary mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;
	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the entry is within the current scope and its return type matches, enter
		if (this->scopeStack.top().find(name)->second.func == true && type == this->scopeStack.top().find(name)->second.fv.t.t)
		{
			found = true;
			break;
		}

		//if value not found at current level, store copy of current level and pop
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//restore stack to original state
	for (int j = 0; j < tempStack.size(); j++)
	{
		this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
	}

	//return result
	if (found)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function to test the parameter types of a function entry
bool MyCompiler::BackEnd::SymbolTable::isTypeParameters(vector<ptype> paramTypes, string name)
{
	//creation of temporary mapping vector
	std::vector <std::map<std::string, stentry>> tempStack;
	bool found = false;

	//test all levels of stack for entry
	while (this->scopeStack.size() != 0)
	{
		//if the entry is within the current scope and is of type function, then enter
		if (inCurrentScope(name) && this->scopeStack.top().find(name)->second.func == true)
		{
			//if found entry has no parameters, then return false
			if (this->scopeStack.top().find(name)->second.fv.params.size() == 0)
			{
				found = false;
				break;
			}
			//else store copy of found entry's parameters
			vector<varval> params = this->scopeStack.top().find(name)->second.fv.params;

			//if the parameter vectors are of different sizes, then return false
			if (params.size() != paramTypes.size())
			{
				params.clear();
				continue;
			}

			//otherwise, loop through vectors comparing types of each parameter
			for (int i = 0; i < params.size(); i++)
			{
				//if any single parameter differs in type, return false
				if (params.at(i).t != paramTypes.at(i))
				{
					found = false;
					break;
				}
			}
			//otherwise return tre
			found = true;
			break;
		}
		//if value not found at current level, store copy of current level and pop
		tempStack.push_back(this->scopeStack.top());
		this->scopeStack.pop();
	}

	//restore stack to original state
	for (int j = 0; j < tempStack.size(); j++)
	{
		this->scopeStack.push(tempStack.at(tempStack.size() - (1 + j)));
	}

	//return result
	if (found)
	{
		return true;
	}
	else
	{
		return false;
	}
}