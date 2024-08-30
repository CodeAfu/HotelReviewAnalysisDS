#pragma once

#include "node.h"
#include "review.h"

template<typename T>
class LinkedList {
public:
	LinkedList();
	~LinkedList();

public:
	LinkedList(const LinkedList<T>& other);
	LinkedList<T>& operator=(const LinkedList<T>& other);

public:
	// Build Linked List
	void insertAtBeginning(const T& value);
	void insertAtEnd(const T& value);

	// Query and Controls
	T getValue();
	T& getValueByRef();
	const T& getValue() const;
	Node<T>* getCurrentNode();
	Node<T>* searchValue(const T& value);
	bool hasNext();
	void next();
	void reset();

	// Print
	void display();

private:
	Node<T>* head;
	Node<T>* current;
	Node<T>* tail;

	void copyFrom(const LinkedList<T>& other);
};

extern template class LinkedList<Review>;
extern template class LinkedList<std::string>;