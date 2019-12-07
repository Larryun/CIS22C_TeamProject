// Binary tree abstract base class
// Created by A. Student
// Modified by: Tommy Vu, Steven Vu

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
	void inOrderOutput(void visit(ItemType&, ofstream&), ofstream& outFile) const { _inorder(visit, rootPtr, outFile); }
	void postOrder(void visit(ItemType&)) const { _postorder(visit, rootPtr); }
	void breadthTrav(void visit(ItemType&)) const { _breadthTrav(visit, rootPtr); }
	void iterativePreOrder(void visit(ItemType&)) const { _iterativePreOrder(visit, rootPtr); }
	void iterativeInOrder(void visit(ItemType&)) const { _iterativeInOrder(visit, rootPtr); }
	void iterativePostOrder(void visit(ItemType&)) const { _iterativePostOrder(visit, rootPtr); }
	void print(void visit(ItemType&)) const { _print(visit, rootPtr, 0); }
	void indentedTraversal(void visitIndented(int, ItemType&)) const { _indentedTraversal(0, visitIndented, rootPtr); }

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
	void _inorder(void visit(ItemType&, ofstream&), BinaryNode<ItemType>* nodePtr, ofstream& outFile) const;
	void _postorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _breadthTrav(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _iterativePreOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _iterativeInOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _iterativePostOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _indentedTraversal(int level, void visit(int, ItemType&), BinaryNode<ItemType>* nodePtr) const;
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

//Inorder Traversal For Output
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType&, ofstream&), BinaryNode<ItemType>* nodePtr, ofstream& outFile) const
{
	if (nodePtr != 0)
	{
		ItemType dummy;
		_inorder(visit, nodePtr->getLeftPtr(), outFile);
		dummy = nodePtr->getItem();
		visit(dummy, outFile);
		_inorder(visit, nodePtr->getRightPtr(), outFile);
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

template<class ItemType>
void BinaryTree<ItemType>::_iterativePostOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		Stack<BinaryNode<ItemType>*> s, output;
		BinaryNode<ItemType>* poppedNode;
		ItemType item;
		s.push(nodePtr);

		while (!s.isEmpty())
		{
			s.pop(poppedNode);
			output.push(poppedNode);

			if (poppedNode->getLeftPtr())
			{
				s.push(poppedNode->getLeftPtr());
			}
			if (poppedNode->getRightPtr())
			{
				s.push(poppedNode->getRightPtr());
			}
		}

		while (!output.isEmpty())
		{
			output.pop(poppedNode);
			item = poppedNode->getItem();
			visit(item);
		}
	}
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


template<class ItemType>
void BinaryTree<ItemType>::_iterativeInOrder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		Stack<BinaryNode<ItemType>*> s;
		BinaryNode<ItemType>* curr = nodePtr;
		ItemType item;
		while (!s.isEmpty() || curr)
		{
			if (curr != 0)
			{
				s.push(curr);
				curr = curr->getLeftPtr();
			}
			else
			{
				s.pop(curr);
				visit(item = curr->getItem());
				curr = curr->getRightPtr();
			}
		}
	}
}

template<class ItemType>
void BinaryTree<ItemType>::_indentedTraversal(int level, void visit(int, ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		ItemType item = nodePtr->getItem();
		visit(level, item);
		_indentedTraversal(level + 1, visit, nodePtr->getRightPtr());
		_indentedTraversal(level + 1, visit, nodePtr->getLeftPtr());
	}
}



#endif