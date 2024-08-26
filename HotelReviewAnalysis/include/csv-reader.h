#pragma once
#include "linkedlist.h"

namespace csvreader {
	std::string asString(std::string fileName);
	void asArr(std::string*& arr, size_t& count, const std::string& fileName);
	LinkedList asLL(std::string fileName);
}
