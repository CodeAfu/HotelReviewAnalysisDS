#include <iostream>
#include "linkedlist.h"
#include "Node.h";

LinkedList::~LinkedList() {
	Node* n = head;
	while (n) {
		Node* next = n->next;
		delete n;
		n = next;
	}
}

void LinkedList::insertAtBeginning(const std::string val) {
	Node* n = new Node();

	n->value = val;
	n->next = head;
	head = n;

	if (!current) {
		current = n;
		tail = n;
	}
}

void LinkedList::insertAtEnd(const std::string val) {
	Node* n = new Node();
	n->value = val;
	n->next = NULL;
	tail = n;

	if (!head) {
		head = n;
		current = n;
		tail = n;
		return;
	} 

	Node* temp = head;
	while (temp->next) {
		temp = temp->next;
	}

	temp->next = n;
}

std::string LinkedList::getValue() {
	if (!current) {
		throw std::runtime_error("Current Node is null.");
	}
	return current->value;
}

Node* LinkedList::getCurrentNode() {
	if (!current) {
		throw std::runtime_error("Current node is null.");
	}
	return current;
}

Node* LinkedList::searchValue(const std::string& value) {
	Node* n = current;
	if (current == tail) {
		std::cout << "[LOG] Linked List current pointer needs to be reset.\n";
		std::cout << "[WARNING] Returning Null Pointer.\n";
		return nullptr;
	}

	while (n) {
		if (n->value == value) {
			return n;
		}
		n = n->next;
	}
	return nullptr;
}


bool LinkedList::hasNext() {
	return current && current->next;
}

void LinkedList::next() {
	if (!hasNext()) {
		std::cout << "No new node available.\n";
	}
	current = current->next;
}

void LinkedList::reset() {
	if (!current) {
		throw std::runtime_error("Current Node is null.");
	}
	current = head;
}

void LinkedList::display() {
	if (!head) {
		std::cout << "LinkedList not initialized\n";
		return;
	}

	Node* temp = head;

	while (temp) {
		std::cout << temp->value << " -> ";
		temp = temp->next;
	}
	std::cout << "NULL\n";
}

