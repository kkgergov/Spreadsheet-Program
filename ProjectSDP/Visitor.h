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

#endif // !VISITOR_BASE
