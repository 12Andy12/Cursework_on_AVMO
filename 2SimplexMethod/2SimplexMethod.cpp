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
#include "simplexTable.h"
#include "Rational.h"

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
bool operator!= (Rational f, int s)
{
	double c = double(double(f.a) / double(f.b));
	//cout << "a/b = " << c << "   s = " << s;
	if (c != s)
		return true;
	return false;
}

bool operator== (Rational f, int s)
{
	double c = double(double(f.a) / double(f.b));
	if (c == s)
		return true;
	return false;
}

bool operator< (Rational f, int s)
{
	double c = double(double(f.a) / double(f.b));
	if (c < s)
		return true;
	return false;
}

bool operator< (Rational f, Rational s)
{
	double c1 = double(double(f.a) / double(f.b));
	double c2 = double(double(s.a) / double(s.b));
	if (c1 < c2)
		return true;
	return false;
}

bool operator> (Rational f, Rational s)
{
	double c1 = double(double(f.a) / double(f.b));
	double c2 = double(double(s.a) / double(s.b));
	if (c1 > c2)
		return true;
	return false;
}

bool operator> (Rational f, int s)
{
	double c = double(double(f.a) / double(f.b));
	if (c > s)
		return true;
	return false;
}

bool operator>= (Rational f, Rational s)
{
	double c1 = double(double(f.a) / double(f.b));
	double c2 = double(double(s.a) / double(s.b));
	if (c1 >= c2)
		return true;
	return false;
}

bool operator>= (Rational f, int s)
{
	double c1 = double(double(f.a) / double(f.b));
	if (c1 >= s)
		return true;
	return false;
}

ostream& operator<< (ostream& out, Rational s)
{
	if (s.b == 1 || s.a == 0)
		return out << s.a;
	return out << s.a << "/" << s.b;
}

istream& operator>> (istream& out, Rational& s)
{
	double number;
	out >> number;
	std::stringstream ss;
	ss << std::setprecision(15) << number;
	std::string strNum = ss.str();
	size_t pos = strNum.find('.');
	int len = 0;
	s.b = 1;
	if (pos != strNum.npos)
	{
		len = strNum.size() - 1 - pos;
		s.a = number;

		for (int i = 0; i < len; ++i)
		{
			s.a *= 10;
			s.b *= 10;
		}
		//cout << " (" << s.a << " " << s.b<<") ";
	}
	else
	{
		s.a = number;
		//cout << " (" << s.a << " " << s.b << ") ";
	}
	return out;
}


template <typename T>
ostream& operator<< (ostream& out, vector<vector<T>> v)
{
	for (auto i : v) {
		for (auto j : i)
			out << setw(3) << j << ' ';
		out << '\n';
	}
	return out;
}



template <typename T>
ostream& operator<< (ostream& out, vector<T> v)
{
	for (auto i : v)
		out << setw(3) << i << ' ';
	return out;
}

ostream& operator<< (ostream& out, simplexTable t)
{
	SetConsoleTextAttribute(h, 4);
	out << "\n      1";

	for (int i = 0; i < t.colCount - 1; ++i)
		out << setw(3) << "x" << i + 1;

	SetConsoleTextAttribute(h, 15);
	out << "\n";
	for (int i = 0; i < t.table.size(); ++i) {
		SetConsoleTextAttribute(h, 4);
		if (i < t.rowCount)
			out << setw(3) << "x" << t.basisId[i] + 1;
		else if (i == t.rowCount)
			out << "   Z";
		else
			out << "  CO";

		SetConsoleTextAttribute(h, 15);

		for (int j = 0; j < t.colCount; ++j)
		{
			if (i == t.rowCount + 1 && t.table[i][j] == 0)
				SetConsoleTextAttribute(h, 0);
			else
				SetConsoleTextAttribute(h, 15);

			out << setw(3) << t.table[i][j] << ' ';
		}

		out << '\n';
	}
	SetConsoleTextAttribute(h, 15);
	
	//out << "Admin = " << t.admin << "\n";

	if (t.COinvalid)
	{
		SetConsoleTextAttribute(h, 3);
		out << "CO is dead\n";
	}

	if (!t.z_isGood())
	{
		SetConsoleTextAttribute(h, 3);
		out << "\nHaven`t solution\n";
	}

	if (!t.isOptimal())
	{
		SetConsoleTextAttribute(h, 3);
		out << "\nSolution isn`t optimal\n";
	}
	else
	{
		t.calculateResult();
		SetConsoleTextAttribute(h, 2);
		out << "\nSolution is optimal\n";
		Rational r = t.result;
		out << "Z(" << t.resultZ << ") = " << t.result;
	}

	SetConsoleTextAttribute(h, 15);
	//out << t.rowAdmin << " " << t.colAdmin << "\n";
	return out;
}



int main()
{

	simplexTable tab("3.txt");
	tab.findBasis();
	tab.initTable();
	tab.initAdmin();
	if (tab.COinvalid)
		return 0;
	do {
		cout << tab;
		tab.calculateTable();
	} while (!tab.isOptimal() && tab.z_isGood() && tab.initAdmin());
	
	cout << tab;


	/*while (!tab.isOptimal())
	{
		tab.findBasis();
		tab.initTable();
		tab.initAdmin();
		tab.calculateTable();
		cout << tab;
	}
	*/

}
