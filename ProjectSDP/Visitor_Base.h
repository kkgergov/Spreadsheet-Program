#ifndef VISITOR_BASE
#define VISITOR_BASE

#include "AST_Nodes.h"

//we are going to use this class as a base for our interpreter
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

#endif // !VISITOR_BASE
