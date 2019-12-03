// Implementation file for the City class
// Written By: A. Student
// Changed By: Tommy Vu
// IDE: Visual Studios

#include <iostream>
#include "Crypto.h"
#include <iomanip>
using namespace std;

//**************************************************
// Constructor
//**************************************************
Cryptocurrency::Cryptocurrency()
{
	coinName = "";
	alg = "";
	supply = 0;
	dateFound = 0;
	price = 0;
	founder = "";
}

//**************************************************
// Overloaded << Operator
//Comment: Used to directly cout objects in one stream. 
//**************************************************

/*
ostream& operator<<(ostream& strm, const Cryptocurrency& obj)
{
	strm << setw(14) << "State: " << obj.getName() << endl;
	strm << setw(14) << "Name: " << obj.getAlg() << endl;
	strm << setw(14) << "Year Founded: " << obj.getDate() << endl;

	return strm;
}
*/

//************************************************************
// Overloaded < operator       
// Comment: Used to compare object names with each other directly.
//************************************************************

bool Cryptocurrency::operator < (const Cryptocurrency& obj)
{
	bool status;

	if (coinName < obj.getName())
		status = true;
	else
		status = false;
	return status;
}

//************************************************************
// Overloaded > operator       
// Comment: Used to compare object names with each other directly.
//************************************************************

bool Cryptocurrency::operator > (const Cryptocurrency& obj)
{
	bool status;

	if (coinName > obj.getName())
		status = true;
	else
		status = false;
	return status;
}

//*************************************************************
// Overloaded == operator. Returns true if the current object *
// is set to a value equal to that of right.                  *
//*************************************************************

bool Cryptocurrency::operator == (const Cryptocurrency& obj)
{
	bool status;

	if (coinName == obj.getName())
		status = true;
	else
		status = false;

	return status;
}

//**************************************************
// Overloaded Constructor
//**************************************************
Cryptocurrency::Cryptocurrency(string n, string a, long long s, int d, double p, string f)
{
	coinName = n;
	alg = a;
	supply = s;
	dateFound = d;
	price = p;
	founder = f;
}

//**************************************************
// Donstructor
//**************************************************
Cryptocurrency::~Cryptocurrency()
{

}

//***********************************************************
// Displays the values of the City object member variables
//***********************************************************
void Cryptocurrency::display()
{
	cout << right << setw(14) << "Coin: " << coinName << endl;
	cout << right << setw(14) << "Alg: " << alg << endl;
	cout << right << setw(14) << "Supply: " << supply << endl;
	cout << right << setw(14) << "Year Founded: " << dateFound << endl;
	cout << right << setw(14) << "Price: " << price << endl;
	cout << right << setw(14) << "Founder: " << founder << endl;
}

