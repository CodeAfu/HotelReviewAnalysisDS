#include <iostream>
#include "linkedlist.h";
#include "review.h";
#include "review_stats.h"
#include "word.h"

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), current(nullptr), tail(nullptr) { }

template<typename T>
LinkedList<T>::~LinkedList() {
	Node<T>* n = head;
	while (n) {
		Node<T>* next = n->next;
		delete n;
		n = next;
	}
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) 
	: head(nullptr), current(nullptr), tail(nullptr) {
	copyFrom(other);
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
	if (this != &other) {
		this->~LinkedList();
		head = current = tail = nullptr;
		Node<T>* temp = other.head;
		while (temp) {
			insertAtEnd(temp->value);
			temp = temp->next;
		}
	}
	return *this;
}


template<typename T>
LinkedList<T>& LinkedList<T>::operator+=(const LinkedList<T>& other) {
	if (other.head == nullptr) {
		return *this;
	}

	Node<T>* otherNode = other.head;

	while (otherNode != nullptr) {
		this->insertAtEnd(otherNode->value);
		otherNode = otherNode->next;
	}

	return *this;
}

template<typename T>
void LinkedList<T>::insertAtBeginning(const T& val) {
	Node<T>* n = new Node<T>();
	n->value = val;
	n->next = head;
	head = n;

	if (!current) {
		current = n;
		tail = n;
	}
}

template<typename T>
void LinkedList<T>::insertAtEnd(const T& val) {
	Node<T>* n = new Node<T>();
	n->value = val;
	n->next = nullptr;

	if (!head) {
		head = n;
		current = n;
		tail = n;
		return;
	} 

	tail->next = n;
	tail = n;
}

template<typename T>
void LinkedList<T>::insertSorted(const T& val) {
	Node<T>* n = new Node<T>();
	n->value = val;
	n->next = nullptr;

	// Handle Uninitialized and val > head
	if (!head || val < head->value) {
		n->next = head;
		head = n;
		if (!tail) {
			tail = n;
		}
		return;
	}

	// Handle in-between
	Node<T>* curr = head;
	while (curr->next && curr->next->value < val) {
		curr = curr->next;
	}

	n->next = curr->next;
	curr->next = n;

	if (!n->next) {
		tail = n;
	}
}

template<typename T>
T LinkedList<T>::getValue() {
	if (!current) {
		throw std::runtime_error("Current Node is null.");
	}
	return current->value;
}

template<typename T>
T& LinkedList<T>::getValueByRef() {
	if (!current) {
		throw std::runtime_error("Current Node is null.");
	}
	return current->value;
}

template<typename T>
const T& LinkedList<T>::getValue() const {
	if (!current) {
		throw std::runtime_error("Current node is null.");
	}
	return current->value;
}

template<typename T>
Node<T>* LinkedList<T>::getHead() const {
	//if (!head) {
	//	std::cerr << "Head node is null.\n";
	//}
	return head;
}

template<typename T>
Node<T>* LinkedList<T>::getCurrentNode() {
	if (!current) {
		throw std::runtime_error("Current node is null.");
	}
	return current;
}

template<typename T>
Node<T>* LinkedList<T>::getTail() const {
	//if (!tail) {
	//	std::cerr << "Tail node is null.\n";
	//}
	return tail;
}

template<typename T>
bool LinkedList<T>::hasNext() {
	return current && current->next;
}

template<typename T>
void LinkedList<T>::next() {
	if (!hasNext()) {
		std::cout << "No new node available.\n";
		return;
	}
	current = current->next;
}

template<typename T>
void LinkedList<T>::reset() {
	if (!current && !head && !tail) {
		throw std::runtime_error("Current Node is null.");
	}
	current = head;
}

template<typename T>
T* LinkedList<T>::linearSearch(const T& value) {
	Node<T>* n = head;
	while (n) {
		if (n->value == value) {
			return &(n->value);
		}
		n = n->next;
	}
	return nullptr;
}

template<typename T>
T* LinkedList<T>::binarySearch(const T& value) {
	Node<T>* start = head;
	Node<T>* end = nullptr;

	while (start != end) {
		Node<T>* mid = start;
		Node<T>* fast = start->next;

		while (fast != end && fast->next != end) {
			mid = mid->next;
			fast = fast->next->next;
		}

		if (mid->value == value) {
			return &(mid->value);
		}
		else if (mid->value < value) {
			start = mid->next;
		} else {
			end = mid;
		}
	}
	return nullptr;
}

template<typename T>
void LinkedList<T>::display() {
	if (!head) {
		std::cout << "LinkedList not initialized\n";
		return;
	}

	Node<T>* temp = head;

	while (temp) {
		std::cout << temp->value << " -> ";
		temp = temp->next;
	}
	std::cout << "NULL\n";
}

template<typename T>
void LinkedList<T>::copyFrom(const LinkedList<T>& other) {
	Node<T>* temp = other.head;
	while (temp) {
		insertAtEnd(temp->value);
		temp = temp->next;
	}
}

template class LinkedList<Review>;
template class LinkedList<ReviewStats>;
template class LinkedList<Word>;
template class LinkedList<std::string>;