#include "Visitor.h"

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