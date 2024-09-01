#include <iostream>

#include "linkedlist.h"
#include "review_stats.h"

bool ReviewStats::operator==(const ReviewStats& other) const {
	return review == other.review && &wordsPos == &other.wordsPos && &wordsNeg == &other.wordsNeg
		&& numPos == other.numPos && numNeg == other.numNeg && numWords == other.numWords;
}

bool ReviewStats::operator<(const ReviewStats& other) const {
	return this->review < other.review;
}

std::ostream& operator<<(std::ostream& os, const ReviewStats& reviewStats) {
	os << reviewStats.review;
	return os;
}

void ReviewStats::log() {
	std::cout << "Comment: " << review.comment << std::endl;
	std::cout << "Rating: " << review.rating << std::endl;
	std::cout << "Positive Words: "; wordsPos.display();
	std::cout << "Negative Words: "; wordsNeg.display();
	std::cout << "Positive Num: " << numPos << std::endl;
	std::cout << "Negative Num: " << numNeg << std::endl;
	std::cout << "Num Words: " << numWords << std::endl;
}

template class LinkedList<std::string>;
