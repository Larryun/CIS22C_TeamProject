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

const string MAIN_COMMANDS = "ABCDEFGHI";
const string SEARCH_COMMANDS = "ABCDE";
const string LIST_COMMANDS = "ABCDEF";
const string NOT_FOUND = "Cryptocurrency Not Found!";

// Function Prototypes
void displayMenu();
void insertManager(BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
void deleteManager(Stack<Cryptocurrency*>* deleteStack, BinarySearchTree <CryptocurrencyPtr>* primaryTree, BinarySearchTree <CryptocurrencyPtr>* secondaryTree,HashTable<CryptocurrencyPtr>* hashArr);
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
bool getInput(char& command, const string& COMMANDS);
bool validateInput(char& command, const string& COMMANDS);
bool isAllAlpha(string& s);
void trimSpaces(string& line);
void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);

void runCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr,
	Stack<Cryptocurrency*>* deleteStack);

void searchManager( BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);
void listManager( BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr);

void runSearchCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr);

void runListCommand(char& runCommand,
	BinarySearchTree<CryptocurrencyPtr>* primaryTree,
	BinarySearchTree<CryptocurrencyPtr>* secondaryTree,
	HashTable<CryptocurrencyPtr>* hashArr);

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
	cout << endl << "====================================================" << endl;
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
	int supply, year;
	double price;

	cout << "What is the name of the Cryptocurrency?" << endl;
	getline(cin, name);
	trimSpaces(name);

	cout << "What is " << name << "'s Algorithm?" << endl;
	getline(cin, alg);
	trimSpaces(alg);

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

	cin.ignore();
	cout << "Who founded " << name << "?" << endl;
	getline(cin, founder);
	trimSpaces(founder);

	crypto = new Cryptocurrency(name, alg, supply, year, price, founder);
	insertAll(*crypto, primaryTree, secondaryTree, hashArr);

	cout << "The Crypto has been inserted!" << endl;
	cout << endl;

	// for testing purposes
	 cout << name << " " << alg << " " << supply << " " << year << " " << price << " " << founder << endl;
		 
}

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

void primaryKeySearchManager(BinarySearchTree<CryptocurrencyPtr>* tree, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin,name);
	trimSpaces(name);
	doSearch<BinarySearchTree<CryptocurrencyPtr>>(tree, name, printCrypto, treeSearch);
}

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

void hashTableSearchManager(HashTable<CryptocurrencyPtr>* table, void printCrypto(CryptocurrencyPtr&))
{
	string name;
	cout << "Please input the name of the Cryptocurrency: " << endl;
	getline(cin, name);
	trimSpaces(name);
	doSearch<HashTable<CryptocurrencyPtr>>(table, name, printCrypto, hashTableSearch);
}

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

void undo(Stack<Cryptocurrency*>* undoStack, BinarySearchTree<CryptocurrencyPtr>* primaryTree, BinarySearchTree<CryptocurrencyPtr>* secondaryTree, HashTable<CryptocurrencyPtr>* hashArr)
{
	Cryptocurrency* poppedItem;
	if (undoStack->pop(poppedItem))
		cout << "Undo deleting " + poppedItem->getName() + " ..." << endl;
		insertAll(*poppedItem, primaryTree, secondaryTree, hashArr);
		cout << "Undo successful!" << endl;
}
