#ifndef ASD_PRINTER_HEADER
#define ASD_PRINTER_HEADER

#include <iostream>
#include "Token.h"
#include "Visitor.h"
#include "AST_Nodes.h"

class AST_Printer : public Visitor
{
private:
	int tabs;

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
	void print(AST_Node*);
};

#endif // !ASD_PRINTER_HEADER


