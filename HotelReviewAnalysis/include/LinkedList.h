#pragma once
#include "Node.h"

class LinkedList {
public:

	LinkedList() : head(NULL), tail(NULL) { }
	~LinkedList();

	void insertAtBeginning(int value);
	void insertAtEnd(int value);
	void display();

private:
	Node* head;
	Node* tail;
};

