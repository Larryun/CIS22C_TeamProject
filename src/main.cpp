// main test driver for CryptoCurrency Database
// Modified by: Tommy Vu, Meng Leong Un

#include "BinarySearchTree.h"  // BST ADT 
#include "Crypto.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string COMMANDS = "ABCDEFGH";

// Function Prototypes
void displayMenu();

void printCrypto(Cryptocurrency& crypto);
void printCrypto(CryptocurrencyPtr& cryptoPtr);
void buildTree(string filename, BinarySearchTree<Cryptocurrency>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree);
bool getInput(char& command, string& searchKey);
bool validateInput(char& command, string& searchKey);
bool isAllAlpha(string& s);
void runCommand(char& runCommand,
				BinarySearchTree<Cryptocurrency>* primaryTree,
				BinarySearchTree<CryptocurrencyPtr>* secondaryTree);

int main()
{
	string inputFileName = "CryptoList.txt";

	BinarySearchTree <Cryptocurrency>* primaryTree = new BinarySearchTree <Cryptocurrency>;
	BinarySearchTree <CryptocurrencyPtr>* secondaryTree = new BinarySearchTree <CryptocurrencyPtr>;

	buildTree(inputFileName, primaryTree, secondaryTree);

	// Cryptocurrency used for returning and comparing
	Cryptocurrency dummyCrypto;
	Cryptocurrency returnedCrypto;

	// Used as a place to hold User Input for insert.
	// char insert[30];
	int counter = 1;

	// Used to take input and keeps running loop until Q
	displayMenu();
	
	char command;
	string searchKey;
	do
	{
		searchKey = "";
		if (getInput(command, searchKey))
			runCommand(command, primaryTree, secondaryTree);
			//cout << command << " " << searchKey << endl;
	} while (command != 'H');

	cout << "Bye! Thanks for using the Cryptocurrency Database!" << endl;
	return 0;
}

/*
	getInput:
		return true if input is correct otherwise return false, 
		and read command and searchKey if avaliable
*/
bool getInput(char& command, string& searchKey)
{
	cout << "Please input an operation: \n";
	cin >> command;
	command = toupper(command);
	// Ignore all input after the first charater
	cin.clear(); cin.ignore(100, '\n');

	// If command is search
	if (command == 'C')
	{
		cout << "Please input the key: \n";
		getline(cin, searchKey);

		// Convert all charater in string into upper
		for (char& c : searchKey)
			c = toupper(c);
	}

	return validateInput(command, searchKey);
}

/*
	validateInput:
		return true if input is correct otherwise return false
*/
bool validateInput(char& command, string& searchKey)
{
	if (COMMANDS.find(command) != -1 && isAllAlpha(searchKey))
		return true;
	else
		return false;
}

/*
	isAllAlpha:
		returns true is all char in string s is alphabet otherwise return false
*/
bool isAllAlpha(string& s)
{
	if (s.length() != 0)
	{
		for (char c : s)
			if (!isalpha(c)) return false;
	}
	return true;
}

/*
	runCommand:
		run the coresponding function with given command
*/
void runCommand(char& runCommand,
				BinarySearchTree<Cryptocurrency>* primaryTree,
				BinarySearchTree<CryptocurrencyPtr>* secondaryTree)
{
	switch (runCommand)
	{
	case 'A':
		// doSomething()
		break;
	case 'B':
		break;
	case 'C':
		break;
	case 'D':
		// Temparay testing
		primaryTree->breadthTrav(printCrypto);
		cout << "-------" << endl;
		secondaryTree->breadthTrav(printCrypto);
		break;
	case 'E':
		break;
	case 'F':
		break;
	case 'G':
		break;
	case 'H':
		break;
	}
}

// Displays menu.
void displayMenu()
{
	cout << "Welcome to our Cryptocurrency Database! Please select one of the following commands!" << endl;
	cout << "" << endl;
	cout << "A - Add new a Cryptocurrency " << endl;
	cout << "B - Delete a Cryptocurrency" << endl;
	cout << "C - Search for a Cryptocurrency" << endl;
	cout << "D - See Cryptocurrencies sorted in various ways" << endl;
	cout << "E - Write the Cryptocurrencies to a file" << endl;
	cout << "F - See the statistics" << endl;
	cout << "G - Help to show the menu" << endl;
	cout << "H - Exit" << endl;
	cout << endl;
}

// Builds a Binary Search Tree from a text file.
void buildTree(string filename, BinarySearchTree<Cryptocurrency>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree)
{
	ifstream inFile;
	string algor, founder, name;
	long long supply;
	int year;
	double price;
	Cryptocurrency* crypto;
	CryptocurrencyPtr cryptoPtr;

	inFile.open(filename);
	if (!inFile)
	{
		cout << "Error opening the input file: \"" << filename << "\"" << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile >> algor >> supply >> founder >> year >> price)
	{
		// To ignore the space after price
		inFile.ignore();
		getline(inFile, name);
		//cout << algor << " " << supply << " " << founder << " " << year << " " << price << " " << name << endl;
		crypto = new Cryptocurrency(name, algor, supply, year, price, founder);

		// Set the name of algorithm as the secondary key
		cryptoPtr = CryptocurrencyPtr(crypto, algor);
		primaryTree->insert(*crypto);
		secondaryTree->insert(cryptoPtr);
	}
	inFile.close();
}

/*
	printCrypto:
		print infomations of the cryptocurrnecy in given format
*/
void printCrypto(Cryptocurrency& crypto)
{
	// Subject to change
	cout << left;
	cout << setw(15) << crypto.getAlg();
	cout << setw(20) << crypto.getName();
	cout << setw(4) << crypto.getDate() << endl;
}

/*
	printCrypto:
		overloaded function for printting CryptocurrencyPtr
		print infomations of the cryptocurrnecy in given format
*/
void printCrypto(CryptocurrencyPtr& cryptoPtr)
{
	// Dereferencing the CryptocurrencyPtr then call the regular printCrypto function
	printCrypto(*(cryptoPtr.crypto));
}
