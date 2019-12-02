// Binary tree abstract base class
// Created by A. Student
// Modified by: Tommy Vu

#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include "Queue.h"
#include "StackADT.h"
#include <iomanip>


template<class ItemType>
class BinaryTree
{
protected:
	BinaryNode<ItemType>* rootPtr;		// ptr to root node
	int count;	// number of nodes in tree

public:
	// "admin" functions
	BinaryTree() { rootPtr = 0; count = 0; }
	BinaryTree(const BinaryTree<ItemType>& tree) { }
	virtual ~BinaryTree() { }

	// common functions for all binary trees
	bool isEmpty() const { return count == 0; }
	int size() const { return count; }
	void clear() { destroyTree(rootPtr); rootPtr = 0; count = 0; }
	void preOrder(void visit(ItemType&)) const { _preorder(visit, rootPtr); }
	void inOrder(void visit(ItemType&)) const { _inorder(visit, rootPtr); }
	void postOrder(void visit(ItemType&)) const { _postorder(visit, rootPtr); }
	void breadthTrav(void visit(ItemType&)) const { _breadthTrav(visit, rootPtr); }
	void iterativePreOrder(void visit(ItemType&)) const { _iterativePreOrder(visit, rootPtr); }
	void iterativePostOrder(void visit(ItemType&)) const { _iterativePostOrder(visit, rootPtr); }
	void print(void visit(ItemType&)) const { _print(visit, rootPtr, 0); }

	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType& newData) = 0;
	virtual bool remove(const ItemType& data) = 0;
	virtual bool getEntry(const ItemType& anEntry, ItemType& returnedItem) const = 0;

private:
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType>* nodePtr);

	// internal traverse
	void _preorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _inorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _postorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _breadthTrav(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _iterativePreOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _print(void visit(ItemType&), BinaryNode<ItemType>* nodePtr, int level) const;
	void _iterativePostOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
};

//Destroy the entire tree
// I have no idea how exactly I could use this as it's a private,
// Maybe I could've added another destroy tree as a public function?
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr != 0)
	{
		destroyTree(nodePtr->getLeftPtr());
		destroyTree(nodePtr->getRightPtr());
		cout << "DELETING NODE: " << nodePtr->getItem.display() << endl;
		nodePtr = 0;
	}
}

//Preorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_preorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		ItemType item = nodePtr->getItem();
		visit(item);
		_preorder(visit, nodePtr->getLeftPtr());
		_preorder(visit, nodePtr->getRightPtr());
	}
}

//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		_inorder(visit, nodePtr->getLeftPtr());
		ItemType item = nodePtr->getItem();
		visit(item);
		_inorder(visit, nodePtr->getRightPtr());
	}
}

//Postorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_postorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		_postorder(visit, nodePtr->getLeftPtr());
		_postorder(visit, nodePtr->getRightPtr());
		ItemType item = nodePtr->getItem();
		visit(item);
	}
}

//Breadth Traversal
template<class ItemType>
void BinaryTree<ItemType>::_breadthTrav(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{

	if (nodePtr != 0)
	{
		Queue<BinaryNode<ItemType>*> queue;
		queue.enqueue(nodePtr);
		ItemType item = nodePtr->getItem();

		while (queue.isEmpty() == false)
		{
			queue.dequeue(nodePtr);
			item = nodePtr->getItem();
			visit(item);

			if (nodePtr->getLeftPtr() != 0)
			{
				queue.enqueue(nodePtr->getLeftPtr());
			}

			if (nodePtr->getRightPtr() != 0)
			{
				queue.enqueue(nodePtr->getRightPtr());
			}
		}
	}

}



// Iterative PostOrder
template<class ItemType>
void BinaryTree<ItemType>::_iterativePostOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{

	if (nodePtr == 0)
	{
		return;
	}
		Stack<BinaryNode<ItemType>*> stack;
		BinaryNode<ItemType>* nimrod;
		BinaryNode<ItemType>* item3;
		ItemType item = nodePtr->getItem();

		do
		{
			while (nodePtr != 0)
			{
				if (nodePtr->getRightPtr() != 0)
				{
					stack.push(nodePtr->getRightPtr());
				}
				stack.push(nodePtr);
				nodePtr = nodePtr->getLeftPtr();
			}

			stack.pop(nodePtr);
			if (stack.isEmpty() == false)
			{
				stack.pop(item3);
				stack.push(item3);
			}
			else
			{
				item3 = 0;
			}
			nimrod = nodePtr->getRightPtr();

			if (nodePtr->getRightPtr() != 0 && item3 == nimrod)
			{
				
				stack.pop(item3);
				stack.push(nodePtr);
				nodePtr = nodePtr->getRightPtr();
			}
			else
			{
				item = nodePtr->getItem();
				visit(item);
				nodePtr = 0;
			}
		} while (stack.isEmpty() == false);
}

// Iterative PreOrder
template<class ItemType>
void BinaryTree<ItemType>::_iterativePreOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{

	if (nodePtr != 0)
	{
		Stack<BinaryNode<ItemType>*> stack;
		stack.push(nodePtr);
		ItemType item = nodePtr->getItem();

		while (stack.isEmpty() == false)
		{
			stack.pop(nodePtr);
			item = nodePtr->getItem();
			visit(item);

			if (nodePtr->getRightPtr() != 0)
			{
				stack.push(nodePtr->getRightPtr());
			}
			if (nodePtr->getLeftPtr() != 0)
			{
				stack.push(nodePtr->getLeftPtr());
			}

		}
	}

}


//Prints list in PreOrder (Root, Right, Left)
// Uses and additional parameter, int level, to display the correct level as it's printing.
template<class ItemType>
void BinaryTree<ItemType>::_print(void visit(ItemType&), BinaryNode<ItemType>* nodePtr, int level) const
{
	if (nodePtr != 0)
	{
		level++;
		ItemType item = nodePtr->getItem();
		cout << setw(8 * (level)) << "Level " << level << ": ";
		visit(item);
		_print(visit, nodePtr->getRightPtr(), level);
		_print(visit, nodePtr->getLeftPtr(), level);
	}
	else
	{
		level--;
	}
}


#endif

