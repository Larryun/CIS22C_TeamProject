// main test driver for CryptoCurrency Database
// Modified by: Tommy Vu, Meng Leong Un, Michael Wong

#include "BinarySearchTree.h"  // BST ADT 
#include "Crypto.h"
#include "HashTable.h"
#include "StackADT.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string COMMANDS = "ABCDEFGH";

// Function Prototypes
void displayMenu();
void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
void deleteManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&));
void secondaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&));
void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&));
bool treeSearch(BinarySearchTree<CryptocurrencyPtr>* tree, string key, CryptocurrencyPtr& result);
void insertAll(Cryptocurrency& crypto, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void printCrypto(Cryptocurrency& crypto);
void printCrypto(CryptocurrencyPtr& cryptoPtr);
void printHeader();
int getNumberOfLines(string filename);
void buildTree(string filename, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
bool getInput(char& command);
bool validateInput(char& command);
bool isAllAlpha(string& s);
void runCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr);

int main()
{
	string inputFileName = "CryptoList.txt";
	int lines = getNumberOfLines(inputFileName);

	Stack<CryptocurrencyPtr> undoStack;

	BinarySearchTree <CryptocurrencyPtr>* primaryTree = new BinarySearchTree <CryptocurrencyPtr>;
	BinarySearchTree <CryptocurrencyPtr>* secondaryTree = new BinarySearchTree <CryptocurrencyPtr>;

	HashTable<CryptocurrencyPtr>* hashArr = new HashTable<CryptocurrencyPtr>(lines); //Initialize the hashTable
	buildTree(inputFileName, primaryTree, secondaryTree, hashArr);
	hashArr->printTable(printHeader, printCrypto);

	// Used as a place to hold User Input for insert.
	// char insert[30];
	int counter = 1;

	displayMenu();

	char command;
	string searchKey;
	do
	{
		if (getInput(command))
			runCommand(command, primaryTree, secondaryTree, hashArr);

	} while (command != 'H');

	cout << "Bye! Thanks for using the Cryptocurrency Database!" << endl;
	return 0;
}

/*
	getInput:
		return true if input is correct otherwise return false,
		and read command and searchKey if avaliable
*/
bool getInput(char& command)
{
	cout << "Please input an operation: \n";
	cin >> command;
	command = toupper(command);
	// Ignore all input after the first charater
	cin.clear(); cin.ignore(100, '\n');
	return validateInput(command);
}

/*
	validateInput:
		return true if input is correct otherwise return false
*/
bool validateInput(char& command)
{
	if (COMMANDS.find(command) != -1)
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
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr)
{
	switch (runCommand)
	{
	case 'A':
		insertManager(primaryTree, secondaryTree, hashArr);
		break;
	case 'B':
		deleteManager(primaryTree, secondaryTree, hashArr);
		break;
	case 'C':
		primaryKeySearchManager(primaryTree, printCrypto);
		break;
	case 'D':
		secondaryKeySearchManager(secondaryTree, printCrypto);
		break;
	case 'E':
		hashTableSearchManager(hashArr, printCrypto);
		break;
	case 'F':
		// Temporary testing
		primaryTree->inOrder(printCrypto);
		cout << "-------" << endl;
		secondaryTree->inOrder(printCrypto);
		break;
	case 'G':
		break;
	case 'H':
		break;
	case 'I':
		break;
	}
}

// Displays menu.
void displayMenu()
{
	// add a undo delete
	cout << "Welcome to our Cryptocurrency Database! Please select one of the following commands!" << endl;
	cout << "" << endl;
	cout << "A - Add new a Cryptocurrency " << endl;
	cout << "B - Delete a Cryptocurrency" << endl;
	cout << "C - Search for a Cryptocurrency with Primary Key in BST" << endl;
	cout << "D - Search for a Cryptocurrency with Secondary Key in BST" << endl;
	cout << "E - Search for a Cryptocurrency with Primary Key in Hash Table" << endl;
	cout << "F - See Cryptocurrencies sorted in various ways" << endl;
	cout << "G - Write the Cryptocurrencies to a file" << endl;
	cout << "H - See the statistics" << endl;
	cout << "I - Help to show the menu" << endl;
	cout << "J - Exit" << endl;
	cout << endl;
}

/* insertAll:
		insert the cryptocurrenty to primaryTree, secondaryTree and hash table
*/
void insertAll(Cryptocurrency& crypto, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	string name = crypto.getName();
	string alg = crypto.getAlg();
	CryptocurrencyPtr* nameAsKey = new CryptocurrencyPtr(&crypto, name);
	CryptocurrencyPtr* algAsKey = new CryptocurrencyPtr(&crypto, alg);
	primaryTree->insert(*nameAsKey);
	secondaryTree->insert(*algAsKey);
	hashArr->insertItem(name, nameAsKey);
}

int getNumberOfLines(string filename)
{
	ifstream inFile;
	string tmp;
	int count = 0;
	inFile.open(filename);
	if (!inFile)
	{
		cout << "Error opening the input file: \"" << filename << "\"" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		while (!inFile.eof())
		{
			getline(inFile, tmp);
			count++;
		}
	}
	inFile.close();
	return count-1;

}

// Builds a Binary Search Tree from a text file.
void buildTree(string filename, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	
	ifstream inFile;
	string algor, founder, name;
	long long supply;
	int year;
	double price;
	Cryptocurrency* crypto;

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
		insertAll(*crypto, primaryTree, secondaryTree, hashArr);
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
	cout << setw(20) << crypto.getName();
	cout << setw(15) << crypto.getAlg();
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

void printHeader()
{
	cout << left;
	cout << setw(20) << "Name";
	cout << setw(15) << "Algorithm";
	cout << setw(4) << "Date Founded" << endl;
	cout << "====================================================" << endl;
}
/* This is not needed. All hashing processes are done in the hashTable class
void hashNumber(int len)
{

}
*/

// Add a validate function if needed
// bool getInsertData(T& data, string message, bool validate(T&))
template<typename T>
bool getInsertData(T& data, string message)
{
	cout << message << endl;
	cin >> data;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(80, '\n');
		return false;
	}
}

void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* crypto;
	int checker = 0;
	string name, alg, founder;
	//char name [30];
	//char alg[20];
	//char founder[30];
	int supply, year;
	double price;

	// NOTE: Input conversion hasn't been implemented, so __Bitcoin would print the two spaces before Bitcoin still.
	
	cout << "What is the name of the Cryptocurrency?" << endl;
	getline(cin, name);
	//cin.getline(name, 29);

	cout << "What is " << name << "'s Algorithm?" << endl;
	getline(cin, alg);
	//cin.getline(alg, 19);

	while (!getInsertData(year, "What year was " + name + " founded?"))
	{
		cout << "Please enter a valid year." << endl;
	}

	while (!getInsertData(supply, "What is the supply for " + name + " ?"))
	{
		cout << "Please enter a valid supply." << endl;
	}

	while (!getInsertData(price, "How much is " + name + " worth?"))
	{
		cout << "Please enter a valid price." << endl;
	}

	//do // Do while loop makes sure you put the correct input for int year and forces you input an integer.
	//{
	//	checker = 0;
	//	cout << "How much is " << name << " worth?" << endl;
	//	cin >> price;
	//	if (cin.fail())
	//	{
	//		cout << "Please enter a valid price." << endl;
	//		checker = 1;
	//		cin.clear();
	//		cin.ignore(80, '\n');
	//	}
	//} while (checker == 1);

	cin.ignore();
	cout << "Who founded " << name << "?" << endl;
	getline(cin, founder);

	crypto = new Cryptocurrency(name, alg, supply, year, price, founder);
	insertAll(*crypto, primaryTree, secondaryTree, hashArr);

	cout << "The Crypto has been inserted!" << endl;
	cout << endl;

	// for testing purposes
	 cout << name << " " << alg << " " << supply << " " << year << " " << price << " " << founder << endl;
		 
}

void deleteManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	CryptocurrencyPtr cryptoPtr;
	string name, alg;
	cout << "Enter the name of the Cryptocurrency you wish to delete" << endl;
	getline(cin, name);
	// Check if cryptocurreny exists in the primary tree
	if (treeSearch(primaryTree, name, cryptoPtr))
	{
		alg = cryptoPtr.getCrypto()->getAlg();
		primaryTree->remove(CryptocurrencyPtr(nullptr, name));
		secondaryTree->remove(CryptocurrencyPtr(nullptr, alg));
		hashArr->deleteItem(name);
		cout << name << " has been deleted from both tree and hash table" << endl;
	}
	else
	{
		cout << "Cryptocurency not found!" << endl;
	}
}

bool treeSearch(BinarySearchTree<CryptocurrencyPtr>* tree, string key, CryptocurrencyPtr& result)
{
	if (tree->getEntry(CryptocurrencyPtr(nullptr, key), result))
		return true;
	else
		return false;
}

bool hashTableSearch(HashTable<CryptocurrencyPtr>* table, string key, CryptocurrencyPtr& result)
{
	if (table->findItem(key, result))
		return true;
	else
		return false;
}

/* SearchManager:
		The template for searchManger in different data structure
*/
template<class T>
void searchManager(T* dataStructure, string& key, void printCrypto(CryptocurrencyPtr&), bool searchFunction(T*, string, CryptocurrencyPtr&))
{
	CryptocurrencyPtr resultCryptoPtr;

	// Change the hashArr parameter to primary if you want to search in tree
	//if (treeSearch(tree, name, resultCryptoPtr))
	if (searchFunction(dataStructure, key, resultCryptoPtr))
	{
		cout << "Data Found!" << endl;
		printHeader();
		printCrypto(resultCryptoPtr);
		cout << endl;
	}
	else {
		cout << "Not Found!" << endl;
	}
}

void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin,name);
	searchManager<BinarySearchTree<CryptocurrencyPtr>>(tree, name, printCrypto, treeSearch);
	//treeSearchManager(tree, name, printCrypto);
}

void secondaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string alg;
	cout << "Please input the algorithm of the Cryptocurrency: " << endl;
	getline(cin, alg);
	searchManager<BinarySearchTree<CryptocurrencyPtr>>(tree, alg, printCrypto, treeSearch);
	//treeSearchManager(tree, alg, printCrypto);
}

void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin, name);
	searchManager<HashTable<CryptocurrencyPtr>>(table, name, printCrypto, hashTableSearch);
	//treeSearchManager(tree, alg, printCrypto);
}