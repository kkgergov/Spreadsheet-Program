// ProjectSDP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Lexer.h"
#include "SyntaxAnalyzer.h"
#include "Parser.h"
#include "AST_Printer.h"
#include "COO_SparseMatrix.h"
#include "CellInterpreter.h"
using namespace std;

void printTokens(const std::vector<Token>& v)
{
    for (const Token& a : v)
    {
        std::cout << a.toString();
        std::cout << " ";
    }
    std::cout << "\n";
}

int main()
{
    
    //Lexer l1("((R[R2C1]C[-7] + 8) < 3) && R4C5 + 0.112 + 48+sum[sum[1:11][1:1]:2][2:3]  ");
    //Lexer l2("IF((0 && 4), 3 || 2, 5+4)");
    //Lexer l2("+(-8)-count4+sum(countsumcoun");
    //Lexer l3("IF(5>4 && 0, \"true\" + 8 , \"false\" + 5 * (0 || 1) +)");
    
    
    Lexer l3("R0C8");
    std::vector<Token> v;

    try {
        l3.tokenize_input(v);
    }
    catch (std::runtime_error& re)
    {
        std::cout << re.what() << "\n";
        return 1;
    }

    //printTokens(v);

    SyntaxAnalyzer a(v);

    try {
        std::cout << "\n" << a.correct() << "\n";
    }
    catch (std::runtime_error& re)
    {
        std::cout << re.what() << "\n";
        return 1;
    }
    

    AST_Node* test = Parser(v).parse();
    AST_Printer().print(test);


    COO_SparseMatrix coo_mat("test1.txt");
    CellInterpreter interpreter(&coo_mat);
    try
    {
        interpreter.interpret(test);
    }
    catch (std::runtime_error& re)
    {
        cout << re.what() << "\n";
        return 1;
    }
    cout << "interpreter output is: " << interpreter.eval() << "\n";
    


    
    /*
    COO_SparseMatrix a("test1.txt");
    a.insert(1, 4, "\" nigga\"");
    a.insert(2, 4, "\" nigga\"");
    a.insert(3, 4, "\" nigga\"");
    a.insert(5, 0, "\" nigga\"");
    a.insert(7, 0, "\" nigga\"");
    a.insert(7, 8, "25 + 60 +70 +80");
    //std::cout << a.get_expr(7,8) << "\n";
    a.export_as_csv("output.txt");

    std::vector<std::string> expressions;
    a.get_expr_region(expressions, 1, 3, 7, 8);
    for (int i = 0; i < expressions.size(); i++)
    {
        std::cout << expressions[i] << "\n";
    }

    
    CellInterpreter inter(&a);
    inter.interpret(a.find(3,0));
    cout << inter.intEval();
    
    //a.print_all_expr();
    */

    return 0;
}

