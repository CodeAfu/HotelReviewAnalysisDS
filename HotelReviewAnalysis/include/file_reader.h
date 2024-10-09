#pragma once
#include "review.h"

namespace filereader {
	LinkedList<std::string> asLLString(const std::string& fileName);
	LinkedList<Review> asLLReview(const std::string& fileName);
}
