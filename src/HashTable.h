
// Hash table abstract base class
// Created by Michael Wong
// Modified by Meng Leong Un

#ifndef _HASH_TABLE
#define _HASH_TABLE
#include <string>


template<class ItemType>
class HashTable
{
private:
	int CAPACITY = 0; //Change it later
	int THRESHOLD = 75; // Threshold for re-hashing
	int hash(string&) const;
	bool findNextIndex(ItemType**, string&, ItemType&, int&) const;
	bool findIndex(ItemType**, string&, ItemType&, int&) const;
	bool _findItem(ItemType**, string&, ItemType&, ItemType&);
	bool _deleteItem(ItemType**, string&, ItemType&);
	bool _insertItem(ItemType**, string&, ItemType*);
	void reHash();
	bool isPrime(int);
	int nextPrime(int);
	// dynamically allocated array
	ItemType** hashTable;
	int counter = 0;
	// int dummy = -100; //For deletion purpose

public:
	HashTable(int);
	~HashTable() { delete[]hashTable; }
	// Take the any string as key and store the pointer to the object
	bool insertItem(string& , ItemType*);
	// Take the find the item with the key
	bool findItem(string&, ItemType&);
	// Take the delete the item with the corresponding key
	bool deleteItem(string&);
	int getSize() { return counter; };
	int getLoadFactor() { return (int)(((double)counter/(double)CAPACITY)*100.0); };
	bool isFull() { return counter == CAPACITY; };
	bool isEmpty() { return counter == 0; }
	//For Debug
	void printTable()
	{
		for (int i = 0; i < CAPACITY; i++)
		{
			cout << i << ": " << hashTable[i] << endl;
		}
	}
};

template<class ItemType>
bool HashTable<ItemType>::isPrime(int num)
{
	for (int i = 2; i < num / 2; i++)
	{
		if (num % i == 0)
			return false;
	}
	return true;
}

template<class ItemType>
int HashTable<ItemType>::nextPrime(int num)
{
	for (int i = num; i < INFINITY; i++)
		if (isPrime(i))
			return i;
}

template<class ItemType>
HashTable<ItemType>::HashTable(int size)
{
	CAPACITY = nextPrime(size * 2);
	counter = 0;
	hashTable = new ItemType* [CAPACITY]; 
	for (int i = 0; i < CAPACITY; i++)
		hashTable[i] = nullptr;
}

template<class ItemType>
int HashTable<ItemType>::hash(string& key) const
{
	int index = 0;
	for (int i = 1; i <= key.size(); i++) {
		index += (key[i - 1] * key[i - 1] * i);
	}
	return index % CAPACITY;
}

/* findNextIndex:
		A function for linear proble collision resulotion. Find the next available index of the given key
*/
template<class ItemType>
bool HashTable<ItemType>::findNextIndex(ItemType** table, string& key, ItemType& item, int& returnIndex) const
{
	int count = 0;
	returnIndex = hash(key);
	// Available about index found
	if (table[returnIndex] == nullptr) return true;
	// Same item have found in the table, return false
	if (*table[returnIndex] == item) return false;
	// Find next available index
	while (count < CAPACITY && table[returnIndex] != nullptr)
	{
		count++;
		returnIndex = (returnIndex + 1 ) % CAPACITY;
	}
}

/* findNextIndex:
		A function for searching the item. Find and return the item with the given key
*/
template<class ItemType>
bool HashTable<ItemType>::findIndex(ItemType** table, string& key, ItemType& item, int& returnIndex) const
{
	int count = 0;
	returnIndex = hash(key);
	while (count < CAPACITY)
	{
		count++;
		if (table[returnIndex] == nullptr) continue;
		// Same item have found in the table, break the loop
		if (*table[returnIndex] == item) break;
		returnIndex = (returnIndex + 1) % CAPACITY;
	}
	if (count < CAPACITY)
		return true;
	else
		return false;
}

template<class ItemType>
bool HashTable<ItemType>::insertItem(string& key, ItemType* entry)
{
	if (getLoadFactor() >= THRESHOLD)
		reHash();
	return _insertItem(this->hashTable, key, entry);
}


/* findNextIndex:
		insert new item to the table, return false if fulled
*/
template<class ItemType>
bool HashTable<ItemType>::_insertItem(ItemType** table, string& key, ItemType* entry)
{
	int index;
	if(!isFull()){
		if (findNextIndex(table, key, *entry, index)) {
			table[index] = entry;
			//cout << table[index]->getKey() << endl;
			counter++;
			return true;
		}
	}
	return false;
}


/* findItem:
		find the item with given key
*/
template<class ItemType>
bool HashTable<ItemType>::findItem(string& key, ItemType& result)
{
	CryptocurrencyPtr dummyCryptoPtr(nullptr, key);
	return _findItem(this->hashTable, key, dummyCryptoPtr, result);

}

/* _findItem:
		A private function find the item with given key in a dummy object
*/
template<class ItemType>
bool HashTable<ItemType>::_findItem(ItemType** table, string& key, ItemType& item, ItemType& result) 
{
	int index;

	if(!isEmpty())
	{
		if (findIndex(table, key, item, index))
		{
			result = *table[index];
			return true;
		}
	}
	return false;
}

/* deleteItem:
		find and set the pointer to nullptr
*/
template<class ItemType>
bool HashTable<ItemType>::deleteItem(string& key)
{
	CryptocurrencyPtr dummyCryptoPtr(nullptr, key);
	return _deleteItem(this->hashTable, key, dummyCryptoPtr);

}

/* _deleteItem:
		A private function for deleteItem
*/
template<class ItemType>
bool HashTable<ItemType>::_deleteItem(ItemType** table, string& key, ItemType& item)
{
	int index;
	if (findIndex(table, key, item, index))
	{
		delete table[index];
		table[index] = nullptr;
		counter--;
		return true;
	}
	else
	{
		return false;
	}
}

template<class ItemType>
void HashTable<ItemType>::reHash()
{
	int OLD_CAPACITY = CAPACITY;
	CAPACITY = nextPrime(CAPACITY * 2);
	ItemType** oldTable = hashTable;
	ItemType** newTable = new ItemType* [CAPACITY];
	for (int i = 0; i < CAPACITY; i++) newTable[i] = nullptr;
	string key;
	for (int i = 0, count = 0; i < OLD_CAPACITY && count < counter; i++)
	{
		if (hashTable[i] != nullptr)
		{
			key = hashTable[i]->getKey();
			_insertItem(newTable, key, hashTable[i]);
		}
	}
	this->hashTable = newTable;
	delete[] oldTable;
}

#endif