#pragma once
#include <iostream>
#include "linkedlist.h"

namespace csvreader {
	std::string asString(const std::string& fileName);
	void asArr(std::string*& arr, size_t& count, const std::string& fileName);
	LinkedList asLL(const std::string& fileName);
}
