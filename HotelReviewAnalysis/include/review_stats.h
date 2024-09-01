#pragma once

#include "review.h"

struct ReviewStats {
	Review review;
	LinkedList<std::string> wordsPos;
	LinkedList<std::string> wordsNeg;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	unsigned int numWords = 0;

	/// TODO: Add Semantic Analysis Data Here
	
	bool operator==(const ReviewStats& other) const;
	bool operator<(const ReviewStats& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ReviewStats& review);

	void log();
};
