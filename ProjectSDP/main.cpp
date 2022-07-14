#include <iostream>
#include <string>
#include <vector>
#include "Lexer.h"
#include "SyntaxAnalyzer.h"
#include "Parser.h"
#include "COO_SparseMatrix.h"

AST_Node* tokenize_analyze_parse(std::string input)
{
	try
	{
		std::vector<Token> v;
		Lexer(input).tokenize_input(v);
		SyntaxAnalyzer(v).correct();
		return Parser(v).parse();
	}
	catch (std::runtime_error& re)
	{
		std::cout << re.what() << "\n";
		return nullptr;
	}
}


int main()
{
	bool table_is_loaded = false;
	COO_SparseMatrix *current_table = nullptr;

	std::cout << "Welcome, please start with loading a table from the computer using the command LOAD.\n\n\n";

	std::string cmd, arg1, arg2;
	while (true)
	{
		std::cout << "-----\n";
		std::cout << "table is loaded: " << (table_is_loaded ? "YES" : "NO") << "\n";
		std::cout << "enter command:\n>";

		std::cin >> cmd;
		if (cmd == "SET")
		{
			std::cin >> arg1;

			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			int i=-1, j=-1;

			std::vector<Token> adress;
			try {
				Lexer(arg1).tokenize_input(adress);
			}
			catch (std::runtime_error& re)
			{
				std::cout << re.what() << "\n";
				continue;
			}
			catch (...)
			{
				std::cout << "Unknown error occured.\n";
				return 1;
			}
			
			if (adress.size() == 5 && adress[0].tag == TokenType::X_AXIS && adress[2].tag == TokenType::Y_AXIS &&
									  adress[1].tag == TokenType::INT && adress[3].tag == TokenType::INT)
			{
				i = adress[1].int_v;
				j = adress[3].int_v;
			}
			else
			{
				std::cout << "Invalid format of adress, try again\n";
				continue;
			}

			std::cin >> arg2;
			current_table->insert(i, j, arg2);
			std::cout << "Succsessfuly set formula at (" << i << ", " << j << ") to " << arg2 << "\n";

		}
		else if (cmd == "PRINT")
		{

		}
		else if (cmd == "SAVE")
		{
			std::cin >> arg1;

			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			try
			{
				current_table->export_as_csv(arg1);
			}
			catch (std::runtime_error& re)
			{
				std::cout << re.what() << "\n";
				std::cout << "Please try again to save the table\n";
				continue;
			}
			catch (...)
			{
				std::cout << "Unknown error occured.\n";
				return 1;
			}

			std::cout << "Successfully saved current table to " << arg1 << "\n";
		}
		else if (cmd == "LOAD")
		{
			std::cin >> arg1;

			if (table_is_loaded)
			{
				std::cout << "Unloading old table from memory...\n";
				delete current_table;
				current_table = nullptr;
				table_is_loaded = false;
			}

			try
			{
				current_table = new COO_SparseMatrix(arg1);
			}
			catch (std::runtime_error& re)
			{
				current_table = nullptr;

				std::cout << re.what() << "\n";
				std::cout << "Please try again to load a table\n";
				continue;
			}
			catch (...)
			{
				std::cout << "Unknown error occured.\n";
				return 1;
			}

			std::cout << "Loaded table from file: " << arg1 << "\n";
			table_is_loaded = true;
		}

		else if (cmd == "++")
		{

		}
		else if (cmd == "--")
		{

		}
		else if (cmd == "EXIT")
		{
			if (table_is_loaded)
			{
				std::cout << "Unloading table from memory...\n";
				delete current_table;
				table_is_loaded = false;
			}

			std::cout << "Goodbye!\n";
			break;
		}
		else
		{
			std::cout << ">wrong command, try again\n";
		}

	}

	return 0;
}