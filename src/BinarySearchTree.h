// Binary Search Tree ADT
// Created by A. Student
// Modified by: Tommy Vu
 
#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"
#include "BinaryNode.h"


template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{   
private:
	// internal insert node: insert newNode in nodePtr subtree
	BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);
   
	// internal remove node: locate and delete target node under nodePtr subtree
	BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool & success);
   
	// delete target node from tree, called by internal remove node
	BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);
   
	// remove the leftmost node in the left subtree of nodePtr
	BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType & successor);

	// search the leftmost node in the left subtree of nodePtr
	BinaryNode<ItemType>* findLeftNode(BinaryNode<ItemType>* treePtr) const;

	// search the rightmost node in the right subtree of nodePtr
	BinaryNode<ItemType>* findRightNode(BinaryNode<ItemType>* treePtr) const;
   
	// search for target node
	BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType & target) const;
   
public:  
	// insert a node at the correct location
    bool insert(const ItemType & newEntry);
	// remove a node if found
	bool remove(const ItemType & anEntry);
	// find a target node
	bool getEntry(const ItemType & target, ItemType & returnedItem) const;
	// find all entry with the target node
	bool getAllEntry(const ItemType & target, void visit(ItemType& )) const;
	// find leftmost node
	bool leftNode(ItemType & cool) const;
	// find rightmost node
	bool rightNode(ItemType& cool) const;
 
};


///////////////////////// public function definitions ///////////////////////////
//Inserting items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType & newEntry)
{
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
	this->rootPtr = _insert(this->rootPtr, newNodePtr);
	this->count++;
	return true; 
}  

//Removing items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType & target)
{
	bool isSuccessful = false;
	this->rootPtr = _remove(this->rootPtr, target, isSuccessful);
	if (isSuccessful)
	{
		this->count--;
	}
	return isSuccessful; 
}  

//Finding entries within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::getEntry(const ItemType& anEntry, ItemType & returnedItem) const
{
	bool checker = false;
	BinaryNode<ItemType>* entry;
	entry = findNode(this->rootPtr, anEntry);
	if (entry)
	{
		returnedItem = entry->getItem();
		checker = true;
	}
    return checker;
}  

template<class ItemType>
bool BinarySearchTree<ItemType>::getAllEntry(const ItemType& target, void visit(ItemType&)) const
{
	// find the node
	BinaryNode<ItemType>* entry = findNode(this->rootPtr, target);
	// if node is found
	if (entry)
	{
		do {
			// visit the node
			ItemType tmp = entry->getItem();
			visit(tmp);
			// find next node with the same key
			// since the, by default, we insert the node with the same key to the right
			// so all the node with the same key will be in the right-subtree of the node
			entry = findNode(entry->getRightPtr(), target);
		} while (entry);
		return true;
	}
	return false;
}

//Finding left most node within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::leftNode(ItemType & cool) const
{
	bool checker = false;
	BinaryNode<ItemType>* entry;
	entry = findLeftNode(this->rootPtr);
	if (entry)
	{
		cool = entry->getItem();
		checker = true;
	}
	return checker;
}

//Finding right most node within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::rightNode(ItemType& cool) const
{
	bool checker = false;
	BinaryNode<ItemType>* entry;
	entry = findRightNode(this->rootPtr);
	if (entry)
	{
		cool = entry->getItem();
		checker = true;
	}
	return checker;
}




//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr)
{
	if (nodePtr == 0)
	{
		return newNodePtr;
	}
	else
	{
		if (newNodePtr->getItem() < nodePtr->getItem())
		{
			nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr));
		}
		else
		{
			nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr));
		}
		return nodePtr;
	}
}

//Implementation for the search operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* nodePtr,
                                                           const ItemType & target) const
{
	if (nodePtr == 0)
	{
		return 0;
	}
	else
	{
		if (nodePtr->getItem() == target)
		{
			return nodePtr;
		}
		if (nodePtr->getItem() < target)
		{
			return findNode(nodePtr->getRightPtr(), target);
		}
		else
		{
			return findNode(nodePtr->getLeftPtr(), target);
		}
	}
}

//Implementation for the left node search
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findLeftNode(BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr == 0)
	{
		return 0;
	}
	else
	{
		if (nodePtr->getLeftPtr() != 0)
		{
			return findLeftNode(nodePtr->getLeftPtr());
		}
		else
		{
			return nodePtr;
		}
	}
}

//Implementation for the right node search
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findRightNode(BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr == 0)
	{
		return 0;
	}
	else
	{
		if (nodePtr->getRightPtr() != 0)
		{
			return findRightNode(nodePtr->getRightPtr());
		}
		else
		{
			return nodePtr;
		}
	}
}


////////////////////////////////////////////////////////////////////////
// The following functions will be needed for the team project
//Implementation of the remove operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr,
                                                          const ItemType target,
                                                          bool & success)

{
	if (nodePtr == 0)                   
	{
		success = false;
		return 0;
	}
	if (nodePtr->getItem() > target)		 
		nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success));
	else if (nodePtr->getItem() < target)	 
		nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
	else		
	{
		nodePtr = deleteNode(nodePtr);
		success = true;
	}      
	return nodePtr;   
}  
//Implementation of the delete operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr->isLeaf())				
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)  
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0) 
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;      
	}
	else                                  
	{
		ItemType newNodeValue;
		nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;
	}  
}  
//Implementation to remove the left leaf
template<class ItemType> 
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
                                                                     ItemType & successor)
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return deleteNode(nodePtr);
	}
	else 
	{
		nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
		return nodePtr;
	}       
}


#endif
