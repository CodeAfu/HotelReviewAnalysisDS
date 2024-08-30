#include <iostream>
#include "linkedlist.h";

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
	tail = n;

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
Node<T>* LinkedList<T>::getCurrentNode() {
	if (!current) {
		throw std::runtime_error("Current node is null.");
	}
	return current;
}

template<typename T>
Node<T>* LinkedList<T>::searchValue(const T& value) {
	/// Test
	//Node<T>* n = current;
	//if (current != head) {
	//	std::cout << "[LOG] Linked List current pointer needs to be reset.\n";
	//	std::cout << "[WARNING] Returning Null Pointer.\n";
	//	return nullptr;
	//}
	Node<T>* n = head;
	while (n) {
		if (n->value == value) {
			return n;
		}
		n = n->next;
	}
	return nullptr;
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
	if (!current) {
		throw std::runtime_error("Current Node is null.");
	}
	current = head;
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
template class LinkedList<std::string>;