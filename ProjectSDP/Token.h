#ifndef TOKEN_STRUCT
#define TOKEN_STRUCT

#include <string>
#include "TokenType.h"

struct Token
{
	TokenType tag;

	int int_v = 0;
	float float_v = 0.;
	std::string string_v = "";

	std::string toString() const
	{
		std::string result;

		switch (tag)
		{
		case TokenType::INT:
			result = "int(" + std::to_string(int_v) + ")";
			break;
		case TokenType::FLOAT:
			result = "float(" + std::to_string(float_v) + ")";
			break;
		case TokenType::STRING:
			result = "string(" + string_v + ")";
			break;
		case TokenType::EQ:
			result = "equals";
			break;
		case TokenType::NEQ:
			result = "not_equals";
			break;
		case TokenType::LESS:
			result = "less";
			break;
		case TokenType::MORE:
			result = "more";
			break;
		case TokenType::OR:
			result = "or";
			break;
		case TokenType::AND:
			result = "and";
			break;
		case TokenType::NOT:
			result = "not";
			break;
		case TokenType::LPAREN:
			result = "lparen";
			break;
		case TokenType::RPAREN:
			result = "rparen";
			break;
		case TokenType::PLUS:
			result = "plus";
			break;
		case TokenType::MINUS:
			result = "minus";
			break;
		case TokenType::MUL:
			result = "mul";
			break;
		case TokenType::DIV:
			result = "div";
			break;
		case TokenType::SQ_LPAREN:
			result = "sqlparen";
			break;
		case TokenType::SQ_RPAREN:
			result = "sqrparen";
			break;
		case TokenType::X_AXIS:
			result = "x_axis";
			break;
		case TokenType::Y_AXIS:
			result = "y_axis";
			break;
		case TokenType::IF:
			result = "if";
			break;
		case TokenType::COMMA:
			result = "comma";
			break;
		case TokenType::COLON:
			result = "colon";
			break;
		case TokenType::SUM:
			result = "sum";
			break;
		case TokenType::COUNT:
			result = "count";
			break;
		case TokenType::OTHER:
			result = "other";
			break;
		case TokenType::ERROR:
			result = "ERROR(" + string_v + ")";
			break;
		case TokenType::END:
			result = "end";
			break;
		default:
			break;
		}

		return result;
	}
};

#endif // !TOKEN_STRUCT
