#pragma once

struct Node {
	std::string value;
	Node* next;

	Node() { }
	Node(std::string val, Node* nxt) : value(val), next(nxt) { }
};