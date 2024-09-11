#pragma once
#include "node.h"

template<typename T>
class LinkedList {
public:
	LinkedList();
	~LinkedList();

	LinkedList(const LinkedList<T>& other);
	LinkedList<T>& operator=(const LinkedList<T>& other);
	LinkedList<T>& operator+=(const LinkedList<T>& other);

public:
	// Build Linked List
	void insertAtBeginning(const T& value);
	void insertAtEnd(const T& value);
	void insertSorted(const T& value);

	// Query and Controls
	T getValue();
	T& getValueByRef();
	const T& getValue() const;
	Node<T>* getHead() const;
	Node<T>* getCurrentNode();
	Node<T>* getTail() const;
	int getSize() const;
	bool hasNext();
	void next();
	void reset();

	T* linearSearch(const T& value);
	T* binarySearch(const T& value);
	void bubbleSort();
	void quickSort();

	// Print
	void display();

private:
	Node<T>* head;
	Node<T>* current;
	Node<T>* tail;

	uint32_t size;
	
private:
	void copyFrom(const LinkedList<T>& other);
	void swapValues(Node<T>* x, Node<T>* y);
	void swapNodes(Node<T>* x, Node<T>* y);
	Node<T>* quickSortRecursive(Node<T>* head, Node<T>* end);
	Node<T>* partition(Node<T>* head, Node<T>* end, Node<T>** newHead, Node<T>** newEnd);
	Node<T>* concatenate(Node<T>* left, Node<T>* pivot, Node<T>* right);
	Node<T>* getTailNode(Node<T>* node);
};
