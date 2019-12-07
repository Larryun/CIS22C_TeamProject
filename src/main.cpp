// main test driver for CryptoCurrency Database
// Modified by: Tommy Vu, Meng Leong Un, Michael Wong, Steven Vu

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
const string LIST_COMMANDS = "ABCDEFG";
const string NOT_FOUND = "Cryptocurrency Not Found!";

// Menu printing function
void displayMenu();
void displaytSearchSubMenu();
void displaytListSubMenu();

// Build tree function
void buildTree(string filename, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);

// Manager Functions
void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void deleteManager(Stack<Cryptocurrency*>* deleteStack, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void searchManager(BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
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
void visitIndented(int level, CryptocurrencyPtr& cryptoPtr);
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
bool supplyValidator(long long& supply);
bool priceValidator(double& price);
bool validateInput(char& command, const string& COMMANDS);
bool isAllAlpha(string& s);
void normalizeStr(string& str);

// Trivial Functions
bool isKeyExists(string& key, BinarySearchTree<CryptocurrencyPtr>* tree);
bool insertAll(Cryptocurrency& crypto, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
int getNumberOfLines(string filename);
void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void clearStack(Stack<Cryptocurrency*>* undoStack);
void trimSpaces(string& line);
void exit(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void printWelcome();

// Build to File Functions
void writeData(BinarySearchTree <CryptocurrencyPtr>* primaryTree, string outFileName, Stack<Cryptocurrency*>* undoStack);
void visitPrint(CryptocurrencyPtr& a, ofstream& outFile);



int main()
{
	printWelcome();
	string inputFileName = "CryptoList.txt";
	int lines = getNumberOfLines(inputFileName);

	Stack<Cryptocurrency*>* undoStack = new Stack<Cryptocurrency*>();

	BinarySearchTree <CryptocurrencyPtr>* primaryTree = new BinarySearchTree <CryptocurrencyPtr>;
	BinarySearchTree <CryptocurrencyPtr>* secondaryTree = new BinarySearchTree <CryptocurrencyPtr>;

	HashTable<CryptocurrencyPtr>* hashArr = new HashTable<CryptocurrencyPtr>(lines); //Initialize the hashTable
	buildTree(inputFileName, primaryTree, secondaryTree, hashArr);

	displayMenu();

	char command;
	do
	{
		if (getInput(command, MAIN_COMMANDS))
			runCommand(command, primaryTree, secondaryTree, hashArr, undoStack);

	} while (command != MAIN_COMMANDS[MAIN_COMMANDS.length() - 1]);
	cout << "Bye! Thanks for using the Cryptocurrency Database!" << endl;
	return 0;
}

/*
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
	return true if input is correct otherwise return false
*/
bool validateInput(char& command, const string& COMMANDS)
{
	if (COMMANDS.find(command) != -1)
		return true;
	else
		cout << command << " is not a valid operation. Please enter a valid operation. (Press " <<  *(COMMANDS.rbegin()+1) << " for help) \n" << endl;
	return false;
}

/*
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
		writeData(primaryTree, "SortedOutput.txt", deleteStack);
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
		writeData(primaryTree, "SortedOutput.txt", deleteStack);
		exit(deleteStack, primaryTree, secondaryTree, hashArr);
		break;
	}
}

/* 
	display the main menu for different operations
*/

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

/* 
	display the search sub-menu for searching operation
*/
void displaytSearchSubMenu()
{
	cout << "Sub-menu for search operation" << endl;
	cout << "" << endl;
	cout << "A - Search for a Cryptocurrency with Primary Key in BST" << endl;
	cout << "B - Search for a Cryptocurrency with Secondary Key in BST" << endl;
	cout << "C - Search for a Cryptocurrency with Primary Key in Hash Table" << endl;
	cout << "D - Help to show the menu" << endl;
	cout << "E - Exit" << endl;
	cout << endl;
}

/* 
	display the list sub-menu for listing operation
*/
void displaytListSubMenu()
{
	cout << "Sub-menu for Listing operation" << endl;
	cout << "" << endl;
	cout << "A - List unsorted list" << endl;
	cout << "B - List data sorted by the primary key (name of the cryptocurrency)" << endl;
	cout << "C - List data sorted by the secondary key (algorithm of the cryptocurrency)" << endl;
	cout << "D - Special Print" << endl;
	cout << "F - Help to show the menu" << endl;
	cout << "G - Exit" << endl;
	cout << endl;
}


/* 
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
	// Insert with alg as secondary key and name as primary key
	CryptocurrencyPtr* algAsKey = new CryptocurrencyPtr(&crypto, alg, name);
	primaryTree->insert(*nameAsKey);
	secondaryTree->insert(*algAsKey);
	hashArr->insertItem(name, nameAsKey);
	return true;
}

/* 
	get number of lines in the file
*/
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
	return count - 1;
}

/* 
	Builds a Binary Search Tree from a text file.
*/

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
	print infomations of the cryptocurrnecy in given format
*/
void printCrypto(Cryptocurrency& crypto)
{
	// Subject to change
	cout << left;
	cout << setw(24) << crypto.getName();
	cout << setw(15) << crypto.getAlg();
	cout << setw(6) << crypto.getDate();
	cout << setw(12) << crypto.getFounder();
	cout << "$" << setw(8) << crypto.getPrice();
	cout << setw(12) << crypto.getSupply() << "||" << endl;

}

/*
	overloaded function for printting CryptocurrencyPtr
	print infomations of the cryptocurrnecy in given format
*/
void printCrypto(CryptocurrencyPtr& cryptoPtr)
{
	// Dereferencing the CryptocurrencyPtr then call the regular printCrypto function
	printCrypto(*(cryptoPtr.crypto));
}

/*
	print a line of given symble in given len
*/
string divider(int len, char symbol)
{
	string t = "";
	for (int i = 0; i < len; i++)
		t += symbol;
	return t;
}

/*
	print the header for the printCrypto function
*/
void printHeader()
{
	cout << endl << divider(80, '=') << endl;
	cout << left;
	cout << setw(24) << "Name";
	cout << setw(15) << "Algorithm";
	cout << setw(6) << "Date";
	cout << setw(12) << "Founder";
	cout << setw(9) << "Price";
	cout << setw(12) << "Supply" << endl;
	cout << divider(80, '=') << endl;
}

/*
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
	promt the user and take the value, return false if fail and not validated
*/
template<typename T>
bool getInsertData(T& data, string message, bool validate(T&))
{
	if (getInsertData(data, message) && validate(data)) return true;
	return false;
}

/*
	validate the year
*/
bool yearValidator(int& year)
{
	return (year >= 1950 && year <= 2019);
}

/*
	validate the supply
*/
bool supplyValidator(long long& supply)
{
	supply = (int)supply;
	cin.ignore(80, '\n');
	return (supply >= 0);
}

/*
	validate the price
*/
bool priceValidator(double& price)
{
	return (price >= 0);
}

/*
	chekc if the key is exists in the BST
*/
bool isKeyExists(string& key, BinarySearchTree<CryptocurrencyPtr>* tree)
{
	CryptocurrencyPtr tmp;
	return (tree->getEntry(CryptocurrencyPtr(nullptr, key), tmp));

}

/*
	handle taking inputs and inerst the the trees and hash table.
*/
void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* crypto;
	CryptocurrencyPtr tmp;
	int checker = 0;
	string name, alg, founder;
	long long supply;
	int year;
	double price;

	cout << "What is the name of the Cryptocurrency?" << endl;
	getline(cin, name);
	normalizeStr(name);
	
	if (isKeyExists(name, primaryTree))
	{
		cout << "The Cryptocurrency " + name + " already exists in the primary tree!" << endl;
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
	normalizeStr(founder);

	crypto = new Cryptocurrency(name, alg, supply, year, price, founder);
	insertAll(*crypto, primaryTree, secondaryTree, hashArr);

	cout << "The Cryptocurrency " + name + " has been inserted!" << endl;
	cout << endl;

	// for testing purposes
	// cout << name << " " << alg << " " << supply << " " << year << " " << price << " " << founder << endl;

}

/*
	handling deletion and taking input from user
	push deleted item to undo stack
	and remove the item from hash table, and two trees
*/
void deleteManager(Stack<Cryptocurrency*>* undoStack, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	CryptocurrencyPtr cryptoPtr;
	CryptocurrencyPtr tmp;
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
		// Remove the node form secondary tree with algorithm as key and name as primary key
		secondaryTree->remove(CryptocurrencyPtr(nullptr, alg, name));
		// Remove the node form secondary tree with name
		hashArr->deleteItem(name);
		cout << name << " has been deleted from both trees and the hash table" << endl;
	}
	else
	{
		cout << NOT_FOUND << endl;
	}
}

/*
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
		cout << divider(80, '=') << endl << endl;
		break;
	case 'B':
		secondaryKeySearchManager(secondaryTree, printCrypto);
		cout << divider(80, '=') << endl << endl;
		break;
	case 'C':
		hashTableSearchManager(hashArr, printCrypto);
		cout << divider(80, '=') << endl << endl;
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
	handling taking and running the command
*/
void searchManager(BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	char command;
	displaytSearchSubMenu();
	do
	{
		if (getInput(command, SEARCH_COMMANDS))
			runSearchCommand(command, primaryTree, secondaryTree, hashArr);
		// Using the last charactor in commands list as the exit command
		// If command == last charactor in commands list, then break
	} while (command != SEARCH_COMMANDS[SEARCH_COMMANDS.length() - 1]);
}

/* 
	The template for searching in different data structure.  I am Meng, Meng is awesome.
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
	}
	else {
		cout << NOT_FOUND << endl;
	}
}

/*
	handling search opeartion in primary BST
*/
void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin, name);
	trimSpaces(name);
	doSearch<BinarySearchTree<CryptocurrencyPtr>>(tree, name, printCrypto, treeSearch);
}

/*
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

}

/*
	handling search opeartion in hash table
*/
void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin, name);
	normalizeStr(name);
	doSearch<HashTable<CryptocurrencyPtr>>(table, name, printCrypto, hashTableSearch);
}

/*
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
		cout << "Load factor: " << hashArr->getLoadFactor() << "%" << endl;
		break;
	case 'B':
		cout << "Printing the primary tree in sorted list:" << endl;
		printHeader();
		primaryTree->inOrder(printCrypto);
		cout << divider(80, '=') << endl << endl;
		break;
	case 'C':
		cout << "Printing the secondary tree in sorted list:" << endl;
		printHeader();
		secondaryTree->inOrder(printCrypto);
		cout << divider(80, '=') << endl << endl;
		break;
	case 'D':
		// Speacial print
		primaryTree->indentedTraversal(visitIndented);
		//primaryTree->print(print1);
		cout << endl;
		break;
	case 'E':
		// Hidden Print
		hashArr->printInfo(printHeader, printCrypto);
		break;
	case 'F':
		displaytListSubMenu();
		break;
	case 'G':
		cout << "Exiting List Manager..." << endl << endl;
		break;
	}

}

/*
	for handling operations in listing and printing
*/
void listManager(BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	char command;
	displaytListSubMenu();
	do
	{
		if (getInput(command, LIST_COMMANDS))
			runListCommand(command, primaryTree, secondaryTree, hashArr);
		// Using the last charactor in commands list as the exit command
		// If command == last charactor in commands list, then break
	} while (command != LIST_COMMANDS[LIST_COMMANDS.length() - 1]);
}

/*
	undo the most recent deletion
*/
void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* poppedItem;
	if (undoStack->pop(poppedItem))
	{
		cout << "Undo deleting " + poppedItem->getName() + " ..." << endl;
		if (insertAll(*poppedItem, primaryTree, secondaryTree, hashArr))
			cout << "Undo successful!" << endl;
		else
			cout << "Undo Failed, Key Already Exists." << endl;
	}
	else
	{
		cout << "Undo Stack is Empty." << endl;
	}
}

/*
	print the statistics of the hash table in a table format
*/
void printHashTableStats(HashTable<CryptocurrencyPtr>* hashArr)
{
	cout << endl << "Statistics for cryptocurrency hash table: " << endl;
	cout << divider(80, '=') << endl;
	cout << left;
	cout << setw(10) << "Capacity" << setw(15) << "Counts";
	cout << setw(15) << "Load Factor (%)" << setw(15) << "Collisions";
	cout << setw(20) << "Total Item Stored" << endl;
	cout << divider(80, '=') << endl;
	cout << setw(10) << hashArr->getCapacity() << setw(15) << hashArr->getSize();
	cout << setw(15) << hashArr->getLoadFactor() << setw(15) << hashArr->getCollision();
	cout << setw(20) << hashArr->getSize() + hashArr->getCollision() << endl;
	cout << divider(80, '=') << endl;
}

/*
	visit function for speacial print
*/
void visitIndented(int level, CryptocurrencyPtr& cryptoPtr)
{
	for (int i = 0; i < level; i++)
		cout << "\t";
	cout << "Level " << level + 1 << ": " << cryptoPtr.getCrypto()->getName() << endl;
}

/*
	write data in trees to the file
*/
void writeData(BinarySearchTree <CryptocurrencyPtr>* primaryTree, string outFileName, Stack<Cryptocurrency*>* undoStack)
{
	ofstream textFile;
	textFile.open(outFileName.c_str());
	// If cannot open file
	if (!textFile.is_open())
	{
		cout << "Cannot open file: " + outFileName << endl;
		return;
	}
	textFile << "Cryptocurrency Database" << endl;
	textFile << endl;

	textFile << "============= ============= ============ ======= ======== =====================" << endl;
	textFile << "Algorithm     Supply        Founder      Year    Price    Coin Name" << endl;
	textFile << "============= ============= ============ ======= ======== =====================" << endl;

	primaryTree->inOrderOutput(visitPrint, textFile);
	clearStack(undoStack);
	cout << "Data writen to " + outFileName << endl;
	cout << "Undo stack cleared!" << endl;

	textFile.close();
}

/*
	visit function for printing
*/
void visitPrint(CryptocurrencyPtr& a, ofstream& outFile)
{
	Cryptocurrency dummy = *a.getCrypto();
	outFile << left;
	outFile << setw(13) << dummy.getAlg() << " " << setw(13) << dummy.getSupply() << " " << setw(12) << dummy.getFounder()
		<< " " << setw(7) << dummy.getDate() << " " << setw(8) << dummy.getPrice() << " " << setw(22) << dummy.getName() << endl;
}

/*
	clear the undo stack
*/
void clearStack(Stack<Cryptocurrency*>* undoStack)
{
	Cryptocurrency* tmp;
	while (undoStack->pop(tmp))
		delete tmp;
}

/*
	delete all memory and exit the program:
*/
void exit(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	cout << "Deleting Undo Stack..." << endl;
	delete undoStack;
	cout << "Deleting Primary Tree..." << endl;
	delete primaryTree;
	cout << "Deleting Secondary Tree..." << endl;
	delete secondaryTree;
	cout << "Deleting hash table..." << endl;
	delete hashArr;
}

/*
	trim the spaces and make the first letter in each word th upper
*/
void normalizeStr(string& str)
{
	string result = "";
	string tmp = "";
	trimSpaces(str);
	for (int i = 0; i < str.length(); i++)
	{
		// At the end
		if (i + 1 == str.length())
		{
			tmp += (str[i]);
			tmp[0] = toupper(tmp[0]);
			result += tmp;
			break;
		}
		else if (str[i] == ' ' && str[i - 1] != ' ')
		{
			// Uppercase for the first letter
			tmp[0] = (tmp[0]);
			result += tmp;
			result += " ";
			tmp = "";
		}
		else if (str[i] != ' ')
		{
			tmp += (str[i]);
		}
	}
	str = result;
}

/*
	print the welcome message!
*/
void printWelcome()
{
	cout << "\n\n\t =================== -WELCOME- ===================\n\n";
	cout << "\tThis project is based off of CryptoCurrencies.\n"
		<< "\tWe will have two BST: One with the coins name\n"
		<< "\tand another with the coins algorithm. \n";
	cout << "\tWe also have a hash array using modulo division.\n\n";
	cout << "\tTeam Coordinator: Tommy Vu\n";
	cout << "\tBST Algorithms: Steven Vu\n";
	cout << "\tHash List Algorithms: Michael Wong\n";
	cout << "\tScreen Output: Meng Leong Un\n";
	cout << "\tFile I/O: Tommy Vu\n";
	cout << "\tTest Plan: Michael Wong & Meng Leong Un\n";
	cout << "\tProject Documentation: Tommy Vu & Steven Vu";
	cout << "\n\n\t ============== -CryptoCurrencies- ==============\n\n";
}
