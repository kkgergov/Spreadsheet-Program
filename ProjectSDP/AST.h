#pragma once

#include "TokenType.h"
#include "Token.h"

#include "AST_Nodes.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;


class Parser
{
private:

	vector<Token> t_input;
	int pos;
	Token current_token;

	void eat(TokenType t)
	{
		if (pos < t_input.size() - 1)
		{
			++pos;
			current_token = t_input[pos];
		}
	}

	//we create the actual tree from here
	AST_Node* cell()
	{
		if (current_token.tag == TokenType::END)
		{
			eat(TokenType::END);

			return nullptr;
		}
		else
		{
			AST_Node *result = COND_expr();
			eat(TokenType::END);

			return result;
		}
	}

	AST_Node* COND_expr()
	{
		if (current_token.tag == TokenType::IF)
		{

			eat(TokenType::IF);
			eat(TokenType::LPAREN);
			AST_Node *Cond = OR_expr(); //condition
			eat(TokenType::COMMA);
			AST_Node *Then = OR_expr(); //then
			eat(TokenType::COMMA);
			AST_Node *Else = OR_expr(); //else
			eat(TokenType::RPAREN);

			return new Conditional_Operator(Cond, Then, Else);
		}
		else
		{
			return OR_expr();
		}
	}

	AST_Node* OR_expr()
	{
		AST_Node* result = AND_expr();
		if (current_token.tag == TokenType::OR)
		{
			eat(TokenType::OR);
			result = new Binary_Operator(TokenType::OR, result, OR_expr());
		}

		return result;
	}

	AST_Node* AND_expr()
	{
		AST_Node* result = NOT_expr();
		if (current_token.tag == TokenType::AND)
		{
			eat(TokenType::AND);
			result = new Binary_Operator(TokenType::AND, result, AND_expr());
		}

		return result;
	}

	AST_Node* NOT_expr()
	{

		if (current_token.tag == TokenType::NOT)
		{
			eat(TokenType::NOT);
			return new Unary_Operator(TokenType::NOT, COMP_expr());
		}

		return COMP_expr();
	}
	AST_Node* COMP_expr()
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

	AST_Node* ADD_expr()
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

	AST_Node* MULT_expr()
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

	AST_Node* NEGATE_expr()
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

	AST_Node* Value()
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
			AST_Node* x, * y;

			eat(TokenType::X_AXIS);
			if (current_token.tag == TokenType::SQ_LPAREN)
			{
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
				x = COND_expr();
				eat(TokenType::Y_AXIS);
				y = COND_expr();
			}

			result = new JUMP_Node(x, y);

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

			result = new Table_Func(TokenType::SUM, x1, x2, y1, y2);
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

			result = new Table_Func(TokenType::COUNT, x1, x2, y1, y2);
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

public:
	Parser(const std::vector<Token>& v) : pos(0), t_input(v)
	{
		current_token = t_input[pos];
	}

	AST_Node* parse()
	{

		AST_Node* root = new ROOT_Node(cell());

		return root;
	}
};

//semantic analyzer here

class PrintNodes : public Visitor
{
private:
	int tabs;

	void visit(ROOT_Node* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "r_node\n";
		++tabs;
		ast->Tree_expr->accept(*this);
		--tabs;
	}
	void visit(JUMP_Node* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "j_node\n";
		++tabs;
		ast->x_coord->accept(*this);
		ast->y_coord->accept(*this);
		--tabs;
	}

	void visit(Table_Func* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "tf_node\n";
		++tabs;
		ast->xMIN->accept(*this);
		ast->xMAX->accept(*this);
		ast->yMIN->accept(*this);
		ast->yMAX->accept(*this);
		--tabs;
	}

	void visit(Conditional_Operator* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "cnd_op\n";
		++tabs;
		ast->Condition_expr->accept(*this);
		ast->Then_expr->accept(*this);
		ast->Else_expr->accept(*this);
		--tabs;
	}

	void visit(Binary_Operator* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "bin_op(";
		Token{ ast->op_name }.printToken();
		cout << ")\n";
		++tabs;
		ast->Left_expr->accept(*this);
		ast->Right_expr->accept(*this);
		--tabs;
	}
	void visit(Unary_Operator* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "un_op(";
		Token{ ast->op_name }.printToken();
		cout << ")\n";
		++tabs;
		ast->Right_expr->accept(*this);
		--tabs;
	}
	void visit(Float* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "float(" << ast->value << ")\n";
	}
	void visit(Integer* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "int(" << ast->value << ")\n";
	}
	void visit(String* ast)
	{
		for (int i = 0; i < tabs; ++i) cout << "\t";
		cout << "string(" << ast->value << ")\n";
	}
public:
	void print(AST_Node* tree)
	{
		tabs = 0;
		tree->accept(*this);
	}
};

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