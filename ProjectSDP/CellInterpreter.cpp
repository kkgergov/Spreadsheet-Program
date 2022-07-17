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

	int temp_i = curr_i, temp_j = curr_j;

	if (ast->adressing_type == TokenType::ABSOLUTE)
	{
		curr_i = x_crd;
		curr_j = y_crd;
	}
	else if (ast->adressing_type == TokenType::RELATIVE)
	{
		curr_i += x_crd;
		curr_j += y_crd;
	}

	//try to load in the other cell and interpret it
	AST_Node* other_cell_expr_ast = nullptr;
	try
	{
		std::string other_cell_expr_string = Table->get_expr(curr_i, curr_j);

		std::vector<Token> tokenized;
		Lexer(other_cell_expr_string).tokenize_input(tokenized);

		SyntaxAnalyzer(tokenized).correct();

		other_cell_expr_ast = Parser(tokenized).parse();

		//AST_Printer().print(other_cell_expr_ast);
	}
	catch (std::runtime_error& re)
	{
		//std::cout << re.what() << "\n";
		AST_Deleter().deleter_delete(other_cell_expr_ast);

		throw std::runtime_error(
			std::string(">INTERPRETER\nerror: cannot load cell ") +
			"(" + std::to_string(curr_i) + ", " + std::to_string(curr_j) + ")\n" +
			"reason: " + re.what());
	}

	other_cell_expr_ast->accept(*this);

	AST_Deleter().deleter_delete(other_cell_expr_ast);

	curr_i = temp_i;
	curr_j = temp_j;
}
void CellInterpreter::visit(Table_Func* ast)
{
	if (ast->type == TokenType::SUM && sum_iscalled) return;

	int x1, x2, y1, y2;

	ast->XMIN_expr->accept(*this);
	x1 = current_value;
	ast->YMIN_expr->accept(*this);
	y1 = current_value;
	ast->XMAX_expr->accept(*this);
	x2 = current_value;
	ast->YMAX_expr->accept(*this);
	y2 = current_value;
	
	std::vector<std::pair<int,int>> to_sum;
	Table->get_coords_region(to_sum, x1, x2, y1, y2);

	//std::cout << "[" << x1 << ": " << x2 << "][" << y1 << ": " << y2 << "]\n";
	///*for (const auto& a : to_sum)
	//{
	//	std::cout << "(" << a.first << ", " << a.second << ")\n";
	//}*/

	if (ast->type == TokenType::COUNT)
	{
		current_value = to_sum.size();
		return;
	}

	sum_iscalled = true;
	int sum = 0;
	int i_temp = curr_i, j_temp = curr_j;
	for (const std::pair<int,int>& coords : to_sum)
	{
		std::string formula = Table->get_expr(coords.first, coords.second);

		curr_i = coords.first, curr_j = coords.second;

		AST_Node* ast_expr = nullptr;
		try
		{
			std::vector<Token> tokenized;
			Lexer(formula).tokenize_input(tokenized);

			SyntaxAnalyzer(tokenized).correct();

			ast_expr = Parser(tokenized).parse();
		}
		catch (std::runtime_error& re)
		{
			AST_Deleter().deleter_delete(ast_expr);
			//std::cout << re.what() << "\n";
			throw std::runtime_error(
				std::string(">INTERPRETER\nerror: parse cell in region\n") +
				"formula is: " + formula + "\n" +
				"reason: " + re.what());
		}


		ast_expr->accept(*this);
		sum += current_value;
		AST_Deleter().deleter_delete(ast_expr);
	}
	sum_iscalled = false;

	curr_i = i_temp, curr_j = j_temp;

	current_value = sum;
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

CellInterpreter::CellInterpreter(COO_SparseMatrix* table, int i = 0, int j = 0) :Table(table), current_value(0), curr_i(i), curr_j(j), sum_iscalled(false)
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