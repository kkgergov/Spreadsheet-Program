#pragma once

#include "TokenType.h"
#include "Token.h"

#include "AST_Nodes.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;


//semantic analyzer here

class AST
{
public:
	AST();
	AST(const string& input); //creates ast from string, if it fails, we just create root with text

	AST(const AST&) = default;
	AST& operator=(const AST&) = default;
	~AST(); //delete the expression tree, then the root node

	AST_Node* join(const AST& other); //check if something wrong happens when we edit the other tree
	void deleteExprTree(); //deletes the tree below the root
private:
	void parse();
	void interpret() const;

	AST_Node* root;
};