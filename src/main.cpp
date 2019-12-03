// main test driver for CryptoCurrency Database
// Modified by: Tommy Vu, Meng Leong Un, Michael Wong

#include "BinarySearchTree.h" 
#include "Crypto.h"
#include "HashTable.h"
#include "StackADT.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string MAIN_COMMANDS = "ABCDEFGHI";
const string SEARCH_COMMANDS = "ABCDE";
const string LIST_COMMANDS = "ABCDEF";
const string NOT_FOUND = "Cryptocurrency Not Found!";

// Menu printing function
void displayMenu();
void displaytSearchSubMenu();
void displaytListSubMenu();

// Build tree function
void buildTree(string filename, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);

// Manager Functions
void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
void deleteManager(Stack<Cryptocurrency*>* deleteStack, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
void searchManager( BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&));
void secondaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&));
void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&));
void listManager(BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);

// Main Search Function
template<class T>
void doSearch(T* dataStructure, string& key, void printCrypto(CryptocurrencyPtr&), bool searchFunction(T*, string, CryptocurrencyPtr&));
// Search methods for tree and hash table
bool treeSearch(BinarySearchTree<CryptocurrencyPtr>* tree, string key, CryptocurrencyPtr& result);
bool hashTableSearch(HashTable<CryptocurrencyPtr>* table, string key, CryptocurrencyPtr& result);

// Screen Output function
void printCrypto(Cryptocurrency& crypto);
void printCrypto(CryptocurrencyPtr& cryptoPtr);
void printHeader();
void printHashTableStats(HashTable<CryptocurrencyPtr>* hashArr);
string divider(int len, char symbol);

// Input Function
bool getInput(char& command, const string& COMMANDS);
template<typename T> bool getInsertData(T& data, string message);
template<typename T> bool getInsertData(T& data, string message, bool validate(T&));

// Run command functions for main, searching and listing
void runCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr,
	Stack<Cryptocurrency*>* deleteStack);

void runSearchCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr);

void runListCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr);

// Validation Function
bool yearValidator(int& year);
bool supplyValidator(int& supply);
bool priceValidator(double& price);
bool validateInput(char& command, const string& COMMANDS);
bool isAllAlpha(string& s);

// Trivial Functions
bool isKeyExists(string& key, BinarySearchTree<CryptocurrencyPtr>* tree);
bool insertAll(Cryptocurrency& crypto, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
int getNumberOfLines(string filename);
void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void trimSpaces(string& line);

int main()
{
	string inputFileName = "CryptoList.txt";
	int lines = getNumberOfLines(inputFileName);

	Stack<Cryptocurrency*>* undoStack = new Stack<Cryptocurrency*>();

	BinarySearchTree <CryptocurrencyPtr>* primaryTree = new BinarySearchTree <CryptocurrencyPtr>;
	BinarySearchTree <CryptocurrencyPtr>* secondaryTree = new BinarySearchTree <CryptocurrencyPtr>;

	HashTable<CryptocurrencyPtr>* hashArr = new HashTable<CryptocurrencyPtr>(lines); //Initialize the hashTable
	buildTree(inputFileName, primaryTree, secondaryTree, hashArr);
	//hashArr->printTable(printHeader, printCrypto);

	// Used as a place to hold User Input for insert.
	// char insert[30];
	int counter = 1;

	displayMenu();

	char command;
	string searchKey;
	do
	{
		if (getInput(command, MAIN_COMMANDS))
			runCommand(command, primaryTree, secondaryTree, hashArr, undoStack);

	} while (command != MAIN_COMMANDS[MAIN_COMMANDS.length()-1]);

	cout << "Bye! Thanks for using the Cryptocurrency Database!" << endl;
	return 0;
}

/*
	getInput:
		return true if input is correct otherwise return false,
		and read command and searchKey if avaliable
*/
bool getInput(char& command, const string& COMMANDS)
{
	cout << "Please input an operation: \n";
	cin >> command;
	command = toupper(command);
	// Ignore all input after the first charater
	cin.clear(); cin.ignore(100, '\n');
	return validateInput(command, COMMANDS);
}

/*
	validateInput:
		return true if input is correct otherwise return false
*/
bool validateInput(char& command, const string& COMMANDS)
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
	HashTable<CryptocurrencyPtr>* hashArr,
	Stack<Cryptocurrency*>* deleteStack)
{
	switch (runCommand)
	{
	case 'A':
		insertManager(primaryTree, secondaryTree, hashArr);
		break;
	case 'B':
		deleteManager(deleteStack, primaryTree, secondaryTree, hashArr);
		break;
	case 'C':
		searchManager(primaryTree, secondaryTree, hashArr);
		// Display the main menu after exit from the search manager
		displayMenu();
		break;
	case 'D':
		listManager(primaryTree, secondaryTree, hashArr);
		displayMenu();
		// Display the main menu after exit from the search manager
		break;
	case 'E':
		// Write to file
		break;
	case 'F':
		// Show statistics
		printHashTableStats(hashArr);
		break;
	case 'G':
		// Undo delete
		undo(deleteStack, primaryTree, secondaryTree, hashArr);
		break;
	case 'H':
		displayMenu();
		break;
	case 'I':
		//exit();
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
	cout << "C - Show Search sub-menu" << endl;
	cout << "D - Show Lists sub-menu" << endl;
	cout << "E - Write the Cryptocurrencies to a file" << endl;
	cout << "F - See the statistics" << endl;
	cout << "G - Undo Delete" << endl;
	cout << "H - Help to show the menu" << endl;
	cout << "I - Exit" << endl;
	cout << endl;
}

void displaytSearchSubMenu()
{
	cout << "Sub-menu for search operation"<< endl;
	cout << "" << endl;
	cout << "A - Search for a Cryptocurrency with Primary Key in BST" << endl;
	cout << "B - Search for a Cryptocurrency with Secondary Key in BST" << endl;
	cout << "C - Search for a Cryptocurrency with Primary Key in Hash Table" << endl;
	cout << "D - Help to show the menu" << endl;
	cout << "E - Exit" << endl;
	cout << endl;
}

void displaytListSubMenu()
{
	cout << "Sub-menu for Listing operation"<< endl;
	cout << "" << endl;
	cout << "A - List unsorted list" << endl;
	cout << "B - List data sorted by the primary key (name of the cryptocurrency)" << endl;
	cout << "C - List data sorted by the secondary key (algorithm of the cryptocurrency)" << endl;
	cout << "D - Speacial Print" << endl;
	cout << "E - Help to show the menu" << endl;
	cout << "F - Exit" << endl;
	cout << endl;
}


/* insertAll:
		insert the cryptocurrenty to primaryTree, secondaryTree and hash table
*/
bool insertAll(Cryptocurrency& crypto, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	string name = crypto.getName();
	string alg = crypto.getAlg();
	// return false is the key already exists in primary tree
	if (isKeyExists(name, primaryTree))
		return false;
	CryptocurrencyPtr* nameAsKey = new CryptocurrencyPtr(&crypto, name);
	CryptocurrencyPtr* algAsKey = new CryptocurrencyPtr(&crypto, alg);
	primaryTree->insert(*nameAsKey);
	secondaryTree->insert(*algAsKey);
	hashArr->insertItem(name, nameAsKey);
	return true;
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

/*
	divider:
		print a line of given symble in given len
*/
string divider(int len, char symbol)
{
	string t = "";
	for (int i = 0; i < len; i++)
		t += symbol;
	return t;
}

void printHeader()
{
	cout << endl << divider(55, '=') << endl;
	cout << left;
	cout << setw(20) << "Name";
	cout << setw(15) << "Algorithm";
	cout << setw(4) << "Date Founded" << endl;
	cout << divider(55, '=') << endl;
}
/* This is not needed. All hashing processes are done in the hashTable class
void hashNumber(int len)
{

}
*/

/*
	trimSpaces:
		remove all leading and trailing spaces
*/
void trimSpaces(string& line)
{
	const string whiteSpaces = " \t\v\r\n";
	size_t start = line.find_first_not_of(whiteSpaces);
	size_t end = line.find_last_not_of(whiteSpaces);
	line = line.substr(start, end - start + 1);
}

/* 
	getInsertData:
		promt the user and take the value, return false if fail
*/
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
	return true;
}

/* 
	getInsertData:
		promt the user and take the value, return false if fail and not validated
*/
template<typename T>
bool getInsertData(T& data, string message, bool validate(T&))
{
	if (getInsertData(data, message) && validate(data)) return true;
	return false;
}

bool yearValidator(int& year)
{
	return (year >= 1950 && year <= 2019);
}

bool supplyValidator(int& supply)
{
	return (supply >= 0);
}

bool priceValidator(double& price)
{
	return (price >= 0);
}

/*
	isKeyExists:
		chekc if the key is exists in the BST
*/
bool isKeyExists(string& key, BinarySearchTree<CryptocurrencyPtr>* tree)
{
	CryptocurrencyPtr tmp;
	return (tree->getEntry(CryptocurrencyPtr(nullptr, key), tmp));

}

void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* crypto;
	CryptocurrencyPtr tmp;
	int checker = 0;
	string name, alg, founder;
	int supply, year;
	double price;

	cout << "What is the name of the Cryptocurrency?" << endl;
	getline(cin, name);
	trimSpaces(name);
	if (isKeyExists(name, primaryTree))
	{
		cout << "The Cryptocurrency" + name + " already exists in the primary tree!" << endl;
		return;
	}

	cout << "What is " << name << "'s Algorithm?" << endl;
	getline(cin, alg);
	trimSpaces(alg);

	while (!getInsertData(year, "What year was " + name + " founded?", yearValidator))
	{
		cout << "Please enter a valid year." << endl;
	}

	while (!getInsertData(supply, "What is the supply for " + name + "?", supplyValidator))
	{
		cout << "Please enter a valid supply." << endl;
	}

	while (!getInsertData(price, "How much is " + name + " worth?", priceValidator))
	{
		cout << "Please enter a valid price." << endl;
	}

	cin.ignore();
	cout << "Who founded " << name << "?" << endl;
	getline(cin, founder);
	trimSpaces(founder);

	crypto = new Cryptocurrency(name, alg, supply, year, price, founder);
	insertAll(*crypto, primaryTree, secondaryTree, hashArr);

	cout << "The Cryptocurrency " + name + " has been inserted!" << endl;
	cout << endl;

	// for testing purposes
	// cout << name << " " << alg << " " << supply << " " << year << " " << price << " " << founder << endl;
		 
}

/*
	deleteManager:
		handling deletion and taking input from user
		push deleted item to undo stack
		and remove the item from hash table, and two trees
*/
void deleteManager(Stack<Cryptocurrency*>* undoStack, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	CryptocurrencyPtr cryptoPtr;
	string name, alg;
	cout << "Enter the name of the Cryptocurrency you wish to delete" << endl;
	getline(cin, name);
	trimSpaces(name);
	// Check if cryptocurreny exists in the primary tree
	if (treeSearch(primaryTree, name, cryptoPtr))
	{
		// Push to undo stack
		undoStack->push(cryptoPtr.getCrypto());
		alg = cryptoPtr.getCrypto()->getAlg();
		// Remove the node form primary tree name as key
		primaryTree->remove(CryptocurrencyPtr(nullptr, name));
		// Remove the node form secondary tree with algorithm as key 
		secondaryTree->remove(CryptocurrencyPtr(nullptr, alg));
		// Remove the node form secondary tree with name
		hashArr->deleteItem(name);
		cout << name << " has been deleted from both tree and hash table" << endl;
	}
	else
	{
		cout << NOT_FOUND << endl;
	}
}

/*
	treeSearch:
		search in BST, return ture if found, else false
*/
bool treeSearch(BinarySearchTree<CryptocurrencyPtr>* tree, string key, CryptocurrencyPtr& result)
{
	if (tree->getEntry(CryptocurrencyPtr(nullptr, key), result))
		return true;
	else
		return false;
}

/*
	hashTableSearch:
		search in hash table, return ture if found, else false
*/
bool hashTableSearch(HashTable<CryptocurrencyPtr>* table, string key, CryptocurrencyPtr& result)
{
	if (table->findItem(key, result))
		return true;
	else
		return false;
}

/*
	runSearchManager:
		handling calling function with corresponding command
*/
void runSearchCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr)
{
	switch (runCommand)
	{
	case 'A':
		primaryKeySearchManager(primaryTree, printCrypto);
		break;
	case 'B':
		secondaryKeySearchManager(secondaryTree, printCrypto);
		break;
	case 'C':
		hashTableSearchManager(hashArr, printCrypto);
		break;
	case 'D':
		displaytSearchSubMenu();
		break;
	case 'E':
		cout << "Exiting Search Manager..." << endl << endl;
		break;
	}

}

/*
	searchManager:
		handling taking and running the command
*/
void searchManager( BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	char command;
	displaytSearchSubMenu();
	do
	{
		if (getInput(command, SEARCH_COMMANDS))
			runSearchCommand(command, primaryTree, secondaryTree, hashArr);
	// Using the last charactor in commands list as the exit command
	// If command == last charactor in commands list, then break
	} while (command != SEARCH_COMMANDS[SEARCH_COMMANDS.length()-1]);
}

/* doSearch:
		The template for searching in different data structure
*/
template<class T>
void doSearch(T* dataStructure, string& key, void printCrypto(CryptocurrencyPtr&), bool searchFunction(T*, string, CryptocurrencyPtr&))
{
	CryptocurrencyPtr resultCryptoPtr;

	// Change the hashArr parameter to primary if you want to search in tree
	//if (treeSearch(tree, name, resultCryptoPtr))
	printHeader();
	if (searchFunction(dataStructure, key, resultCryptoPtr))
	{
		printCrypto(resultCryptoPtr);
		cout << endl;
	}
	else {
		cout << NOT_FOUND << endl;
	}
}

/*
	primaryKeySearchManager:
		handling search opeartion in primary BST
*/
void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin,name);
	trimSpaces(name);
	doSearch<BinarySearchTree<CryptocurrencyPtr>>(tree, name, printCrypto, treeSearch);
}

/*
	secondaryKeySearchManager:
		handling search opeartion in secondary BST
*/
void secondaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string alg;
	cout << "Please input the algorithm of the Cryptocurrency: " << endl;
	getline(cin, alg);
	trimSpaces(alg);
	printHeader();
	if (!tree->getAllEntry(CryptocurrencyPtr(nullptr, alg), printCrypto))
		cout << NOT_FOUND << endl;
	cout << endl;

}

/*
	hashTableSearchManage:
		handling search opeartion in hash table
*/
void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin, name);
	trimSpaces(name);
	doSearch<HashTable<CryptocurrencyPtr>>(table, name, printCrypto, hashTableSearch);
}

/*
	runListCommand:
		mapping the function to the corresponding command
*/
void runListCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr)
{
	switch (runCommand)
	{
	case 'A':
		cout << "Printing the hash table in unsorted list:" << endl;
		hashArr->printTable(printHeader, printCrypto);
		break;
	case 'B':
		cout << "Printing the primary tree in sorted list:" << endl;
		printHeader();
		primaryTree->inOrder(printCrypto);
		break;
	case 'C':
		cout << "Printing the secondary tree in sorted list:" << endl;
		printHeader();
		secondaryTree->inOrder(printCrypto);
		break;
	case 'D':
		// Speacial print
		displaytSearchSubMenu();
		break;
	case 'E':
		displaytSearchSubMenu();
		break;
	case 'F':
		cout << "Exiting List Manager..." << endl << endl;
		break;
	}

}

/*
	listManager:
		for handling operations in listing and printing
*/
void listManager( BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	char command;
	displaytListSubMenu();
	do
	{
		if (getInput(command, LIST_COMMANDS))
			runListCommand(command, primaryTree, secondaryTree, hashArr);
	// Using the last charactor in commands list as the exit command
	// If command == last charactor in commands list, then break
	} while (command != LIST_COMMANDS[LIST_COMMANDS.length()-1]);
}

/*
	undo:
		undo the most recent deletion
*/
void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* poppedItem;
	if (undoStack->pop(poppedItem))
	{
		cout << "Undo deleting " + poppedItem->getName() + " ..." << endl;
		if(insertAll(*poppedItem, primaryTree, secondaryTree, hashArr))
			cout << "Undo successful!" << endl;
		else 
			cout << "Undo Failed, Key Already Exists." << endl;
	}
}

/*
	printHashTableStats:
		print the statistics of the hash tabel in a table format
*/
void printHashTableStats(HashTable<CryptocurrencyPtr>* hashArr)
{
	cout << endl << "Statistics for cryptocurrency hashArr: " << endl;
	cout << divider(60, '=') << endl;
	cout << left;
	cout << setw(10) << "Capacity" << setw(15) << "Total Counts";
	cout << setw(15) << "Load Factor" << setw(15) << "Collisions" << endl;
	cout << divider(60, '=') << endl;
	cout << setw(10) << hashArr->getCapacity() << setw(15) << hashArr->getSize();
	cout << setw(15) << hashArr->getLoadFactor() << setw(15) << hashArr->getCollision() << endl;
	cout << divider(60, '=') << endl;
}
