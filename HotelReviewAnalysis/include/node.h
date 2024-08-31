#pragma once

template<typename T>
struct Node {
	T value;
	Node* next;

	Node() { }
	Node(T& val) : value(val), next(nullptr) { }
	Node(T& val, Node* nxt) : value(val), next(nxt) { }
};