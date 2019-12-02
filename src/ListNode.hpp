// Specification file for the ListNode class
// Written By: Meng Leong Un
// Changed By: Meng Leong Un
// IDE: Visual Studio

#include <iostream>
#ifndef LISTNODE_H
#define LISTNODE_H

// DList Template Definition
template<class T>
class ListNode
{
private:
	T data;
	ListNode<T>* forw;
	ListNode<T>* back;
		
public:
	//Constructor
	ListNode();
	ListNode(T &value) {
		forw = NULL;
		back = NULL;
		data = value;
	}

	//getters
	T getData();
	ListNode<T>* getForw();
	ListNode<T>* getBack();

	//setters
	void setData(T&);
	void setBack(ListNode<T>*);
	void setForw(ListNode<T>*);
};


// DList Template Implementation

//***********************************************************
// Constructor
//***********************************************************
template <class T>
ListNode<T>::ListNode() {
	forw = NULL;
	back = NULL;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
T ListNode<T>::getData() {
	return data;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
ListNode<T>* ListNode<T>::getForw() {
	return forw;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
ListNode<T>* ListNode<T>::getBack() {
	return back;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
void ListNode<T>::setData(T &value) {
	 data = value;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
void ListNode<T>::setForw(ListNode<T>* nextNode) {
	forw = nextNode;
}

//***********************************************************
// Getter
//***********************************************************
template <class T>
void ListNode<T>::setBack(ListNode<T>* nextNode) {
	back = nextNode;
}
#endif
