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
		AST_Node* tree;

		if (current_token.tag == Type::END)
		{
			tree = nullptr;
			eat(TokenType::END);
		}
		else if (current_token.tag == Type::STRING)
		{
			tree = new String(current_token.string_v);
			eat(Type::STRING);
			eat(Type::END);
		}
		else
		{
			tree = COND_expr();
			eat(Type::END);
		}

		return tree;
	}

	AST_Node* COND_expr()
	{
		AST_Node* expr = OR_expr();

		//if this is actually the conditional expression for IF function
		if (current_token.tag == Type::IF)
		{
			eat(Type::IF);
			AST_Node* then_expr = OR_expr();
			eat(Type::COLON);
			AST_Node* else_expr = OR_expr();
			return new Conditional_Operator(expr, then_expr, else_expr);
		}

		return expr;
	}

	AST_Node* OR_expr()
	{
		AST_Node* result = AND_expr();
		if (current_token.tag == Type::OR)
		{
			eat(Type::OR);
			result = new Binary_Operator(Type::OR, result, OR_expr());
		}

		return result;
	}

	AST_Node* AND_expr()
	{
		AST_Node* result = NOT_expr();
		if (current_token.tag == Type::AND)
		{
			eat(Type::AND);
			result = new Binary_Operator(Type::AND, result, AND_expr());
		}

		return result;
	}

	AST_Node* NOT_expr()
	{

		if (current_token.tag == Type::NOT)
		{
			eat(Type::NOT);
			return new Unary_Operator(Type::NOT, COMP_expr());
		}

		return COMP_expr();
	}
	AST_Node* COMP_expr()
	{

		AST_Node* expr = ADD_expr();

		switch (current_token.tag)
		{
		case Type::EQ:
			eat(Type::EQ);
			expr = new Binary_Operator(Type::EQ, expr, COMP_expr());
			break;
		case Type::NEQ:
			eat(Type::NEQ);
			expr = new Binary_Operator(Type::NEQ, expr, COMP_expr());
			break;
		case Type::LESS:
			eat(Type::LESS);
			expr = new Binary_Operator(Type::LESS, expr, COMP_expr());
			break;
		case Type::MORE:
			eat(Type::MORE);
			expr = new Binary_Operator(Type::MORE, expr, COMP_expr());
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
		case Type::PLUS:
			eat(Type::PLUS);
			expr = new Binary_Operator(Type::PLUS, expr, ADD_expr());
			break;
		case Type::MINUS:
			eat(Type::MINUS);
			expr = new Binary_Operator(Type::MINUS, expr, ADD_expr());
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
		case Type::MUL:
			eat(Type::MUL);
			expr = new Binary_Operator(Type::MUL, expr, MULT_expr());
			break;
		case Type::DIV:
			eat(Type::DIV);
			expr = new Binary_Operator(Type::DIV, expr, MULT_expr());
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
		case Type::PLUS:
			eat(Type::PLUS);
			expr = new Unary_Operator(Type::PLUS, Value());
			break;
		case Type::MINUS:
			eat(Type::MINUS);
			expr = new Unary_Operator(Type::MINUS, Value());
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

		case Type::LPAREN:
			eat(Type::LPAREN);
			result = COND_expr();
			eat(Type::RPAREN);
			break;
		case Type::X_AXIS:
		{
			AST_Node* x, * y;

			eat(Type::X_AXIS);
			if (current_token.tag == Type::SQ_LPAREN)
			{
				eat(Type::SQ_LPAREN);
				x = COND_expr();
				eat(Type::SQ_RPAREN);
				eat(Type::Y_AXIS);
				eat(Type::SQ_LPAREN);
				y = COND_expr();
				eat(Type::SQ_RPAREN);
			}
			else
			{
				x = COND_expr();
				eat(Type::Y_AXIS);
				y = COND_expr();
			}

			result = new JUMP_Node(x, y);

			break;
		}
		case Type::SUM:
		{
			AST_Node* x1, * x2, * y1, * y2;

			eat(Type::SUM);
			eat(Type::SQ_LPAREN);
			x1 = COND_expr();
			eat(Type::COLON);
			x2 = COND_expr();
			eat(Type::SQ_RPAREN);
			eat(Type::SQ_LPAREN);
			y1 = COND_expr();
			eat(Type::COLON);
			y2 = COND_expr();
			eat(Type::SQ_RPAREN);

			result = new Table_Func(Type::SUM, x1, x2, y1, y2);
			break;
		}
		case Type::COUNT:
		{
			AST_Node* x1, * x2, * y1, * y2;

			eat(Type::COUNT);
			eat(Type::SQ_LPAREN);
			x1 = COND_expr();
			eat(Type::COLON);
			x2 = COND_expr();
			eat(Type::SQ_RPAREN);
			eat(Type::SQ_LPAREN);
			y1 = COND_expr();
			eat(Type::COLON);
			y2 = COND_expr();
			eat(Type::SQ_RPAREN);

			result = new Table_Func(Type::COUNT, x1, x2, y1, y2);
			break;
		}
		case Type::FLOAT:
			result = new Float(current_token.float_v);
			eat(Type::FLOAT);
			break;
		case Type::INT:
			result = new Integer(current_token.int_v);
			eat(Type::INT);
			break;
		default:
			throw std::invalid_argument("if the analyzer did it's job you shouldnt be able to reach here");
			break;
		}

		return result;
	}

public:
	Parser(const string& s_input) : pos(0)
	{
		Lexer(s_input).tokenize_input(t_input);

		//if the input doesnt follow the syntax, we simply create text node
		if (!SyntaxAnalyzer(t_input).correct())
		{
			t_input.clear();
			Token text_token;
			text_token.tag = Type::STRING;
			text_token.string_v = s_input;

			t_input.push_back(text_token);
			t_input.push_back(Token{ Type::END });
		}

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