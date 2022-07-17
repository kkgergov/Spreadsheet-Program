#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "SyntaxAnalyzer.h"
#include "Parser.h"
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

std::vector<std::string> exctract_words_from_string(const std::string& s)
{
	std::vector<std::string> result;

	std::stringstream ss(s);
	std::string word;

	while (ss >> word)
	{
		result.push_back(word);
	}

	return result;
}

bool verify_and_get_adress(std::string maybe_adress, int& i, int& j)
{
	std::vector<Token> adress;
	try {
		Lexer(maybe_adress).tokenize_input(adress);
	}
	catch (std::runtime_error& re)
	{
		std::cout << re.what() << "\n";
		return false;;
	}
	catch (...)
	{
		std::cout << "Unknown error occured.\n";
		exit(1);
	}

	if (adress.size() == 5 && adress[0].tag == TokenType::X_AXIS && adress[2].tag == TokenType::Y_AXIS &&
		adress[1].tag == TokenType::INT && adress[3].tag == TokenType::INT)
	{
		i = adress[1].int_v;
		j = adress[3].int_v;

		return true;
	}
	else
	{
		return false;
	}
}

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
	COO_SparseMatrix* current_table = nullptr;

	std::cout << "Welcome, please start with loading a table from the computer using the command LOAD.\n\n\n";

	std::string input_line;
	while (true)
	{
		std::cout << "-----\n";
		std::cout << "table is loaded: " << (table_is_loaded ? "YES" : "NO") << "\n";
		std::cout << "enter command:\n>";

		std::getline(std::cin, input_line);
		std::vector<std::string> command = exctract_words_from_string(input_line);

		if (command[0] == "SET" && command.size() == 3)
		{
			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			int i = -1, j = -1;

			if (!verify_and_get_adress(command[1], i, j))
			{
				std::cout << "Invalid format of adress, try again\n";
				continue;
			}

			current_table->insert(i, j, command[2]);
			std::cout << "Succsessfuly set formula at (" << i << ", " << j << ") to " << command[2] << "\n";

		}
		else if (command[0] == "PRINT" && command.size() == 3)
		{
			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			if (command[1] == "EXPR")
			{
				if (command[2] == "ALL")
				{
					std::vector<std::string> res;
					current_table->get_nth_10_full_columns(res, 0);
					for (const std::string& a : res)
					{
						if (a == "@") std::cout << "\n";
						else
						{
							std::cout << a << "\t";
						}
					}
					std::cout << "\n";

				}
			}
			else if (command[1] == "VAL")
			{

			}
			else
			{
				std::cout << "Wrong PRINT arguments\n";
				continue;
			}
		}
		else if (command[0] == "SAVE" && command.size() == 2)
		{

			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			try
			{
				current_table->export_as_csv(command[1]);
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

			std::cout << "Successfully saved current table to " << command[1] << "\n";
		}
		else if (command[0] == "LOAD" && command.size() == 2)
		{

			if (table_is_loaded)
			{
				std::cout << "Unloading old table from memory...\n";
				delete current_table;
				current_table = nullptr;
				table_is_loaded = false;
			}

			try
			{
				current_table = new COO_SparseMatrix(command[1]);
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

			std::cout << "Loaded table from file: " << command[1] << "\n";
			table_is_loaded = true;
		}

		else if (command[0] == "++" && command.size() == 2)
		{
			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			int i = -1, j = -1;

			if (!verify_and_get_adress(command[1], i, j))
			{
				std::cout << "Invalid format of adress, try again\n";
				continue;
			}

			try {
				//insert ovverides the cell with the input if it already exists
				current_table->insert(i, j, current_table->get_expr(i, j) + "+ 1");
			}
			catch (std::runtime_error& re)
			{
				std::cout << re.what() << "\n";
			}
			catch (...)
			{
				std::cout << "Unknown error occured.\n";
				return 1;
			}

			std::cout << "Succsessfully incremented cell " << command[1] << "\n";
		}
		else if (command[0] == "--" && command.size() == 2)
		{
			if (!table_is_loaded)
			{
				std::cout << "load a table first!\n";
				continue;
			}

			int i = -1, j = -1;

			if (!verify_and_get_adress(command[1], i, j))
			{
				std::cout << "Invalid format of adress, try again\n";
				continue;
			}

			try {
				//insert ovverides the cell with the input if it already exists
				current_table->insert(i, j, current_table->get_expr(i, j) + "- 1");
			}
			catch (std::runtime_error& re)
			{
				std::cout << re.what() << "\n";
			}
			catch (...)
			{
				std::cout << "Unknown error occured.\n";
				return 1;
			}

			std::cout << "Succsessfully decremented cell " << command[1] << "\n";
		}
		else if (command[0] == "EXIT")
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