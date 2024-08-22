#include <iostream>
#include "LinkedList.h"
#include "Node.h";

LinkedList::~LinkedList() {
	Node* current = head;
	while (current) {
		Node* next = current->next;
		delete current;
		current = next;
	}
}

void LinkedList::insertAtBeginning(int val) {
	Node* n = new Node();
	n->value = val;
	n->next = head;
	head = n;
}

void LinkedList::insertAtEnd(int val) {
	Node* n = new Node();
	n->value = val;
	n->next = NULL;

	if (!head) {
		head = n;
		return;
	}

	Node* temp = head;
	while (temp->next) {
		temp = temp->next;
	}

	temp->next = n;
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

