#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <vector>
#include <stdexcept>
#include "AST_Nodes.h"
#include "Token.h"

class Parser
{
private:

	std::vector<Token> t_input;
	int pos;
	Token current_token;

	void eat(TokenType);

	//we create the actual tree from here
	AST_Node* cell();

	AST_Node* COND_expr();

	AST_Node* OR_expr();

	AST_Node* AND_expr();

	AST_Node* NOT_expr();
	AST_Node* COMP_expr();

	AST_Node* ADD_expr();

	AST_Node* MULT_expr();

	AST_Node* NEGATE_expr();

	AST_Node* Value();

public:
	Parser(const std::vector<Token>& );

	AST_Node* parse();
};

#endif // !PARSER_HEADER


