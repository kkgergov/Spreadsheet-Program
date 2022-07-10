#ifndef SYNTAX_ANALYZER_HEADER
#define SYNTAX_ANALYZER_HEADER

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream> //to be removed after testing, the parser will handle the error
#include "Token.h"

//analyzes if syntax of the tokenized input follows the allowed grammar
class SyntaxAnalyzer
{
private:
	std::vector<Token> t_input;
	int pos;
	Token current_token;

	//checks if token is of the proper type, then consumes it and updates current_token
	void eat(TokenType);


	void cell();
	void COND_expr();
	void OR_expr();
	void AND_expr();

	void NOT_expr();

	void COMP_expr();

	void ADD_expr();

	void MULT_expr();

	void NEGATE_expr();

	void Value();

public:
	SyntaxAnalyzer(const std::vector<Token>&);
	bool correct();
};

#endif // !SYNTAX_ANALYZER


