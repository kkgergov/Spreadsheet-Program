#pragma once
#include <fstream>
#include <sstream>
#include "Visitor.h"
#include "AST.h"
#include "COO_SparseMatrix.h"
using namespace std;


const float EPSILON = 0.001;
bool f_equal(float a, float b)
{
    return ((a - b) < EPSILON) && ((b - a) < EPSILON);
}
bool f_nequal(float a, float b)
{
    return !f_equal(a, b);
}
//each identifier(cell coordinates) needs to at least be declared before it is used.
//To be declared, means to exist somewhere in the Table (even if its an empty string)
//Strings always get evaluated with value of 0

class CellInterpreter : public Visitor
{
private:
    COO_SparseMatrix* Table;
    bool float_expr;
    float current_float;
    int current_int;

    virtual void visit(ROOT_Node* ast)
    {
        current_int = 0;
        current_float = 0;
        ast->Tree_expr->accept(*this);
    }
    virtual void visit(JUMP_Node* ast)
    {
        int x_crd, y_crd;

        ast->x_coord->accept(*this);
        x_crd = current_int;
        ast->y_coord->accept(*this);
        y_crd = current_int;

        AST_Node* go_to = Table->find(x_crd, y_crd);
        if (!go_to) throw std::runtime_error("This cell does not exist in the table!");

        go_to->accept(*this);
    }
    virtual void visit(Table_Func* ast)
    {
        int x1, x2, y1, y2;

        ast->xMAX->accept(*this);
        ast->xMIN->accept(*this);
        ast->yMAX->accept(*this);
        ast->yMIN->accept(*this);
    }
    virtual void visit(Conditional_Operator* ast)
    {
        int c_i;
        float c_f;

        ast->Condition_expr->accept(*this);
        c_i = current_int;
        c_f = current_float;

        if (c_i || c_f)
        {
            ast->Then_expr->accept(*this);
        }
        else
        {
            ast->Else_expr->accept(*this);
        }

        

    }
    virtual void visit(Binary_Operator* ast)
    {
        int l_i, r_i;
        float l_f, r_f;

        ast->Left_expr->accept(*this);
        l_i = current_int;
        l_f = current_float;
        ast->Right_expr->accept(*this);
        r_i = current_int;
        r_f = current_float;

        switch (ast->op_name)
        {
        case Type::OR:
            current_int = l_i || r_i;
            current_float = l_f || r_f;
            break;
        case Type::AND:
            current_int = l_i && r_i;
            current_float = l_f && r_f;
            break;
        case Type::EQ:
            current_int = (l_i == r_i);
            current_float = f_equal(l_f, r_f);
            break;
        case Type::NEQ:
            current_int = (l_i != r_i);
            current_float = f_nequal(l_f, r_f);
            break;
        case Type::PLUS:
            current_int = l_i + r_i;
            current_float = l_f + r_f;
            break;
        case Type::MINUS:
            current_int = l_i - r_i;
            current_float = l_f - r_f;
            break;
        case Type::MUL:
            current_int = l_i * r_i;
            current_float = l_f * r_f;
            break;
        case Type::DIV:
            current_int = l_i / r_i;
            current_float = l_f / r_f;
            break;
        default:
            break;
        }
    }
    virtual void visit(Unary_Operator* ast)
    {
        ast->Right_expr->accept(*this);

        switch (ast->op_name)
        {
        case Type::NOT:
            current_int = !current_int;
            current_float = !current_float;
            break;
        case Type::MINUS:
            current_int = -current_int;
            current_float = -current_float;
            break;
        default:
            break;
        }
    }

    virtual void visit(Float* ast) 
    {
        float_expr = true;
        current_float = ast->value;
        current_int = (int)ast->value;
    }
    virtual void visit(Integer* ast) 
    {
        current_int = ast->value;
        current_float = (float)ast->value;
    }
    virtual void visit(String* ast)
    {
        current_float = 0;
        current_int = 0;
    }
public:
    CellInterpreter(COO_SparseMatrix* table)
    {
        Table = table;
        float_expr = false; //by default the return type is int
    }

    void interpret(AST_Node* tree)
    {
        tree->accept(*this);
    }

    int intEval() const
    {
        return current_int;
    }

    float floatEval() const
    {
        return current_float;
    }
};
