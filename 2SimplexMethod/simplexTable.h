#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include <string>
#include "Rational.h"

using namespace std;

class simplexTable
{
public:
	int rowCount = 3;
	int colCount = 6;

	int rowAdmin = 0; // координаты разрешающего элемента
	int colAdmin = 0;

	Rational admin = 0;
	bool COinvalid = false;

	vector<int> basisId;
	vector<double> Z;
	vector<vector<double>> matrix;

	double result;
	vector<Rational> resultZ;
	simplexTable(string fileName)
	{
		ifstream file(fileName);
		int n, m;
		file >> n >> m;
		rowCount = n;
		colCount = m;
		matrix.resize(rowCount);
		for (int i = 0; i < rowCount; ++i)
		{
			matrix[i].resize(colCount);
			for (int j = 0; j < colCount; ++j)
				file >> matrix[i][j];
		}
		Z.resize(colCount);
		for (int i = 0; i < colCount; ++i)
			file >> Z[i];

	}

	vector < vector < Rational >> table;

	void findBasis()
	{
		for (int j = 0; j < colCount - 1; ++j)
		{
			int countZero = 0;
			int countOnes = 0;

			for (int i = 0; i < rowCount; ++i)
			{
				if (matrix[i][j] == 1)
					countOnes++;
				if (matrix[i][j] == 0)
					countZero++;
			}

			if (countOnes == 1 && countZero == rowCount - 1)
				basisId.push_back(j);

		}
	}

	void initTable()
	{

		table.resize(rowCount + 2);
		for (int i = 0; i < rowCount; ++i)
		{
			table[i].resize(colCount);
			table[i][0] = matrix[i][colCount - 1];
		}

		for (int i = 0; i < rowCount; ++i)
			for (int j = 0; j < colCount - 1; ++j)
				table[i][j + 1] = matrix[i][j];


		table[rowCount].resize(colCount);
		table[rowCount + 1].resize(colCount);


		table[rowCount][0] = Z[0];
		for (int i = 1; i < Z.size(); ++i)
			table[rowCount][i] = Z[i] * -1;
	}

	void initCO()
	{
		for (int i = 1; i < colCount; ++i)
		{
			if (table[rowAdmin][i] >= 0)
			{
				table[rowCount + 1][i] = 0;
				continue;
			}

			table[rowCount + 1][i] = table[rowCount][i] / table[rowAdmin][i];
		}
	}

	bool initAdmin()
	{
		Rational min = 0;
		int minID = -1;
		for (int i = 0; i < rowCount; ++i)
		{
			if (table[i][0] < min)
			{
				for (int j = 1; j < colCount; ++j)
				{
					if (table[i][j] < 0)
					{
						min = table[i][0];
						minID = i;
						break;
					}
				}
			}
		}
		rowAdmin = minID;

		if (rowAdmin == -1)
		{
			COinvalid = true;
			for (int i = 1; i < colCount; ++i)
			{	
				table[rowCount + 1][i] = 0;
			}
			return false;
		}
		//cout << "rowA = " << rowAdmin << "\n";
		initCO();
		min = 100000;
		minID = -1;
		for (int i = 1; i < colCount; ++i)
		{
			if (table[rowCount + 1][i] < min && table[rowCount+1][i] < 0)
			{
				min = table[rowCount + 1][i];
				minID = i;
			}
		}
		colAdmin = minID;
		//cout << "rowA = " << rowAdmin << " colA = " << colAdmin << "\n";
		if (colAdmin == -1)
		{
			COinvalid = true;
			return false;
		}
			
		admin = table[rowAdmin][colAdmin];

		return true;
	}

	Rational spanchBob(int idrow, int idcol)
	{
		Rational res = table[idrow][idcol] - (table[rowAdmin][idcol] * table[idrow][colAdmin] / admin);
		//if (idcol == 5)
		//	cout << "idrow = " << idrow << "; idcol = " << idcol << "  " << table[idrow][idcol] << " - (" << table[rowAdmin][idcol] << " * " << table[idrow][colAdmin] << " / " << admin << ");\n";
		return res;

	}

	void calculateTable()
	{
		vector < vector < Rational >> newtable = table;

		basisId[rowAdmin] = colAdmin - 1;

		for (int j = 0; j < colCount; ++j)
		{

			for (int i = 0; i < rowCount + 1; ++i)
			{
				if (i == rowAdmin)
					continue;
				newtable[i][j] = spanchBob(i, j);
			}
		}

		for (auto& i : newtable[rowAdmin])
			i /= admin;

		table = newtable;
		if (isOptimal())
			for (auto& i : table[rowCount + 1])
				i = 0;
	}


	bool isOptimal()
	{
		for (int i = 0; i < rowCount; ++i)
			if (table[i][0] < 0)
				return false;
		return true;
	}

	bool z_isGood()
	{
		for (int i = 1; i < colCount; ++i)
			if (table[rowCount][i] < 0)
				return false;

		return true;
	}

	void calculateResult()
	{
		result = 0;
		resultZ.resize(colCount - 1);
		result += Z[0];
		//cout << "result = " << Z[0];

		for (int i = 0; i < rowCount; ++i)
		{
			result += double(Z[basisId[i] + 1]) * double(table[i][0]);
			//cout << " + " << Z[basisId[i] + 1] << " * " << table[i][0];
			resultZ[basisId[i]] = table[i][0];
		}
	}



};

