#include "Lexer.h"

void Lexer::advance()
{
	pos += 1;
	if (pos < text.length())
		current_char = text[pos];
	else
		current_char = '\0';
}

char Lexer::peek(int steps = 1)
{
	int peek_pos = pos + steps;

	if (peek_pos < text.length())
		return text[peek_pos];
	else
		return '\0';
}

Token Lexer::number_token()
{
	float number = 0;

	bool is_float = false;
	while (current_char && isdigit(current_char))
	{
		number = number * 10 + (int)(current_char - '0');
		advance();
	}

	if (current_char == '.')
	{
		is_float = true;
		advance();

		float divisor = 10;
		while (current_char && isdigit(current_char))
		{
			number = number + (float)(int)(current_char - '0') / divisor;
			divisor *= 10;
			advance();
		}

	}

	Token result;
	if (is_float)
	{
		result.tag = TokenType::FLOAT;
		result.float_v = number;
	}
	else
	{
		result.tag = TokenType::INT;
		result.int_v = (int)number;
	}
	return result;
}

Token Lexer::string_token()
{
	Token result;

	advance(); //skip opening quotes

	int initial_pos = pos, length = 0;
	while (current_char && current_char != '"')
	{
		advance();
		++length;
	}

	//we have reached end of input because we didnt encounter closing quotes
	if (!current_char)
	{
		throw std::runtime_error("There are opening quotes at pos: " + std::to_string(pos) + " but they are not closed anywhere");
	}

	advance(); //skip closing quotes

	result.tag = TokenType::STRING;
	result.string_v = text.substr(initial_pos, length);

	return result;
}

Token Lexer::get_next_token()
{
	//std::cout << "current char:" << current_char << "\n";

	while (current_char == ' ')
	{
		advance();
	}

	if (current_char == '\0')
	{
		return Token{ TokenType::END };
	}

	if (current_char == '(')
	{
		advance();
		return Token{ TokenType::LPAREN };
	}

	if (current_char == ')')
	{
		advance();
		return Token{ TokenType::RPAREN };
	}

	if (isdigit(current_char))
	{
		return number_token();
	}

	//if the quotation is incorrect we return OTHER token, which means something is wrong with the input
	if (current_char == '\"')
	{
		try {
			return string_token();
		}
		catch (std::runtime_error& re)
		{
			return Token{ TokenType::OTHER, -1, -1, re.what() };
		}
	}

	if (current_char == '=' && peek() == '=')
	{
		advance();
		advance();
		return Token{ TokenType::EQ };
	}

	if (current_char == '!' && peek() == '=')
	{
		advance();
		advance();
		return Token{ TokenType::NEQ };
	}

	if (current_char == '|' && peek() == '|')
	{
		advance();
		advance();
		return Token{ TokenType::OR };
	}

	if (current_char == '&' && peek() == '&')
	{
		advance();
		advance();
		return Token{ TokenType::AND };
	}

	if (current_char == '!')
	{
		advance();
		return Token{ TokenType::NOT };
	}

	if (current_char == '<')
	{
		advance();
		return Token{ TokenType::LESS };
	}

	if (current_char == '>')
	{
		advance();
		return Token{ TokenType::MORE };
	}


	if (current_char == '+')
	{
		advance();
		return Token{ TokenType::PLUS };
	}

	if (current_char == '-')
	{
		advance();
		return Token{ TokenType::MINUS };
	}

	if (current_char == '*')
	{
		advance();
		return Token{ TokenType::MUL };
	}

	if (current_char == '/')
	{
		advance();
		return Token{ TokenType::DIV };
	}

	if (current_char == 'R')
	{
		advance();
		return Token{ TokenType::X_AXIS };
	}

	if (current_char == 'C')
	{
		advance();
		return Token{ TokenType::Y_AXIS };
	}

	if (current_char == '[')
	{
		advance();
		return Token{ TokenType::SQ_LPAREN };
	}

	if (current_char == ']')
	{
		advance();
		return Token{ TokenType::SQ_RPAREN };
	}

	if (current_char == ':')
	{
		advance();
		return Token{ TokenType::COLON };
	}

	if (current_char == 'I' && peek() == 'F')
	{
		advance();
		advance();
		return Token{ TokenType::IF };
	}

	if (current_char == ',')
	{
		advance();
		return Token{ TokenType::COMMA };
	}

	if (current_char == 'c' && peek(1) == 'o'
		&& peek(2) == 'u' && peek(3) == 'n' && peek(4) == 't')
	{
		advance();
		advance();
		advance();
		advance();
		advance();

		return Token{ TokenType::COUNT };
	}

	if (current_char == 's' && peek(1) == 'u'
		&& peek(2) == 'm')
	{
		advance();
		advance();
		advance();

		return Token{ TokenType::SUM };
	}

	return Token{ TokenType::OTHER, -1, -1, "Unknown symbol \"" + std::string(1,current_char) + " at pos: " + std::to_string(pos) + ".\n" };
}

Lexer::Lexer(const std::string& input) : text(input), pos(0)
{
	current_char = text[pos];
}


//if we encounter ANYTHING that is not a building block of our program, we automatically
//or no closing quotation we throw
void Lexer::tokenize_input(std::vector<Token>& tokenized)
{
	Token curr_token = get_next_token();
	while (curr_token.tag != TokenType::END)
	{
		//std::cout << curr_token.toString() << "\n";

		if (curr_token.tag == TokenType::OTHER)
		{
			//std::cout << "unknown command, string until now: " << text.substr(0, pos) << "\n";

			tokenized.clear();

			throw std::runtime_error(">LEXER\nerror:" + curr_token.string_v + "\ninput: " + text);
			break;
		}

		tokenized.push_back(curr_token);
		curr_token = get_next_token();
	}

	tokenized.push_back(Token{ TokenType::END });
}