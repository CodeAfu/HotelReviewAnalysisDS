#include <iostream>
#include <functional>

#include "linkedlist.h"
#include "review.h"
#include "review_stats.h"
#include "result.h"
#include "word.h"

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), current(nullptr), tail(nullptr), size(0) { }

template<typename T>
LinkedList<T>::~LinkedList() {
	Node<T>* n = head;
	while (n) {
		Node<T>* next = n->next;
		delete n;
		size--;
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
		size++;
		otherNode = otherNode->next;
	}

	return *this;
}

template<typename T>
void LinkedList<T>::insertAtBeginning(const T& val) {
	Node<T>* n = new Node<T>();
	n->value = val;
	n->next = head;

	if (!head) {
		tail = n;
		current = n;
	}

	head = n;
	size++;
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
		size++;
		return;
	} 

	tail->next = n;
	tail = n;
	size++;
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
		size++;
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
	size++;
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
int LinkedList<T>::getSize() const {
	return size;
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
		} else if (mid->value < value) {
			start = mid->next;
		} else {
			end = mid;
		}
	}
	return nullptr;
}

template<typename T>
void LinkedList<T>::bubbleSort() {
	throw std::runtime_error("Sorting algorithms are only compatible with the 'Word' struct.");
}

template<>
void LinkedList<Word>::bubbleSort() {
	if (head == nullptr) {
		return;
	}

	bool swapped;
	Node<Word>* current = head;
	Node<Word>* lastSorted = nullptr;

	do {
		swapped = false;
		current = head;
		
		while (current->next != lastSorted) {
			if (current->next->value.count < current->value.count) {
				swapValues(current, current->next);
				swapped = true;
			}
			current = current->next;
		}
		lastSorted = current;
	} while (swapped);
}

template<typename T>
void LinkedList<T>::quickSort() {
	this->head = quickSortRecursive(this->head, this->getTail());
}

template<typename T>
void LinkedList<T>::sort() {
	if (head == nullptr) {
		return;
	}

	bool swapped;
	Node<T>* current = head;
	Node<T>* lastSorted = nullptr;

	do {
		swapped = false;
		current = head;

		while (current->next != lastSorted) {
			if (current->next->value < current->value) {
				swapValues(current, current->next);
				swapped = true;
			}
			current = current->next;
		}
		lastSorted = current;
	} while (swapped);
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
		size++;
	}
}

template<typename T>
void LinkedList<T>::swapValues(Node<T>* x, Node<T>* y) {
	T temp = x->value;
	x->value = y->value;
	y->value = temp;
}

// O(n)
template<typename T>
void LinkedList<T>::swapNodes(Node<T>* x, Node<T>* y) {
	if (x == y || !x || !y) {
		return;
	}

	Node<T>* prevX = nullptr;
	Node<T>* prevY = nullptr;
	Node<T>* curr = this->head;

	while (curr && curr->next) {
		if (curr->next == x) {
			prevX = curr;
		}
		if (curr->next == y) {
			prevY = curr;
		}
		curr = curr->next;
	}

	// Check if prevX or prevY is null
	if (!prevX && head != x || !prevY && head != y) {
		return;
	}

	// Change next pointers
	if (prevX) {
		prevX->next = y;
	} else {
		head = y;
	}
	if (prevY) {
		prevY->next = x;
	} else {
		head = x;
	}

	Node<T>* temp = x->next;
	x->next = y->next;
	y->next = temp;
}

template<typename T>
Node<T>* LinkedList<T>::quickSortRecursive(Node<T>* head, Node<T>* end) {
	if (head == nullptr || head == end) {
		return head;
	}

	Node<T>* newHead = nullptr;
	Node<T>* newEnd = nullptr;

	Node<T>* pivot = partition(head, end, &newHead, &newEnd);

	if (newHead != pivot) {
		Node<T>* temp = newHead;
		while (temp->next != pivot) {
			temp = temp->next;
		}
		temp->next = nullptr;
		newHead = quickSortRecursive(newHead, temp);
		temp = getTailNode(newHead);
		temp->next = pivot;
	}

	pivot->next = quickSortRecursive(pivot->next, newEnd);

	return newHead;
}

template<typename T>
Node<T>* LinkedList<T>::partition(Node<T>* head, Node<T>* end, Node<T>** newHead, Node<T>** newEnd) {
	throw std::runtime_error("Quick Sort is only compatible with with the 'Word' struct.");
}

template<>
Node<Word>* LinkedList<Word>::partition(Node<Word>* head, Node<Word>* end, Node<Word>** newHead, Node<Word>** newEnd) {
	Node<Word>* pivot = end;
	Node<Word>* prev = nullptr;
	Node<Word>* current = head;
	Node<Word>* tail = pivot;

	*newHead = nullptr;
	*newEnd = nullptr;

	while (current != pivot) {
		// Primary sorting by count, secondary sorting by alphabetical order when counts are equal
		if (current->value.count < pivot->value.count ||
			(current->value.count == pivot->value.count && current->value.word < pivot->value.word)) {
			// Place current node in the sorted portion
			if (*newHead == nullptr) {
				*newHead = current;
			}
			prev = current;
			current = current->next;
		}
		else {
			// Place current node in the unsorted portion
			if (prev) {
				prev->next = current->next;
			}
			Node<Word>* temp = current->next;
			current->next = nullptr;
			tail->next = current;
			tail = current;
			current = temp;
		}
	}

	if (*newHead == nullptr) {
		*newHead = pivot;
	}

	*newEnd = tail;

	return pivot;
}

template<typename T>
Node<T>* LinkedList<T>::concatenate(Node<T>* left, Node<T>* pivot, Node<T>* right) {
	Node<T>* result = nullptr;

	if (left != nullptr) {
		result = left;
		Node<T>* tail = getTailNode(left);
		tail->next = pivot;
	} else {
		result = pivot;
	}

	pivot->next = right;
	return result;
}

template<typename T>
Node<T>* LinkedList<T>::getTailNode(Node<T>* node)	{
	while (node != nullptr && node->next != nullptr) {
		node = node->next;
	}
	return node;
}

template class LinkedList<Review>;
template class LinkedList<ReviewStats>;
template class LinkedList<Word>;
template class LinkedList<std::string>;