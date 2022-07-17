#ifndef AST_DELETER_HEADER
#define AST_DELETER_HEADER

#include "Visitor.h"

//deletes nodes
class AST_Deleter : public Visitor
{

	void visit(ROOT_Node*);
	void visit(JUMP_Node*);

	void visit(Table_Func*);

	void visit(Conditional_Operator*);

	void visit(Binary_Operator*);
	void visit(Unary_Operator*);
	void visit(Float*);
	void visit(Integer*);
	void visit(String*);
public:
	void deleter_delete(AST_Node*);
};

#endif // !AST_DELETER_HEADER




