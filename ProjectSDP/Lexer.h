#ifndef LEXER_HEADER
#define LEXER_HEADER

#include <string>
#include <vector>
#include <stdexcept>
#include "Token.h"

//the lexer class is lexical analyzer as well, because if we encounter something else
//other than the building blocks of our program, we throw an error
class Lexer
{
private:
	std::string text;

	int pos;
	char current_char;

	//increments pos and updates current_char
	void advance();

	//peeks steps positions after current position
	char peek(int);

	//if a digit is encountered, extract the number from text input and convert it to token (floar or int depends on '.' in number)
	Token number_token();

	//if " is encountered, tries to extract the text until next " and converts it into a string token
	Token string_token();

	//gets next token from input, if reaches end - returns END token
	Token get_next_token();

public:
	Lexer(const std::string&);

	//if we encounter ANYTHING that is not a building block of our program, we automatically
	//return only 1 text token of type ERROR, that contains the whole text of the input 
	void tokenize_input(std::vector<Token>&);
};

#endif // !LEXER_HEADER


