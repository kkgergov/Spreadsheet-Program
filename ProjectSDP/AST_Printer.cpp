#include "AST_Printer.h"

void AST_Printer::visit(ROOT_Node* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "r_node\n";
	++tabs;
	ast->Expr->accept(*this);
	--tabs;
}
void AST_Printer::visit(JUMP_Node* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "j_node\n";
	++tabs;
	ast->X_expr->accept(*this);
	ast->Y_expr->accept(*this);
	--tabs;
}

void AST_Printer::visit(Table_Func* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "tf_node\n";
	++tabs;
	ast->XMIN_expr->accept(*this);
	ast->XMAX_expr->accept(*this);
	ast->YMIN_expr->accept(*this);
	ast->YMAX_expr->accept(*this);
	--tabs;
}

void AST_Printer::visit(Conditional_Operator* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "cnd_op\n";
	++tabs;
	ast->Cond_expr->accept(*this);
	ast->Then_expr->accept(*this);
	ast->Else_expr->accept(*this);
	--tabs;
}

void AST_Printer::visit(Binary_Operator* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "bin_op(";
	std::cout << Token{ ast->op_name }.toString();
	std::cout << ")\n";
	++tabs;
	ast->Left_expr->accept(*this);
	ast->Right_expr->accept(*this);
	--tabs;
}
void AST_Printer::visit(Unary_Operator* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "un_op(";
	std::cout << Token{ ast->op_name }.toString();
	std::cout << ")\n";
	++tabs;
	ast->Right_expr->accept(*this);
	--tabs;
}
void AST_Printer::visit(Float* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "float(" << ast->value << ")\n";
}
void AST_Printer::visit(Integer* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "int(" << ast->value << ")\n";
}
void AST_Printer::visit(String* ast)
{
	for (int i = 0; i < tabs; ++i) std::cout << "\t";
	std::cout << "string(" << ast->value << ")\n";
}

void AST_Printer::print(AST_Node* tree)
{
	tabs = 0;
	tree->accept(*this);
}
