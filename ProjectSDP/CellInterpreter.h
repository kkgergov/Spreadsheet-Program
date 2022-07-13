#ifndef CELL_INTERPRETER_HEADER
#define CELL_INTERPRETER_HEADER


#include "Lexer.h"
#include "Parser.h"
#include "Visitor.h"
#include "SyntaxAnalyzer.h"
#include "COO_SparseMatrix.h"


const float EPSILON = 0.001;

//each identifier(cell coordinates) needs to at least be declared before it is used.
//To be declared, means to exist somewhere in the Table (even if its an empty string)
//Strings always get evaluated with value of 0

class CellInterpreter : public Visitor
{
private:
    COO_SparseMatrix* Table;
    float current_value;
    int curr_x, curr_y;

    bool sum_iscalled;

    virtual void visit(ROOT_Node* ast);

    virtual void visit(JUMP_Node* ast);
    virtual void visit(Table_Func* ast);

    virtual void visit(Conditional_Operator* ast);

    virtual void visit(Binary_Operator* ast);
    virtual void visit(Unary_Operator* ast);

    virtual void visit(Float* ast);
    virtual void visit(Integer* ast);
    virtual void visit(String* ast);
public:
    CellInterpreter(COO_SparseMatrix* table);

    void interpret(AST_Node* tree);

    float eval() const;
};

#endif // !CELL_INTERPRETER_HEADER

