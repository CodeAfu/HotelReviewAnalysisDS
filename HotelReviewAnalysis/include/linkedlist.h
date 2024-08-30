#pragma once
#include "node.h"

class LinkedList {
public:
	LinkedList() : head(NULL), current(NULL) { }
	~LinkedList();

	// Build Linked List
	void insertAtBeginning(const std::string value);
	void insertAtEnd(const std::string value);

	// Query and Controls
	std::string getValue();
	Node* getCurrentNode();
	Node* searchValue(const std::string& value);
	bool hasNext();
	void next();
	void reset();

	// Print
	void display();

private:
	Node* head;
	Node* current;
	Node* tail;
};

