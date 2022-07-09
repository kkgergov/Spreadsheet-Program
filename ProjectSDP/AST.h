#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;

//needed for the tokens
enum Type
{
	//primitive
	INT,
	FLOAT,
	STRING,

	//bool types
	EQ,
	NEQ,
	LESS,
	MORE,
	OR,
	AND,
	NOT,

	LPAREN,
	RPAREN,

	//arithmetic types
	PLUS,
	MINUS,
	MUL,
	DIV,

	//go to other cell
	SQ_LPAREN,
	SQ_RPAREN,
	X_AXIS,
	Y_AXIS,

	//if statement
	IF,
	COLON,

	SUM,
	COUNT,
	//if something is of this type, then the while expr is just text and we dont build expr tree, but a text node
	OTHER,
	//every tokenized input should have this at the end
	END
};


//the AST node visitor
class Visitor;


//base class for all the different AST nodes
class AST_Node
{
public:
	virtual void accept(Visitor& v) = 0;
};

//every expression has a special type of root node
class ROOT_Node : public AST_Node
{
public:
	AST_Node* Tree_expr;
	ROOT_Node(AST_Node* tree = nullptr)
		: Tree_expr(tree) {};

	void accept(Visitor& v);
};

//node that redirects to root node of another expression
class JUMP_Node : public AST_Node
{
public:
	//coords of expression somewhere else in the table that we want to evaluate
	//the "jump" from one tree to another happens only during the interpretation part
	AST_Node* x_coord, * y_coord;

	JUMP_Node(AST_Node *i = nullptr, AST_Node *j = nullptr)
		: x_coord(i), y_coord(j) {};

	void accept(Visitor& v);
};


//node that calls a SUM or CNT function over region of the table
class Table_Func : public AST_Node
{
public:
	Type type;
	AST_Node* xMIN, * xMAX, *yMIN, *yMAX;

	Table_Func(Type t, AST_Node* x1, AST_Node* x2, AST_Node* y1, AST_Node* y2)
		: type(t), xMIN(x1), xMAX(x2), yMIN(y1), yMAX(y2) {};

	void accept(Visitor& v);
};

//this is the node for the if operator
class Conditional_Operator : public AST_Node
{
public:
	AST_Node* Condition_expr, * Then_expr, * Else_expr;
	Conditional_Operator(AST_Node* first, AST_Node* second, AST_Node* third)
		: Condition_expr(first), Then_expr(second), Else_expr(third) {};

	void accept(Visitor& v);
};


//this is the node for all binary boolean or arithmetic operators
class Binary_Operator : public AST_Node
{
public:
	AST_Node* Left_expr, * Right_expr;
	Type op_name;
	Binary_Operator(Type t, AST_Node* l, AST_Node* r)
		: Left_expr(l), Right_expr(r), op_name(t) {};

	void accept(Visitor& v);
};


//this is the node for all unary boolean or arithmetic operators
class Unary_Operator : public AST_Node
{
public:
	AST_Node* Right_expr;
	Type op_name;
	Unary_Operator(Type t, AST_Node* r)
		: Right_expr(r), op_name(t) {};

	void accept(Visitor& v);
};



//Primitive types
class Float : public AST_Node
{
public:
	float value;
	Float(float v)
		: value(v) {};

	void accept(Visitor& v);
};

class Integer : public AST_Node
{
public:
	int value;
	Integer(int v)
		: value(v) {};

	void accept(Visitor& v);
};

class String : public AST_Node
{
public:
	string value;
	String(string v)
		: value(v) {};

	void accept(Visitor& v);
};



class Visitor
{
public:
	virtual void visit(ROOT_Node* ast) = 0;
	virtual void visit(JUMP_Node* ast) = 0;
	virtual void visit(Table_Func* ast) = 0;
	virtual void visit(Conditional_Operator* ast) = 0;
	virtual void visit(Binary_Operator* ast) = 0;
	virtual void visit(Unary_Operator* ast) = 0;
	virtual void visit(Float* ast) = 0;
	virtual void visit(Integer* ast) = 0;
	virtual void visit(String* ast) = 0;
};

void ROOT_Node::accept(Visitor& v)
{
	v.visit(this);
}

void JUMP_Node::accept(Visitor& v)
{
	v.visit(this);
}

void Table_Func::accept(Visitor& v)
{
	v.visit(this);
}

void Conditional_Operator::accept(Visitor& v)
{
	v.visit(this);
}

void Binary_Operator::accept(Visitor& v)
{
	v.visit(this);
}

void Unary_Operator::accept(Visitor& v)
{
	v.visit(this);
}

void Float::accept(Visitor& v)
{
	v.visit(this);
}

void Integer::accept(Visitor& v)
{
	v.visit(this);
}

void String::accept(Visitor& v)
{
	v.visit(this);
}


struct Token
{
	Type tag= Type::OTHER;

	//i wish i had the knowledge to make union
	//with string, but alas... :(
	int int_v = 0;
	float float_v = 0.;
	string string_v = "";

	void printToken() const
	{
		switch (tag)
		{
		case Type::INT:
			cout << "int(" << int_v << ")";
			break;
		case Type::FLOAT:
			cout << "float(" << float_v << ")";
			break;
		case Type::STRING:
			cout << "string(" << string_v << ")";
			break;
		case Type::EQ:
			cout << "eq";
			break;
		case Type::NEQ:
			cout << "neq";
			break;
		case Type::LESS:
			cout << "less";
			break;
		case Type::MORE:
			cout << "more";
			break;
		case Type::OR:
			cout << "or";
			break;
		case Type::AND:
			cout << "and";
			break;
		case Type::NOT:
			cout << "not";
			break;
		case Type::LPAREN:
			cout << "lparen";
			break;
		case Type::RPAREN:
			cout << "rparen";
			break;
		case Type::PLUS:
			cout << "plus";
			break;
		case Type::MINUS:
			cout << "minus";
			break;
		case Type::MUL:
			cout << "mul";
			break;
		case Type::DIV:
			cout << "div";
			break;
		case Type::SQ_LPAREN:
			cout << "sqlparen";
			break;
		case Type::SQ_RPAREN:
			cout << "sqrparen";
			break;
		case Type::X_AXIS:
			cout << "x_axis";
			break;
		case Type::Y_AXIS:
			cout << "y_axis";
			break;
		case Type::IF:
			cout << "if";
			break;
		case Type::COLON:
			cout << "colon";
			break;
		case Type::SUM:
			cout << "sum";
			break;
		case Type::COUNT:
			cout << "count";
			break;
		case Type::OTHER:
			cout << "other";
			break;
		case Type::END:
			cout << "end";
			break;
		default:
			break;
		}
	}
};


//the lexer class is lexical analyzer as well, because if we encounter something else
//other than the building blocks of our program, we just return a string token with end token
class Lexer
{
private:
	string text;

	int pos;
	char current_char;

	void advance()
	{
		pos += 1;
		if (pos < text.length())
			current_char = text[pos];
		else 
			current_char = '\0';
	}

	char peek(int steps = 1)
	{
		int peek_pos = pos + steps;

		if (peek_pos < text.length())
			return text[peek_pos];
		else
			return '\0';
	}

	Token number()
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
			result.tag = Type::FLOAT;
			result.float_v = number;
		}
		else
		{
			result.tag = Type::INT;
			result.int_v = (int)number;
		}
		return result;
	}

	Token get_next_token()
	{
		while (current_char)
		{
			while (current_char == ' ')
				advance();

			if (!current_char) return Token{ Type::END };

			if (current_char == '(')
			{
				advance();
				return Token{ Type::LPAREN };
			}

			if (current_char == ')')
			{
				advance();
				return Token{ Type::RPAREN };
			}

			if (isdigit(current_char))
			{
				return number();
			}

			if (current_char == '=' && peek() == '=')
			{
				advance();
				advance();
				return Token{ Type::EQ };
			}

			if (current_char == '!' && peek() == '=')
			{
				advance();
				advance();
				return Token{ Type::NEQ };
			}

			if (current_char == '|' && peek() == '|')
			{
				advance();
				advance();
				return Token{ Type::OR };
			}

			if (current_char == '&' && peek() == '&')
			{
				advance();
				advance();
				return Token{ Type::AND };
			}

			if (current_char == '!')
			{
				advance();
				return Token{ Type::NOT };
			}

			if (current_char == '<')
			{
				advance();
				return Token{ Type::LESS };
			}

			if (current_char == '>')
			{
				advance();
				return Token{ Type::MORE };
			}


			if (current_char == '+')
			{
				advance();
				return Token{ Type::PLUS };
			}

			if (current_char == '-')
			{
				advance();
				return Token{ Type::MINUS };
			}

			if (current_char == '*')
			{
				advance();
				return Token{ Type::MUL };
			}

			if (current_char == '/')
			{
				advance();
				return Token{ Type::DIV };
			}

			if (current_char == 'R')
			{
				advance();
				return Token{ Type::X_AXIS };
			}

			if (current_char == 'C')
			{
				advance();
				return Token{ Type::Y_AXIS };
			}

			if (current_char == '[')
			{
				advance();
				return Token{ Type::SQ_LPAREN };
			}

			if (current_char == ']')
			{
				advance();
				return Token{ Type::SQ_RPAREN };
			}

			if (current_char == '?')
			{
				advance();
				return Token{ Type::IF };
			}

			if (current_char == ':')
			{
				advance();
				return Token{ Type::COLON };
			}

			if (current_char == 'c' && peek(1) == 'o'
				&& peek(2) == 'u' && peek(3) == 'n' && peek(4) == 't')
			{
				advance();
				advance();
				advance();
				advance();
				advance();

				return Token{ Type::COUNT };
			}

			if (current_char == 's' && peek(1) == 'u'
				&& peek(2) == 'm')
			{
				advance();
				advance();
				advance();

				return Token{ Type::SUM };
			}

			return Token{ Type::OTHER };
		}

		return Token{ Type::END };
	}

public:
	Lexer(const string& input) : text(input), pos(0)
	{
		current_char = text[pos];
	}


	//if we encounter ANYTHING that is not a building block of our program, we automatically
	//return only 1 text token, that contains the whole text 
	void tokenize_input(vector<Token>& tokenized)
	{
		Token curr_token = get_next_token();
		while (curr_token.tag != Type::END)
		{
			if (curr_token.tag == Type::OTHER)
			{
				tokenized.clear();
				Token text_token;
				text_token.tag = Type::STRING;
				text_token.string_v = text;
				tokenized.push_back(text_token);
				break;
			}

			tokenized.push_back(curr_token);
			curr_token = get_next_token();
		}
		tokenized.push_back(Token{ Type::END });
	}
};

//analyzes if syntax of the tokenized input follows the allowed grammar
class SyntaxAnalyzer
{
private:
	vector<Token> t_input;
	int pos;
	Token current_token;

	//in the real parser we wont be needing the check
	bool eat(Type input_type)
	{
		if (current_token.tag == input_type)
		{
			if(pos < t_input.size() - 1)
				current_token = t_input[++pos];
			return true;
		}
		else
		{
			cout << "At token index(" << pos << ") expected type(";
			Token wantedToken;
			wantedToken.tag = input_type;
			wantedToken.printToken();
			cout << ") but got type(";
			current_token.printToken();
			cout << ") instead\n";
			return false;
		}
	}


	bool cell()
	{
		if (current_token.tag == Type::END)
		{
			return eat(Type::END);
		}
		else if (current_token.tag == Type::STRING)
		{
			return eat(Type::STRING) && eat(Type::END);
		}
		else
		{
			return COND_expr() && eat(Type::END);
		}

	}
	bool COND_expr()
	{
		//cout << "cond_expr called!\n";
		bool result = OR_expr();
		if (current_token.tag == Type::IF)
		{
			eat(Type::IF);
			result = result && OR_expr();
			result = result && eat(Type::COLON);
			result = result && OR_expr();
		}
		
		return result;
	}
	bool OR_expr()
	{
		//cout << "or_expr called!\n";

		bool result = AND_expr();
		if (current_token.tag == Type::OR)
		{
			eat(Type::OR);
			result = result && OR_expr();
		}

		return result;
	}
	bool AND_expr()
	{
		//cout << "and_expr called!\n";

		bool result = NOT_expr();

		if (current_token.tag == Type::AND)
		{
			eat(Type::AND);
			result = result && AND_expr();
		}

		return result;
	}

	bool NOT_expr()
	{
		//cout << "not_expr called!\n";

		if (current_token.tag == Type::NOT)
			eat(Type::NOT);

		return COMP_expr();
	}

	bool COMP_expr()
	{
		//cout << "comp_expr called!\n";

		bool result = ADD_expr();

		switch (current_token.tag)
		{
		case Type::EQ:
			eat(Type::EQ);
			result = result && COMP_expr();
			break;
		case Type::NEQ:
			eat(Type::NEQ);
			result = result && COMP_expr();
			break;
		case Type::LESS:
			eat(Type::LESS);
			result = result && COMP_expr();
			break;
		case Type::MORE:
			eat(Type::MORE);
			result = result && COMP_expr();
			break;
		default:
			break;
		}

		return result;
	}

	bool ADD_expr()
	{
		//cout << "add_expr called!\n";

		bool result = MULT_expr();

		switch (current_token.tag)
		{
		case Type::PLUS:
			eat(Type::PLUS);
			result = result && ADD_expr();
			break;
		case Type::MINUS:
			eat(Type::MINUS);
			result = result && ADD_expr();
			break;
		default:
			break;
		}

		return result;
	}

	bool MULT_expr()
	{
		//cout << "mult_expr called!\n";
		bool result = NEGATE_expr();

		switch (current_token.tag)
		{
		case Type::MUL:
			eat(Type::MUL);
			result = result && MULT_expr();
			break;
		case Type::DIV:
			eat(Type::DIV);
			result = result && MULT_expr();
			break;
		default:
			break;
		}

		return result;
	}

	bool NEGATE_expr()
	{
		//cout << "negate_expr called!\n";
		if (current_token.tag == Type::MINUS) eat(Type::MINUS);
		else if (current_token.tag == Type::PLUS) eat(Type::PLUS);

		return Value();
	}

	bool Value()
	{
		//cout << "val_expr called!\n";

		switch (current_token.tag)
		{

		case Type::LPAREN:
			return eat(Type::LPAREN) && COND_expr() && eat(Type::RPAREN);
			break;
		case Type::X_AXIS:
		{
			eat(Type::X_AXIS);
			if (current_token.tag == Type::SQ_LPAREN)
			{
				return eat(Type::SQ_LPAREN) &&
					COND_expr() &&
					eat(Type::SQ_RPAREN) &&
					eat(Type::Y_AXIS) &&
					eat(Type::SQ_LPAREN) &&
					COND_expr() &&
					eat(Type::SQ_RPAREN);
			}
			else
			{
				return COND_expr() &&
					eat(Type::Y_AXIS) &&
					COND_expr();
			}
			break;
		}
		case Type::SUM:
		{
			return eat(Type::SUM) &&
				eat(Type::SQ_LPAREN) &&
				COND_expr() &&
				eat(Type::COLON) &&
				COND_expr() &&
				eat(Type::SQ_RPAREN) &&
				eat(Type::SQ_LPAREN) &&
				COND_expr() &&
				eat(Type::COLON) &&
				COND_expr() &&
				eat(Type::SQ_RPAREN);
			break;
		}
		case Type::COUNT:
		{
			return eat(Type::COUNT) &&
				eat(Type::SQ_LPAREN) &&
				COND_expr() &&
				eat(Type::COLON) &&
				COND_expr() &&
				eat(Type::SQ_RPAREN) &&
				eat(Type::SQ_LPAREN) &&
				COND_expr() &&
				eat(Type::COLON) &&
				COND_expr() &&
				eat(Type::SQ_RPAREN);
			break;
		}
		case Type::FLOAT:
			return eat(Type::FLOAT);
			break;
		case Type::INT:
			return eat(Type::INT);
			break;
		default:
			eat(Type::OTHER);
			return false;
			break;
		}
	}

public:
	SyntaxAnalyzer(const vector<Token>& v) : pos(0), t_input(v)
	{

		current_token = t_input[pos];
	}

	bool correct()
	{
		return cell();
	}
};

class Parser
{
private:

	vector<Token> t_input;
	int pos;
	Token current_token;

	void eat(Type t)
	{
		if (pos < t_input.size() - 1)
			current_token = t_input[++pos];
	}

	//we create the actual tree from here
	AST_Node* cell()
	{
		AST_Node* tree;

		if (current_token.tag == Type::END)
		{
			tree = nullptr;
			eat(Type::END);
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
			expr = new Binary_Operator(Type::EQ, expr,COMP_expr());
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
			AST_Node* x, *y;

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
				y =	COND_expr();
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
	void print(AST_Node *tree)
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

	AST_Node *root;
};