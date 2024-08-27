#pragma once
#include "node.h"

class LinkedList {
public:

	LinkedList() : head(NULL), current(NULL) { }
	~LinkedList();

	void insertAtBeginning(std::string value);
	void insertAtEnd(std::string value);

	std::string getValue();
	bool hasNext();
	void next();
	void reset();

	void display();

private:
	Node* head;
	Node* current;
};

