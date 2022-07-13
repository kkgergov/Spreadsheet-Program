#include "Parser.h"

void Parser::eat(TokenType t)
{
	if (pos < t_input.size() - 1)
	{
		++pos;
		current_token = t_input[pos];
	}
}

AST_Node* Parser::cell()
{
	if (current_token.tag == TokenType::END)
	{
		eat(TokenType::END);

		return nullptr;
	}
	else
	{
		AST_Node* result = COND_expr();
		eat(TokenType::END);

		return result;
	}
}

AST_Node* Parser::COND_expr()
{
	if (current_token.tag == TokenType::IF)
	{

		eat(TokenType::IF);
		eat(TokenType::LPAREN);
		AST_Node* Cond = OR_expr(); //condition
		eat(TokenType::COMMA);
		AST_Node* Then = OR_expr(); //then
		eat(TokenType::COMMA);
		AST_Node* Else = OR_expr(); //else
		eat(TokenType::RPAREN);

		return new Conditional_Operator(Cond, Then, Else);
	}
	else
	{
		return OR_expr();
	}
}

AST_Node* Parser::OR_expr()
{
	AST_Node* result = AND_expr();
	if (current_token.tag == TokenType::OR)
	{
		eat(TokenType::OR);
		result = new Binary_Operator(TokenType::OR, result, OR_expr());
	}

	return result;
}

AST_Node* Parser::AND_expr()
{
	AST_Node* result = NOT_expr();
	if (current_token.tag == TokenType::AND)
	{
		eat(TokenType::AND);
		result = new Binary_Operator(TokenType::AND, result, AND_expr());
	}

	return result;
}

AST_Node* Parser::NOT_expr()
{

	if (current_token.tag == TokenType::NOT)
	{
		eat(TokenType::NOT);
		return new Unary_Operator(TokenType::NOT, COMP_expr());
	}

	return COMP_expr();
}
AST_Node* Parser::COMP_expr()
{

	AST_Node* expr = ADD_expr();

	switch (current_token.tag)
	{
	case TokenType::EQ:
		eat(TokenType::EQ);
		expr = new Binary_Operator(TokenType::EQ, expr, COMP_expr());
		break;
	case TokenType::NEQ:
		eat(TokenType::NEQ);
		expr = new Binary_Operator(TokenType::NEQ, expr, COMP_expr());
		break;
	case TokenType::LESS:
		eat(TokenType::LESS);
		expr = new Binary_Operator(TokenType::LESS, expr, COMP_expr());
		break;
	case TokenType::MORE:
		eat(TokenType::MORE);
		expr = new Binary_Operator(TokenType::MORE, expr, COMP_expr());
		break;
	default:
		break;
	}

	return expr;
}

AST_Node* Parser::ADD_expr()
{
	AST_Node* expr = MULT_expr();

	switch (current_token.tag)
	{
	case TokenType::PLUS:
		eat(TokenType::PLUS);
		expr = new Binary_Operator(TokenType::PLUS, expr, ADD_expr());
		break;
	case TokenType::MINUS:
		eat(TokenType::MINUS);
		expr = new Binary_Operator(TokenType::MINUS, expr, ADD_expr());
		break;
	default:
		break;
	}

	return expr;
}

AST_Node* Parser::MULT_expr()
{
	AST_Node* expr = NEGATE_expr();

	switch (current_token.tag)
	{
	case TokenType::MUL:
		eat(TokenType::MUL);
		expr = new Binary_Operator(TokenType::MUL, expr, MULT_expr());
		break;
	case TokenType::DIV:
		eat(TokenType::DIV);
		expr = new Binary_Operator(TokenType::DIV, expr, MULT_expr());
		break;
	default:
		break;
	}

	return expr;
}

AST_Node* Parser::NEGATE_expr()
{

	AST_Node* expr;

	switch (current_token.tag)
	{
	case TokenType::PLUS:
		eat(TokenType::PLUS);
		expr = new Unary_Operator(TokenType::PLUS, Value());
		break;
	case TokenType::MINUS:
		eat(TokenType::MINUS);
		expr = new Unary_Operator(TokenType::MINUS, Value());
		break;
	default:
		expr = Value();
		break;
	}

	return expr;
}

AST_Node* Parser::Value()
{
	AST_Node* result;

	switch (current_token.tag)
	{

	case TokenType::LPAREN:
		eat(TokenType::LPAREN);
		result = COND_expr();
		eat(TokenType::RPAREN);
		break;
	case TokenType::X_AXIS:
	{
		TokenType adressing_type = TokenType::OTHER;
		AST_Node* x, * y;

		eat(TokenType::X_AXIS);
		if (current_token.tag == TokenType::SQ_LPAREN)
		{
			//we encountered brackets, thus the adressing type of the variable must be relative
			adressing_type = TokenType::RELATIVE;

			eat(TokenType::SQ_LPAREN);
			x = COND_expr();
			eat(TokenType::SQ_RPAREN);
			eat(TokenType::Y_AXIS);
			eat(TokenType::SQ_LPAREN);
			y = COND_expr();
			eat(TokenType::SQ_RPAREN);
		}
		else
		{
			//no brackets, so absolute adressation
			adressing_type = TokenType::ABSOLUTE;

			x = COND_expr();
			eat(TokenType::Y_AXIS);
			y = COND_expr();
		}

		result = new JUMP_Node(adressing_type, x, y);

		break;
	}
	case TokenType::SUM:
	{
		AST_Node* x1, * x2, * y1, * y2;

		eat(TokenType::SUM);

		eat(TokenType::SQ_LPAREN);
		x1 = COND_expr();
		eat(TokenType::COLON);
		x2 = COND_expr();
		eat(TokenType::SQ_RPAREN);

		eat(TokenType::SQ_LPAREN);
		y1 = COND_expr();
		eat(TokenType::COLON);
		y2 = COND_expr();
		eat(TokenType::SQ_RPAREN);

		result = new Table_Func(TokenType::SUM, x1, y1, x2, y2);

		break;
	}
	case TokenType::COUNT:
	{
		AST_Node* x1, * x2, * y1, * y2;

		eat(TokenType::COUNT);

		eat(TokenType::SQ_LPAREN);
		x1 = COND_expr();
		eat(TokenType::COLON);
		x2 = COND_expr();
		eat(TokenType::SQ_RPAREN);
		eat(TokenType::SQ_LPAREN);
		y1 = COND_expr();
		eat(TokenType::COLON);
		y2 = COND_expr();
		eat(TokenType::SQ_RPAREN);

		result = new Table_Func(TokenType::COUNT, x1, y1, x2, y2);
		break;
	}
	case TokenType::FLOAT:
		result = new Float(current_token.float_v);
		eat(TokenType::FLOAT);
		break;
	case TokenType::INT:
		result = new Integer(current_token.int_v);
		eat(TokenType::INT);
		break;
	case TokenType::STRING:
		result = new String(current_token.string_v);
		eat(TokenType::STRING);
		break;
	default:
		throw std::invalid_argument("if the analyzer did it's job you shouldnt be able to reach here");
		break;
	}

	return result;
}

Parser::Parser(const std::vector<Token>& v) : pos(0), t_input(v)
{
	current_token = t_input[pos];
}

AST_Node* Parser::parse()
{

	AST_Node* root = new ROOT_Node(cell());

	return root;
}
