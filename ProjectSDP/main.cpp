#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "SyntaxAnalyzer.h"
#include "Parser.h"
#include "COO_SparseMatrix.h"
#include "AST_Deleter.h"
#include "CellInterpreter.h"

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

		if (command.size() == 0)
		{
			continue;
		}

		if (command[0] == "SET" && command.size() >= 3)
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

			std::string formula = "";
			//std::cout << command.size();
			for (int i = 2; i < command.size(); i++)
			{
				formula += command[i];
			}

			current_table->insert(i, j, formula);
			std::cout << "Succsessfuly set formula at (" << i << ", " << j << ") to " << formula << "\n";

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
					int n = 0;
					while (current_table->get_nth_10_full_columns(res, n) != -1)
					{

						for (const std::string& a : res)
						{
							if (a == "@") std::cout << "\n";
							else if (a == ",")  std::cout << "|";
							else
							{
								std::cout << a;
							}
						}
						std::cout << "\n\n-----------------------------------------------\n\n";

						res.clear();
						n++;
					}

				}
				else
				{
					int i = -1, j = -1;

					if (!verify_and_get_adress(command[2], i, j))
					{
						std::cout << "Invalid format of adress, try again\n";
						continue;
					}

					try
					{
						std::cout << current_table->get_expr(i, j) << "\n";
					}
					catch (std::runtime_error& re)
					{
						std::cout << re.what();
					}
					catch (...)
					{
						std::cout << "Unknown error occured.\n";
						return 1;
					}
				}
			}
			else if (command[1] == "VAL")
			{
				if (command[2] == "ALL")
				{

					std::vector<std::pair<int,int>> res;
					int n = 0;
					while (current_table->get_nth_10_full_columns_coords(res, n) != -1)
					{

						for (const std::pair<int,int>& a : res)
						{
							if (a.first == INT_MIN) std::cout << "\n";
							else if (a.first == INT_MAX)  std::cout << "|";
							else
							{
								//std::cout << "(" << a.first << ", " << a.second << ")";
								AST_Node* jump_tree = nullptr; //we create a tree with ROOT node and JUMP node and interpret it
								try
								{
									std::string coords_to_visit = "R" + std::to_string(a.first) + "C" + std::to_string(a.second);

									std::vector<Token> tokenized;
									Lexer(coords_to_visit).tokenize_input(tokenized);

									SyntaxAnalyzer(tokenized).correct();

									jump_tree = Parser(tokenized).parse();
									
									CellInterpreter inter(current_table, a.first, a.second);
									inter.interpret(jump_tree);

									std::cout << inter.eval();
								}
								catch (std::runtime_error& re)
								{
									//std::cout << re.what() << "\n";
									AST_Deleter().deleter_delete(jump_tree);

									std::cout << re.what();

									continue;
								}
								catch (...)
								{
									std::cout << "Unknown error occured.\n";
									return 1;
								}

								AST_Deleter().deleter_delete(jump_tree);
							}
						}
						std::cout << "\n\n-----------------------------------------------\n\n";

						res.clear();
						n++;
					}

				}
				else
				{
					int i = -1, j = -1;

					if (!verify_and_get_adress(command[2], i, j))
					{
						std::cout << "Invalid format of adress, try again\n";
						continue;
					}

					AST_Node* jump_tree = nullptr;
					try
					{
						std::string coords_to_visit = "R" + std::to_string(i) + "C" + std::to_string(j);

						std::vector<Token> tokenized;
						Lexer(coords_to_visit).tokenize_input(tokenized);

						SyntaxAnalyzer(tokenized).correct();

						jump_tree = Parser(tokenized).parse();

						CellInterpreter inter(current_table, i, j);
						inter.interpret(jump_tree);

						std::cout << inter.eval() << "\n";

					}
					catch (std::runtime_error& re)
					{
						std::cout << re.what() <<"\n";
						AST_Deleter().deleter_delete(jump_tree);
					}
					catch (...)
					{
						std::cout << "Unknown error occured.\n";
						return 1;
					}
				}
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