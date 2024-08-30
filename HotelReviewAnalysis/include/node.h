#pragma once

template<typename T>
struct Node {
	T value;
	Node* next;

	Node() { }
	Node(T& val, Node* nxt) : value(val), next(nxt) { }
	Node(T& val) : value(val), next(nullptr) { }
};