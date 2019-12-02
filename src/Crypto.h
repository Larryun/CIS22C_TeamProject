// Specification file for the Cryptocurrency class
// Written By: A. Student
// Changed By: Tommy Vu
// IDE: Visual Studio


#ifndef CRYPTOCURRENCY_H
#define CRYPTOCURRENCY_H
#include <string> 
#include <iostream>

using std::string;
class Cryptocurrency;

// Function Overloaded Prototype
//ostream &operator << (ostream &, const Cryptocurrency&);

class Cryptocurrency
{
private:
	string coinName;
	string alg;
	int supply;
	int dateFound;
	double price;
	string founder;

public:
	Cryptocurrency();
	Cryptocurrency(string n, string a, int s, int d, double p, string f);
	~Cryptocurrency();

	// Setters
	void setName(string n) { coinName = n; }
	void setAlg(string a) { alg = a; }
	void setSupply(int s) { supply = s; }
	void setDate(int d) { dateFound = d; }
	void setPrice(double p) { price = p; }
	void setFounder(string f) { founder = f; }

	// Getters
	string getName() const { return coinName; }
	string getAlg() const { return alg; }
	int getSupply() const { return supply; }
	int getDate() const { return dateFound; }
	double getPrice() const { return price; }
	string getFounder() const { return founder; }

	// Overloaded
	bool operator < (const Cryptocurrency& obj);
	bool operator > (const Cryptocurrency& obj);
	bool operator == (const Cryptocurrency& obj);

	// Friends
	//friend ostream& operator << (ostream&, const Cryptocurrency&);

	// Etc functions
	void display();
};

struct CryptocurrencyPtr{
	Cryptocurrency* crypto;
	string key;

	CryptocurrencyPtr(){};
	CryptocurrencyPtr(Cryptocurrency* crypto_, string key_) { crypto = crypto_; key = key_; };

	bool operator < (const CryptocurrencyPtr& second) const
	{
		return (this->key < second.key);
	}

	bool operator > (const CryptocurrencyPtr& second) const
	{
		return (this->key > second.key);
	}

	bool operator == (const CryptocurrencyPtr& second) const
	{
		return (this->key == second.key);
	}

};
#endif
