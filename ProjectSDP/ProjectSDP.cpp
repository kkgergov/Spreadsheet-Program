// ProjectSDP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Lexer.h"
#include "SyntaxAnalyzer.h"
#include "Parser.h"
#include "AST_Printer.h"
#include "COO_SparseMatrix.h"

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
    /*
    //Lexer l1("((R[R2C1]C[-7] + 8) < 3) && R4C5 + 0.112 + 48+sum[sum[1:11][1:1]:2][2:3]  ");
    //Lexer l2("IF((0 && 4), 3 || 2, 5+4)");
    //Lexer l2("+(-8)-count4+sum(countsumcoun");
    Lexer l3("IF(5>4, \"true\" + 8 , \"false\" )");
    //Lexer l3("\"karakonjul\"");
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
    */


    
    COO_SparseMatrix a("test1.txt");
    /*a.insert(1, 4, "\" nigga\"");
    a.insert(2, 4, "\" nigga\"");
    a.insert(3, 4, "\" nigga\"");
    a.insert(5, 0, "\" nigga\"");
    a.insert(7, 0, "\" nigga\"");
    a.insert(7, 8, "\" nigga\"");*/
    //std::cout << a.n << ", " << a.m << "\n";
    a.export_as_csv("output.txt");


    /*
    CellInterpreter inter(&a);
    inter.interpret(a.find(3,0));
    cout << inter.intEval();
    */
    //a.print_all_expr();
    return 0;
}

