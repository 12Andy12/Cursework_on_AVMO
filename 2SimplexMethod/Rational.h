#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

#include <sstream>

using namespace std;
class Rational
{

public:
	long double a, b;

	Rational(long double chislit = 0, long double znamenat = 1) :a(chislit), b(znamenat)
	{
		a = chislit;
		if (znamenat != 0)
			b = znamenat;
		else
		{
			cout << "Ne mogu podelit na ZERO\n";
			exit(1);
		}
	}

	Rational operator+ (Rational);
	Rational operator+= (Rational);
	Rational operator- (Rational);
	Rational operator* (Rational);
	Rational operator* (int);
	Rational operator/ (Rational);
	Rational operator/= (Rational);
	Rational operator= (Rational);
	Rational operator= (double);

	operator double()
	{
		double res = double(a) / double(b);
		return res;
	}

	void print()
	{
		cout << a << "/" << b;
	}

	long long Nod(long long, long long);
};

Rational abs(Rational a);