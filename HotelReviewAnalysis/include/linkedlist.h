#pragma once

#include "node.h"
//#include "review.h"

#if 0
template<typename T>
class LinkedList;

template<typename T>
class LinkedListIterator {
public:
	using NodeType = Node<T>;
	using PointerType = NodeType*;
	using ReferenceType = T&;

public:
	LinkedListIterator(PointerType ptr) : m_Ptr(ptr) { }

	ReferenceType operator*() const {
		return m_Ptr->value;
	}

	LinkedListIterator& operator++() {
		m_Ptr = m_Ptr->next;
		return *this;
	}

	LinkedListIterator operator++(int) {
		LinkedListIterator temp = *this;
		m_Ptr = m_Pr->next;
		return temp;
	}

	bool operator==(const LinkedListIterator& other) const {
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const LinkedListIterator& other) const {
		return m_Ptr != other.m_Ptr;
	}

	ReferenceType operator*() const {
		return m_Ptr->value;
	}

private:
	PointerType m_Ptr;
};
#endif

template<typename T>
class LinkedList {

#if 0
public:
	using ValueType = T;
	using Iterator = LinkedListIterator<LinkedList<T>>;
#endif

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
	bool hasNext();
	void next();
	void reset();

	T* linearSearch(const T& value);
	T* binarySearch(const T& value);

	// Print
	void display();

#if 0
public:
	// Iterator Implementation
	Iterator begin() { return Iterator(head); }
	Iterator end() { return Iterator(tail->next); }
#endif

private:
	Node<T>* head;
	Node<T>* current;
	Node<T>* tail;

	void copyFrom(const LinkedList<T>& other);
};
