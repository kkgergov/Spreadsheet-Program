#ifndef AST_NODES
#define AST_NODES

#include <string>
#include "TokenType.h"

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
	AST_Node* Expr;
	ROOT_Node(AST_Node* Expr = nullptr)
		: Expr(Expr) {};

	void accept(Visitor& v);
};


//node that redirects to root node of another expression
class JUMP_Node : public AST_Node
{
public:
	//coords of expression somewhere else in the table that we want to evaluate
	//the "jump" from one tree to another happens only during the interpretation part
	TokenType adressing_type;
	AST_Node* X_expr, * Y_expr;

	JUMP_Node(TokenType adressing_type, AST_Node* X_expr, AST_Node* Y_expr)
		: adressing_type(adressing_type), X_expr(X_expr), Y_expr(Y_expr) {};

	void accept(Visitor& v);
};


//node that calls a SUM or CNT function over region of the table
class Table_Func : public AST_Node
{
public:
	TokenType type;
	AST_Node* XMIN_expr, * YMIN_expr, * XMAX_expr, * YMAX_expr;

	Table_Func(
		TokenType type,
		AST_Node* XMIN_expr, AST_Node* YMIN_expr,
		AST_Node* XMAX_expr, AST_Node* yMAX_expr)
		: type(type), XMIN_expr(XMIN_expr), YMIN_expr(YMIN_expr), XMAX_expr(XMAX_expr), YMAX_expr(YMAX_expr) {};

	void accept(Visitor& v);
};


//this is the node for the if operator
class Conditional_Operator : public AST_Node
{
public:
	AST_Node* Cond_expr, * Then_expr, * Else_expr;
	Conditional_Operator(AST_Node* Cond_expr, AST_Node* Then_expr, AST_Node* Else_expr)
		: Cond_expr(Cond_expr), Then_expr(Then_expr), Else_expr(Else_expr) {};

	void accept(Visitor& v);
};


//this is the node for all binary boolean or arithmetic operators
class Binary_Operator : public AST_Node
{
public:
	TokenType op_name;
	AST_Node* Left_expr, * Right_expr;
	Binary_Operator(TokenType op_name, AST_Node* Left_expr, AST_Node* Right_expr)
		: op_name(op_name), Left_expr(Left_expr), Right_expr(Right_expr) {};

	void accept(Visitor& v);
};


//this is the node for all unary boolean or arithmetic operators
class Unary_Operator : public AST_Node
{
public:
	TokenType op_name;
	AST_Node* Right_expr;
	Unary_Operator(TokenType op_name, AST_Node* Right_expr)
		: op_name(op_name), Right_expr(Right_expr) {};

	void accept(Visitor& v);
};


//Primitive types
class Float : public AST_Node
{
public:
	float value;
	Float(float value)
		: value(value) {};

	void accept(Visitor& v);
};

class Integer : public AST_Node
{
public:
	int value;
	Integer(int value)
		: value(value) {};

	void accept(Visitor& v);
};

class String : public AST_Node
{
public:
	std::string value;
	String(std::string value)
		: value(value) {};

	void accept(Visitor& v);
};

#endif // !AST_NODES

