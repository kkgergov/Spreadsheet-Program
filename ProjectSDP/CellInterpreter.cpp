#include "CellInterpreter.h"

bool f_equal(float a, float b)
{
	return ((a - b) < EPSILON) && ((b - a) < EPSILON);
}
bool f_nequal(float a, float b)
{
	return !f_equal(a, b);
}

void CellInterpreter::visit(ROOT_Node* ast)
{
	current_value = 0;
	ast->Expr->accept(*this);
}

void CellInterpreter::visit(JUMP_Node* ast)
{
	int x_crd, y_crd;

	ast->X_expr->accept(*this);
	x_crd = (int)current_value;
	ast->Y_expr->accept(*this);
	y_crd = (int)current_value;

	int temp_x = curr_x, temp_y = curr_y;

	if (ast->adressing_type == TokenType::ABSOLUTE)
	{
		curr_x = x_crd;
		curr_y = y_crd;
	}
	else if (ast->adressing_type == TokenType::RELATIVE)
	{
		curr_x += x_crd;
		curr_y += y_crd;
	}

	//try to load in the other cell and interpret it
	AST_Node* other_cell_expr_ast = nullptr;
	try
	{
		std::string other_cell_expr_string = Table->get_expr(curr_x, curr_y);

		std::vector<Token> tokenized;
		Lexer(other_cell_expr_string).tokenize_input(tokenized);
		SyntaxAnalyzer(tokenized).correct();

		other_cell_expr_ast = Parser(tokenized).parse();
	}
	catch (std::runtime_error& re)
	{
		//std::cout << re.what() << "\n";
		throw std::runtime_error(
			std::string(">INTERPRETER\nerror: cannot load cell ") +
			"(" + std::to_string(curr_x) + ", " + std::to_string(curr_y) + ")\n" +
			"reason: " + re.what());
	}

	other_cell_expr_ast->accept(*this);

	curr_x = temp_x;
	curr_y = temp_y;
}
void CellInterpreter::visit(Table_Func* ast)
{
	int x1, x2, y1, y2;

	/*ast->xMAX->accept(*this);
	ast->xMIN->accept(*this);
	ast->yMAX->accept(*this);
	ast->yMIN->accept(*this);*/
}

void CellInterpreter::visit(Conditional_Operator* ast)
{
	ast->Cond_expr->accept(*this);
	int c_i = (int)current_value; //there is no conversion between float and bool so we only care about integer value of expression

	if (c_i)
	{
		ast->Then_expr->accept(*this);
	}
	else
	{
		ast->Else_expr->accept(*this);
	}
}

void CellInterpreter::visit(Binary_Operator* ast)
{
	float l_f, r_f;

	ast->Left_expr->accept(*this);
	l_f = current_value;
	ast->Right_expr->accept(*this);
	r_f = current_value;

	switch (ast->op_name)
	{
	case TokenType::OR:
		current_value = l_f || r_f;
		break;
	case TokenType::AND:
		current_value = l_f && r_f;
		break;
	case TokenType::EQ:
		current_value = f_equal(l_f, r_f);
		break;
	case TokenType::NEQ:
		current_value = f_nequal(l_f, r_f);
		break;
	case TokenType::PLUS:
		current_value = l_f + r_f;
		break;
	case TokenType::MINUS:
		current_value = l_f - r_f;
		break;
	case TokenType::MUL:
		current_value = l_f * r_f;
		break;
	case TokenType::DIV:
		current_value = l_f / r_f;
		break;
	default:
		break;
	}
}
void CellInterpreter::visit(Unary_Operator* ast)
{
	ast->Right_expr->accept(*this);

	switch (ast->op_name)
	{
	case TokenType::NOT:
		current_value = !current_value;
		break;
	case TokenType::MINUS:
		current_value = -current_value;
		break;
	default:
		break;
	}
}

void CellInterpreter::visit(Float* ast)
{
	current_value = ast->value;
}
void CellInterpreter::visit(Integer* ast)
{
	current_value = (float)ast->value;
}
void CellInterpreter::visit(String* ast)
{
	current_value = 0;
}

CellInterpreter::CellInterpreter(COO_SparseMatrix* table) :Table(table), current_value(0), curr_x(0), curr_y(0)
{
}

void CellInterpreter::interpret(AST_Node* tree)
{
	tree->accept(*this);
}

float CellInterpreter::eval() const
{
	return current_value;
}