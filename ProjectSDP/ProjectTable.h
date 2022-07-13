#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include "Visitor.h"
#include "AST.h"
#include "COO_SparseMatrix.h"
#include "SyntaxAnalyzer.h"
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
    float current_value;

    virtual void visit(ROOT_Node* ast)
    {
        current_value = 0;
        ast->Expr->accept(*this);
    }

    virtual void visit(JUMP_Node* ast)
    {
        int x_crd, y_crd;

        ast->X_expr->accept(*this);
        x_crd = (int)current_value;
        ast->Y_expr->accept(*this);
        y_crd = (int)current_value;

        //try to load in the other cell and interpret it
        AST_Node* other_cell_expr_ast = nullptr;
        try
        {
            std::string other_cell_expr_string = Table->get_expr(x_crd, y_crd);

            std::vector<Token> tokenized;
            Lexer(other_cell_expr_string).tokenize_input(tokenized);
            SyntaxAnalyzer(tokenized).correct();

            other_cell_expr_ast = Parser(tokenized).parse();
        }
        catch (std::runtime_error &re)
        {
            //std::cout << re.what() << "\n";
            throw std::runtime_error(
                std::string(">INTERPRETER\nerror: cannot load cell ") + 
                "(" + std::to_string(x_crd) + std::to_string(y_crd) + ")\n" +
                "reason: " + re.what());
        }

        
        other_cell_expr_ast->accept(*this);
    }
    virtual void visit(Table_Func* ast)
    {
        int x1, x2, y1, y2;

        /*ast->xMAX->accept(*this);
        ast->xMIN->accept(*this);
        ast->yMAX->accept(*this);
        ast->yMIN->accept(*this);*/
    }

    virtual void visit(Conditional_Operator* ast)
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

    virtual void visit(Binary_Operator* ast)
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
    virtual void visit(Unary_Operator* ast)
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

    virtual void visit(Float* ast) 
    {
        current_value = ast->value;
    }
    virtual void visit(Integer* ast) 
    {
        current_value = (float)ast->value;
    }
    virtual void visit(String* ast)
    {
        current_value = 0;
    }
public:
    CellInterpreter(COO_SparseMatrix* table) :Table(table), current_value(0)
    {
    }

    void interpret(AST_Node* tree)
    {
        tree->accept(*this);
    }

    float eval() const
    {
        return current_value;
    }
};
