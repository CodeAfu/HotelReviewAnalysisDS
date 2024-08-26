#pragma once
#include "node.h"

class LinkedList {
public:

	LinkedList() : head(NULL), tail(NULL) { }
	~LinkedList();

	void insertAtBeginning(std::string value);
	void insertAtEnd(std::string value);
	void display();

private:
	Node* head;
	Node* tail;
};

