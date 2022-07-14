#include "AST_Deleter.h"

void AST_Deleter::visit(ROOT_Node* ast)
{
	ast->Expr->accept(*this);
	delete ast;
}
void AST_Deleter::visit(JUMP_Node* ast)
{
	ast->X_expr->accept(*this);
	ast->Y_expr->accept(*this);
	delete ast;
}

void AST_Deleter::visit(Table_Func* ast)
{
	ast->XMAX_expr->accept(*this);
	ast->XMIN_expr->accept(*this);
	ast->YMAX_expr->accept(*this);
	ast->YMIN_expr->accept(*this);

	delete ast;
}

void AST_Deleter::visit(Conditional_Operator* ast)
{
	ast->Cond_expr->accept(*this);
	ast->Else_expr->accept(*this);
	ast->Then_expr->accept(*this);

	delete ast;
}

void AST_Deleter::visit(Binary_Operator* ast)
{
	ast->Left_expr->accept(*this);
	ast->Right_expr->accept(*this);
	delete ast;
}

void AST_Deleter::visit(Unary_Operator* ast)
{
	ast->Right_expr->accept(*this);
	delete ast;
}

void AST_Deleter::visit(Float* ast)
{
	delete ast;
}

void AST_Deleter::visit(Integer* ast)
{
	delete ast;
}

void AST_Deleter::visit(String* ast)
{
	delete ast;
}

void AST_Deleter::deleter_delete(AST_Node* ast)
{
	if (!ast)
	{
		return;
	}

	ast->accept(*this);
}

