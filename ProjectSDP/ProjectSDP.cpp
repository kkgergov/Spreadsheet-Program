// ProjectSDP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Lexer.h"

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
    //Lexer l1("((R[R2C1]C[-7] + 8) < 3) && R4C5 + 0.112 + 48+sum[sum[1:1][1:1]:2][2:3]  ");
    //Lexer l2("IF((0 && 4), 3 || 2, 5+4)");
    //Lexer l2("+(-8)-count4+sum(countsumcoun");
    Lexer l3("256.+ 256 + \"lainovoz\"\"\" + 50");
    std::vector<Token> v;
    l3.tokenize_input(v);

    printTokens(v);
    /*cout << "\n" << SyntaxAnalyzer(v).correct() << "\n";
    AST_Node* test = Parser("(69 > 96) ? 123*0.03457  :: 4/127.789").parse();
    PrintNodes().print(test);*/

    /*
    COO_SparseMatrix a("test1.txt");
    CellInterpreter inter(&a);
    inter.interpret(a.find(3,0));
    cout << inter.intEval();
    */
    //a.print_all_expr();
}

