#ifndef COO_MATRIX_HEADER
#define COO_MATRIX_HEADER

#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

struct Cell
{
	int x;
	int y;
	std::string formula;

	Cell(int x, int y, const std::string& formula) : x(x), y(y), formula(formula) {}
};

struct COO_SparseMatrix
{
	COO_SparseMatrix(int rows = 0, int columns = 0);

	COO_SparseMatrix(const std::string&);

	void export_as_csv(const std::string&);

	std::string get_expr(int, int) const;

	//returns the wanted region of the table as a vector of strings (expressions)
	void get_expr_region(std::vector<std::string>&, int, int, int, int) const;

	void insert(int i, int j, std::string formula);

private:
	bool validCoordinates(int, int) const;

	//checks if the csv has too many rows or columns
	void get_csv_size(std::ifstream&, int&, int&);

	int find_table_idx_in_array(int, int) const;

	//the size of the matrix
	int n, m;

	std::vector<Cell> data;
};

#endif // !COO_MATRIX_HEADER


