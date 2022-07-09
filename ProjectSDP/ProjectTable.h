#pragma once
#include <fstream>
#include <sstream>
#include "AST.h"
using namespace std;

struct COO_SparseMatrix
{
    COO_SparseMatrix(int rows = 0, int columns = 0) : n(rows), m(columns){}

    COO_SparseMatrix(const string& input_file)
    {
        ifstream iFile(input_file);

        if (iFile.good())
        {
            string cur_line, cell_val;
            int i = 0, j = 0;
            int max_n =0, max_m=0;

            //check if n and m exceed, if they do the table cannot be created
            while (getline(iFile, cur_line))
            {

                string word;
                stringstream ss(cur_line);

                while (getline(ss, word, ';'))
                {
                    ++j;
                    if (j > max_m) max_m = j;

                    if (ss.peek() == '\n') ss.ignore();
                }

                j = 0;

                ++i;
                if (i > max_n) max_n = i;
            }

            if (max_n > 10000 || max_m > 10000)
                throw std::runtime_error("Exceeded the maximal size of table!");

            i = 0, j = 0;
            iFile.clear();
            iFile.seekg(0);

            while (getline(iFile, cur_line))
            {

                string word;
                stringstream ss(cur_line);

                while (getline(ss, word, ';'))
                {
                    if (word != "")
                        insert(i, j, word);

                    ++j;

                    if (ss.peek() == '\n') ss.ignore();
                }

                j = 0;

                ++i;
            }


            n = max_n, m = max_m;
        }
        else throw std::runtime_error("couldn't open file!");
    }

    ~COO_SparseMatrix()
    {
        //delete all values from the table
    }

    void print_expr(int index) const
    {
        cout << "At (" << row[index] << ", " << col[index] << "): \n";
        PrintNodes().print(val[index]);
        cout << "\n\n\n";
    }

    void print_all_expr() const
    {
        cout << "Table size is: (" << n << ", " << m << ")\n";

        for (int idx = 0; idx < row.size();++idx)print_expr(idx);
    }
    void insert(int i, int j, string formula)
    {

        //we handle overwrites this way
        if (validCoordinates(i, j))
        {
            row.push_back(i);
            col.push_back(j);

            //if error occurs here, place only root node and notify trough the console
            val.push_back(Parser(formula).parse());
        }
        else
        {
            throw std::invalid_argument("Coordinates outside table!");
            return;
        }
    }
    AST_Node* find(int i, int j)
    {
        if (validCoordinates(i, j))
        {
            for (int l = 0; l < row.size(); ++l)
            {
                if (row[l] == i && col[l] == j) //found element with such coords
                {
                    return val[l];
                }
            }
            return nullptr;
        }
        else return nullptr;
    }
    
    
private:
    bool validCoordinates(int i, int j)
    {
        if (i >= 100000 || j >= 10000 || i < 0 || j < 0)
            return false;
        return true;
    }


    //the size of the matrix
    int n, m;

    vector<int> row, col;
    vector<AST_Node*> val;
};

const float EPSILON = 0.001;
bool f_equal(float a, float b)
{
    return ((a - b) < EPSILON) && ((b - a) < EPSILON);
}
bool f_nequal(float a, float b)
{
    return !f_equal(a, b);
}
//each identifier(cell coordinates) needs to be declared before it is used.
//To be declared, means to exist somewhere in the Table (if it isn't declared, we wont interpret further)
//An empty(not the same as missing) or pure text cell is always evaluated as 0
//if we encounter a boolean 
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