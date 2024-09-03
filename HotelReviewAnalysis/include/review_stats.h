#pragma once

#include <chrono>
#include "word.h"
#include "review.h"

struct ReviewStats {
	Review review;
	LinkedList<Word> wordsPos;
	LinkedList<Word> wordsNeg;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	unsigned int numWords = 0;
	std::chrono::microseconds timeInMillis;

	/// TODO: Add Semantic Analysis Data Here
	
	bool operator==(const ReviewStats& other) const;
	bool operator<(const ReviewStats& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ReviewStats& review);

	void log();
};
