#include "SyntaxAnalyzer.h"

void SyntaxAnalyzer::eat(TokenType input_type)
{
	if (current_token.tag == input_type)
	{
		//the last index where there is something is the size-1, if we are there no need to increment more
		if (pos < t_input.size() - 1)
		{
			++pos;
			current_token = t_input[pos];
		}
	}
	else
	{
		throw std::runtime_error(
			">PARSER error: At token index(" + std::to_string(pos) + ") expected type(" +
			Token{ input_type }.toString() +
			") but got type(" +
			current_token.toString() +
			") instead"
		);
	}
}


void SyntaxAnalyzer::cell()
{
	//std::cout << "cell called!\n";

	if (current_token.tag == TokenType::END)
	{
		eat(TokenType::END);
		return;
	}
	else if (current_token.tag == TokenType::STRING)
	{
		eat(TokenType::STRING);
		eat(TokenType::END);
		return;
	}
	else if (current_token.tag == TokenType::ERROR)
	{
		throw std::runtime_error(current_token.string_v);

		return;
	}
	else
	{
		COND_expr();
		eat(TokenType::END);
		return;
	}

}
void SyntaxAnalyzer::COND_expr()
{

	//std::cout << "cond_expr called!\n";

	if (current_token.tag == TokenType::IF)
	{

		eat(TokenType::IF);
		eat(TokenType::LPAREN);
		OR_expr(); //condition
		eat(TokenType::COMMA);
		OR_expr(); //then
		eat(TokenType::COMMA);
		OR_expr(); //else
		eat(TokenType::RPAREN);
	}
	else
	{
		OR_expr();
	}
}
void SyntaxAnalyzer::OR_expr()
{
	//std::cout << "or_expr called!\n";

	AND_expr();
	if (current_token.tag == TokenType::OR)
	{
		eat(TokenType::OR);
		OR_expr();
	}
}
void SyntaxAnalyzer::AND_expr()
{
	//std::cout << "and_expr called!\n";

	NOT_expr();

	if (current_token.tag == TokenType::AND)
	{
		eat(TokenType::AND);
		AND_expr();
	}
}

void SyntaxAnalyzer::NOT_expr()
{
	//std::cout << "not_expr called!\n";

	if (current_token.tag == TokenType::NOT)
	{
		eat(TokenType::NOT);
	}

	COMP_expr();
}

void SyntaxAnalyzer::COMP_expr()
{
	//std::cout << "comp_expr called!\n";

	ADD_expr();

	switch (current_token.tag)
	{
	case TokenType::EQ:
		eat(TokenType::EQ);
		COMP_expr();
		break;
	case TokenType::NEQ:
		eat(TokenType::NEQ);
		COMP_expr();
		break;
	case TokenType::LESS:
		eat(TokenType::LESS);
		COMP_expr();
		break;
	case TokenType::MORE:
		eat(TokenType::MORE);
		COMP_expr();
		break;
	default:
		break;
	}
}

void SyntaxAnalyzer::ADD_expr()
{
	//std::cout << "add_expr called!\n";

	MULT_expr();

	switch (current_token.tag)
	{
	case TokenType::PLUS:
		eat(TokenType::PLUS);
		ADD_expr();
		break;
	case TokenType::MINUS:
		eat(TokenType::MINUS);
		ADD_expr();
		break;
	default:
		break;
	}
}

void SyntaxAnalyzer::MULT_expr()
{
	//std::cout << "mult_expr called!\n";
	NEGATE_expr();

	switch (current_token.tag)
	{
	case TokenType::MUL:
		eat(TokenType::MUL);
		MULT_expr();
		break;
	case TokenType::DIV:
		eat(TokenType::DIV);
		MULT_expr();
		break;
	default:
		break;
	}
}

void SyntaxAnalyzer::NEGATE_expr()
{
	//std::cout << "negate_expr called!\n";
	if (current_token.tag == TokenType::MINUS) eat(TokenType::MINUS);
	else if (current_token.tag == TokenType::PLUS) eat(TokenType::PLUS);

	Value();
}

void SyntaxAnalyzer::Value()
{
	//std::cout << "val_expr called!\n";

	switch (current_token.tag)
	{

	case TokenType::LPAREN:
		eat(TokenType::LPAREN);
		COND_expr();
		eat(TokenType::RPAREN);

		break;
	case TokenType::X_AXIS:
	{
		eat(TokenType::X_AXIS);
		if (current_token.tag == TokenType::SQ_LPAREN)
		{
			eat(TokenType::SQ_LPAREN);
			COND_expr();
			eat(TokenType::SQ_RPAREN);

			eat(TokenType::Y_AXIS);

			eat(TokenType::SQ_LPAREN);
			COND_expr();
			eat(TokenType::SQ_RPAREN);
		}
		else
		{
			COND_expr();
			eat(TokenType::Y_AXIS);
			COND_expr();
		}

		break;
	}
	case TokenType::SUM:
	{
		eat(TokenType::SUM);

		eat(TokenType::SQ_LPAREN);
		COND_expr();
		eat(TokenType::COLON);
		COND_expr();
		eat(TokenType::SQ_RPAREN);

		eat(TokenType::SQ_LPAREN);
		COND_expr();
		eat(TokenType::COLON);
		COND_expr();
		eat(TokenType::SQ_RPAREN);

		break;
	}
	case TokenType::COUNT:
	{
		eat(TokenType::COUNT);

		eat(TokenType::SQ_LPAREN);
		COND_expr();
		eat(TokenType::COLON);
		COND_expr();
		eat(TokenType::SQ_RPAREN);

		eat(TokenType::SQ_LPAREN);
		COND_expr();
		eat(TokenType::COLON);
		COND_expr();
		eat(TokenType::SQ_RPAREN);

		break;
	}
	case TokenType::FLOAT:
		eat(TokenType::FLOAT);
		break;
	case TokenType::INT:
		eat(TokenType::INT);
		break;
	case TokenType::STRING:
		eat(TokenType::STRING);
		break;
	default:
		throw std::runtime_error(
			">PARSER error: At token index(" + std::to_string(pos) + ") expected type: " +
			"integer, string, float, sum, count, x_axis or lparen, " +
			"but got type(" +
			current_token.toString() +
			") instead"
		);
		break;
	}
}

SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& v) : pos(0), t_input(v)
{

	current_token = t_input[pos];
}

bool SyntaxAnalyzer::correct()
{
	try
	{
		cell();
		return true;
	}
	catch (std::runtime_error& re)
	{
		std::cout << re.what();
		return false;
	}

}
