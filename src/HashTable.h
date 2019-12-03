
// Hash table template 
// Collision resolution method: linked list
// Rehash when exceed the threshold
// Created by Michael Wong
// Modified by Meng Leong Un

#ifndef _HASH_TABLE
#define _HASH_TABLE
#include <string>
#include "DList.hpp"


template<class ItemType>
class HashTable
{
private:
	int CAPACITY = 0; //Change it later
	int THRESHOLD = 75; // Threshold for re-hashing
	int hash(string&) const;
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
	~HashTable() { delete[]hashTable; }
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
	void printTable(void printHeader(), void printData(ItemType&));
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
	hashTable = new DList<ItemType>[CAPACITY]; 
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

template<class ItemType>
bool HashTable<ItemType>::insertItem(string& key, ItemType* entry)
{
	//cout << getLoadFactor() << endl;
	if (getLoadFactor() >= THRESHOLD)
		reHash();
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
		counter--;
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
	DList<ItemType>* oldTable = hashTable;
	DList<ItemType>* newTable = new DList<ItemType>[CAPACITY];
	ItemType linkedListItem;
	string key;
	// reset the counter
	counter = 0;
	// Go through each item in the table
	for (int i = 0, count = 0; i < OLD_CAPACITY && count < old_counter; i++)
	{
		// Go through each item in the linked list
		for (int j = 0; j < hashTable[i].getCount(); j++)
		{
			// get the item in index j
			if (hashTable[i].getNodeAtIndex(j, linkedListItem))
			{
				key = linkedListItem.getKey();
				// insert the the table
				_insertItem(newTable, key, &linkedListItem);
			}
		}
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
		if(hashTable[i].getCount() > 0)
		{
			hashTable[i].traverseForward(printData);
			cout << "-----------------------------------------------" << endl;
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
#endif