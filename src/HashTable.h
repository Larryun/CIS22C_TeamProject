
// Hash table template 
// Collision resolution method: linked list
// Rehash when exceed the threshold
// Created by Michael Wong
// Modified by Meng Leong Un

#ifndef _HASH_TABLE
#define _HASH_TABLE
#include <string>
#include "DList.hpp"
#include <cmath>

template<class ItemType>
class HashTable
{
private:
	int CAPACITY = 0; 
	// Threshold for re-hashing
	int THRESHOLD = 75; 
	int hash(string&) const;
	int goodHash(string& key) const;
	int badHash(string& key) const;

	bool _findItem(DList<ItemType>*, string&, ItemType&, ItemType&);
	bool _deleteItem(DList<ItemType>*, string&, ItemType&);
	bool _insertItem(DList<ItemType>*, string&, ItemType*);
	void reHash();
	bool isPrime(int);
	int nextPrime(int);
	// dynamically allocated array
	DList<ItemType>* hashTable;
	int counter = 0;
	int collision = 0;

public:
	HashTable(int);
	~HashTable() { delete[] this->hashTable; };
	// Take the any string as key and store the pointer to the object
	bool insertItem(string&, ItemType*);
	// Find the item with the key
	bool findItem(string&, ItemType&);
	// Delete the item with the corresponding key
	bool deleteItem(string&);
	int getSize() { return counter; };
	int getCapacity() { return CAPACITY; };
	int getCollision() { return collision; };
	int getLoadFactor() { return (int)(((double)counter/(double)CAPACITY)*100.0); };
	bool isFull() { return counter == CAPACITY; };
	bool isEmpty() { return counter == 0; }
	// Print items in the table
	void printTable(void printHeader(), void printData(ItemType&));
	// Print all items including gaps in the table
	void printInfo(void printHeader(), void printData(ItemType&));
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
	this->hashTable = new DList<ItemType>[CAPACITY]; 
}

template<class ItemType>
int HashTable<ItemType>::hash(string& key) const
{
	// Change it manually for different hash function
	return goodHash(key);
}

template<class ItemType>
int HashTable<ItemType>::goodHash(string& key) const
{
	long long index = 0;
	for (int i = 1; i <= key.size(); i++) {
		int c = (int)key[i - 1];
		index += static_cast<int>(pow(c, 3) + pow(c, 2) + i);
	}
	return index % CAPACITY;
}

template<class ItemType>
int HashTable<ItemType>::badHash(string& key) const
{
	long long index = 0;
	for (int i = 0; i < key.size(); i++) {
		index += key[i];
	}
	return index % CAPACITY;
}

template<class ItemType>
bool HashTable<ItemType>::insertItem(string& key, ItemType* entry)
{
	//cout << getLoadFactor() << endl;
	if (getLoadFactor() >= THRESHOLD)
	{
		reHash();
	}
	// return true if insert successful
	return _insertItem(this->hashTable, key, entry);
}


/* findNextIndex:
		insert new item to the table, return false if fulled
*/
template<class ItemType>
bool HashTable<ItemType>::_insertItem(DList<ItemType>* table, string& key, ItemType* entry)
{
	int index = hash(key);
	if (!isFull()) {
		// If the linked list is empty, which menas no collision
		if (table[index].isEmpty())
			counter++;
		// If the linked list is not empty, which means collision occoured
		else
			collision++;
		table[index].insertNode(*entry);
		return true;
	}
	return false;
}


/* findItem:
		find the item with given key
*/
template<class ItemType>
bool HashTable<ItemType>::findItem(string& key, ItemType& result)
{
	ItemType dummyPtr(nullptr, key);
	// return true if item found
	return _findItem(this->hashTable, key, dummyPtr, result);

}

/* _findItem:
		A private function find the item with given key in a dummy object
*/
template<class ItemType>
bool HashTable<ItemType>::_findItem(DList<ItemType>* table, string& key, ItemType& item, ItemType& result) 
{
	int index = hash(key);
	ItemType resultPtr;

	if(!isEmpty())
	{
		// if item is found in the linked list
		if (table[index].searchList(item, resultPtr))
		{
			result = resultPtr;
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
	ItemType dummyPtr(nullptr, key);
	return _deleteItem(this->hashTable, key, dummyPtr);

}

/* _deleteItem:
		A private function for deleteItem
*/
template<class ItemType>
bool HashTable<ItemType>::_deleteItem(DList<ItemType>* table, string& key, ItemType& item)
{
	int index = hash(key);
	// Remove item form the linked list
	if (table[index].deleteNode(item))
	{
		if(table[index].getCount() == 1)
			collision--;
		else
			counter++;
		return true;
	}
	return false;
}

/* reHash:
		function for re-hashing
*/
template<class ItemType>
void HashTable<ItemType>::reHash()
{
	int OLD_CAPACITY = CAPACITY;
	int old_counter = counter;
	CAPACITY = nextPrime(CAPACITY * 2);
	DList<ItemType>* oldTable = this->hashTable;
	DList<ItemType>* newTable = new DList<ItemType>[CAPACITY];
	ItemType linkedListItem;
	string key;
	// reset the counter
	counter = 0;
	collision = 0;
	// Go through each item in the table
	for (int i = 0, count = 0; i < OLD_CAPACITY && count < old_counter; i++)
	{
		// Go through each item in the linked list
		for (int j = 0; j < this->hashTable[i].getCount(); j++)
		{
			// get the item in index j
			if (this->hashTable[i].getNodeAtIndex(j, linkedListItem))
			{
				key = linkedListItem.getKey();
				// insert the the table
				_insertItem(newTable, key, &linkedListItem);
			}
		}
		if (this->hashTable[i].getCount() >= 1)
			count++;
	}
	this->hashTable = newTable;
	delete[] oldTable;
}

template<class ItemType>
void HashTable<ItemType>::printTable(void printHeader(), void printData(ItemType&))
{
	printHeader();
	for (int i = 0; i < CAPACITY; i++)
	{
		if(this->hashTable[i].getCount() > 0)
		{
			cout << "----------------------------------------" << i << "-------------------------------------" << endl;
			this->hashTable[i].traverseForward(printData);
			/* For Debug, see if the keys in linked-list have the same hash key
			if (hashTable[i].getCount() >= 2)
			{
				ItemType i1, i2;
				string k1, k2;
				hashTable[i].getNodeAtIndex(0, i1);
				hashTable[i].getNodeAtIndex(1, i2);
				k1 = i1.getKey();
				k2 = i2.getKey();
				cout << hash(k1) << " " << hash(k2) << endl;
			}
			*/
		}

	}
}
template<class ItemType>
void HashTable<ItemType>::printInfo(void printHeader(), void printData(ItemType&))
{
	printHeader();
	for (int i = 0; i < CAPACITY; i++)
	{
			//cout << i;
			this->hashTable[i].traverseForward(printData);
			cout << "----------------------------------------" << i << "-------------------------------------" << endl;
	}
	cout << "Number of collisions: " << collision<< endl;
}


#endif
