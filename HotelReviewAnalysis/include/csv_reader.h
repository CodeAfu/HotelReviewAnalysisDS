#pragma once
#include "review.h"

namespace csvreader {
	std::string asString(const std::string& fileName);
	void asArr(std::string*& arr, size_t& count, const std::string& fileName);
	LinkedList<std::string> asLLString(const std::string& fileName);
	LinkedList<Review> asLLReview(const std::string& fileName);
}
