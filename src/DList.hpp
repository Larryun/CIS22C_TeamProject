// Specification file for the LinkedList class
// Written By: A. Student
// Changed By: Meng Leong Un
// IDE: Visual Studio

#ifndef D_LIST_H
#define D_LIST_H
#include "ListNode.hpp"

// DList Template Definition
template<class T>
class DList{
private:
    ListNode<T> *head;
    int count;
    
public:
    DList();   // Constructor
    ~DList();  // Destructor
    
    // Linked list operations
    int getCount() const {return count;}
    void insertNode(T&);
    bool deleteNode(T);
	void traverseForward(void printData(T &)) const;
	void traverseBackward(void printData(T &)) const;
    bool searchList(T, T &) const;
	bool getNodeAtIndex(int, T&);
	bool isEmpty();
};


// DList Template Implementation

//**************************************************
// Constructor of DList
//**************************************************
template<class T>
DList<T>::DList()
{
    head = new ListNode<T>();
	head->setForw(head);
	head->setBack(head);
    count = 0;
}

//**************************************************
// traverseForward shows the value
// stored in each node of the linked list from the start
// pointed to by head.
//**************************************************
template<class T>
void DList<T>::traverseForward(void printData(T &)) const
{
    ListNode<T> *pCur;
	T data;
    pCur = head->getForw();
    while(pCur != head)
    {
		data = pCur->getData();
		printData(data);
        pCur = pCur->getForw();
    }
    //std::cout << "====================================================" << std::endl;
}

//**************************************************
// traverserBackward shows the value
// stored in each node of the linked list from the end
// pointed to by head.
//**************************************************
template<class T>
void DList<T>::traverseBackward(void printCity(T &)) const
{
    ListNode<T> *pCur;
	T data;
    pCur = head->getBack();
    while(pCur != head)
    {
		data = pCur->getData();
		printCity(pCur->getData());
        pCur = pCur->getBack();
    }
	std::cout << "====================================================" << std::endl;
}
//**************************************************
// The searchList function looks for a target city
// in the sorted linked list: if found, returns true
// and copied the data in that node to the output parameter
//**************************************************
template<class T>
bool DList<T>::searchList(T targetData, T &dataOut) const
{
    bool found = false;
    ListNode<T> *pCur = head->getForw();

    while(pCur != head && pCur->getData() < targetData)
        pCur = pCur->getForw();
    
    if(pCur != head && pCur->getData() == targetData)
    {
        dataOut = pCur->getData();
        found = true;
    }
    
    return found;
}

//**************************************************
// The insertNode function inserts a new node in a
// sorted linked list
//**************************************************
template<class T>
void DList<T>::insertNode(T &dataIn)
{
    ListNode<T> *newNode;
    ListNode<T> *pPre;
    ListNode<T> *pCur;
    
    newNode = new ListNode<T>();
	newNode->setData(dataIn);
    
    pPre = head;
	pCur = head->getForw();
    while(pCur != head && pCur->getData() < dataIn)
    {
        pPre = pCur;
		pCur = pCur->getForw();
    }
    
	pPre->setForw(newNode);
	newNode->setBack(pPre);
	newNode->setForw(pCur);
	pCur->setBack(newNode);
    
    count++;
}

//**************************************************
// The deleteNode function searches for a node
// in a sorted linked list; if found, the node is
// deleted from the list and from memory.
//**************************************************
template<class T>
bool DList<T>::deleteNode(T target)
{
    ListNode<T> *pCur;
    ListNode<T> *pPre;
    bool deleted = false;

    pPre = head;
    pCur = head->getForw();
    while(pCur != head && pCur->getData() < target)
    {
        pPre = pCur;
        pCur = pCur->getForw();
    }
    
    if(pCur != head && pCur->getData() == target)
    {
		pPre->setForw(pCur->getForw());
		pCur->getForw()->setBack(pPre);
        delete pCur;
        deleted = true;
        count--;
    }
    return deleted;
}

template<class T>
bool DList<T>::getNodeAtIndex(int index, T& dataOut)
{
	ListNode<T>* pCur = head->getForw();
	int i = 0;
	bool found = false;
	if (i >= count) return false;
	while (pCur != head && i < index)
	{
		pCur = pCur->getForw();
		i++;
	}

    if(pCur != head && i == index)
    {
        dataOut = pCur->getData();
        found = true;
    }
    
    return found;
}

//**************************************************
// isEmpty
// check if the list is empty
//**************************************************
template<class T>
bool DList<T>::isEmpty()
{
	return head->getForw() == head;
}

//**************************************************
// Destructor                                      *
// This function deletes every node in the list.   *
//**************************************************
template<class T>
DList<T>::~DList()
{
    ListNode<T> *pCur;
    ListNode<T> *pNext;
    
    pCur = head->getForw();
    while(pCur != head)
    {
        pNext = pCur->getForw();
        //std::cout << "DEBUG - Destructor: Now deleting " << pCur->getData().getName() << std::endl;
        delete pCur;
        pCur = pNext;
    }
    //std::cout << "DEBUG - Destructor: Now deleting the sentinel node: " << head->getData().getYear() << std::endl;
    delete head;
}
#endif

