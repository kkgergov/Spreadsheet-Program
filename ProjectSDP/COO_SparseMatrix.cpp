#include "COO_SparseMatrix.h"

COO_SparseMatrix::COO_SparseMatrix(int rows, int columns) : n(rows), m(columns) {}

COO_SparseMatrix::COO_SparseMatrix(const std::string& input_file)
{
	std::ifstream iFile(input_file);

	if (iFile.good())
	{
		std::string cur_line, cell_val;
		int i = 0, j = 0;
		int max_n = 0, max_m = 0;

		get_csv_size(iFile, max_n, max_m);

		//check if n and m exceed, if they do the table cannot be created
		if (max_n > 100000 || max_m > 100000)
		{
			throw std::runtime_error(">COO_Table\nerror: Number of rows or cols in the .csv exceeds the maximal allowed!");
		}


		i = 0, j = 0;
		iFile.clear();
		iFile.seekg(0);

		while (getline(iFile, cur_line))
		{

			std::string word;
			std::stringstream ss(cur_line);

			while (std::getline(ss, word, ';'))
			{
				if (word != "")
					insert(j, i, word);

				++j;

				if (ss.peek() == '\n') ss.ignore();
			}

			j = 0;

			++i;
		}


		n = max_n, m = max_m;
	}
	else throw std::runtime_error(">COO_Table\n error: couldn't open file!");
}

void COO_SparseMatrix::export_as_csv(const std::string& output_file)
{
	//lexicoraphically sort our data for linear transformation to dense matrix in the .csv

	std::sort(data.begin(), data.end(), [](Cell a, Cell b)->bool {return a.x < b.x || (a.x == b.x && a.y < b.y);});

	std::ofstream oFile(output_file);

	if (oFile.good())
	{
		int idx_cell_to_export = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (idx_cell_to_export != data.size() && data[idx_cell_to_export].x == i && data[idx_cell_to_export].y == j)
				{

					oFile << data[idx_cell_to_export].formula;
					idx_cell_to_export++;

					if (j == m - 2)
					{
						continue;
					}
				}

				if (j < m - 1)
				{
					oFile << ';';
				}
			}
			oFile << "\n";
		}
	}
	else
	{
		throw std::runtime_error(">COO_TABLE\nerror: file doesnt exist or doesnt have write permissions");
	}

}

std::string COO_SparseMatrix::get_expr(int i, int j) const
{
	int idx = find_table_idx_in_array(i, j);
	if (idx != -1)
	{
		return data[idx].formula;
	}
	else
	{
		throw std::runtime_error(">COO_TABLE\nerror: this variable does not exist in the table");
	}
}

void COO_SparseMatrix::get_expr_region(std::vector<std::string>& result, int xmin, int ymin, int xmax, int ymax) const
{
	if (xmin < 0 || xmin >= m ||
		ymin < 0 || ymin >= n ||
		xmax < 0 || xmax >= m ||
		ymax < 0 || ymax >= n)
	{
		throw std::runtime_error(">COO_Table\nerror: Invalid coordinates for region" + std::to_string(n) + std::to_string(m));
	}

	for (int i = 0; i < data.size();++i)
	{

		if (data[i].x >= xmin && data[i].x <= xmax &&
			data[i].y >= ymin && data[i].y <= ymax)
		{
			result.push_back(data[i].formula);
		}
	}
}

void COO_SparseMatrix::insert(int i, int j, std::string formula)
{

	if (validCoordinates(i, j))
	{

		int idx = find_table_idx_in_array(i, j);
		if (idx != -1) //if exists, just update it
		{
			data[idx].formula = formula;
			return;
		}

		if (i > n - 1) n = i + 1;
		if (j > m - 1) m = j + 1;

		data.push_back(Cell(i, j, formula));
	}
	else
	{
		throw std::runtime_error(">COO_Table\nerror: Coordinates outside table!");
		return;
	}
}

bool COO_SparseMatrix::validCoordinates(int i, int j) const
{
	if (i >= 100000 || j >= 100000 || i < 0 || j < 0)
		return false;
	return true;
}

void COO_SparseMatrix::get_csv_size(std::ifstream& iFile, int& max_n, int& max_m)
{
	std::string cur_line, cell_val;
	int i = 0, j = 0;
	max_n = 0, max_m = 0;

	while (getline(iFile, cur_line))
	{

		std::string word;
		std::stringstream ss(cur_line);

		while (getline(ss, word, ';'))
		{
			++j;
			if (j > max_m) max_m = j;

			if (ss.peek() == '\n') ss.ignore();
		}

		j = 0;

		++i;
	}
	if (i > max_n) max_n = i;

}

int COO_SparseMatrix::find_table_idx_in_array(int i, int j) const
{
	if (validCoordinates(i, j))
	{
		for (int k = 0; k < data.size(); ++k)
		{
			if (data[k].x == j && data[k].y == i) //found element with such coords
			{
				return k;
			}
		}

		return -1;
	}
	else
	{
		throw std::runtime_error(">COO_Table\nerror: given coordinates too big or negative");
	}
}
