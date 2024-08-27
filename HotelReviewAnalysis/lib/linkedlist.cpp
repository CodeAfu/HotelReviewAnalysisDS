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

void LinkedList::insertAtBeginning(std::string val) {
	Node* n = new Node();

	n->value = val;
	n->next = head;
	head = n;

	if (!head) {
		current = n;
	}
}

void LinkedList::insertAtEnd(std::string val) {
	Node* n = new Node();
	n->value = val;
	n->next = NULL;

	if (!head) {
		head = n;
		current = n;
		return;
	}

	Node* temp = head;
	while (temp->next) {
		temp = temp->next;
	}

	temp->next = n;
}

std::string LinkedList::getValue() {
	return current->value;
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

