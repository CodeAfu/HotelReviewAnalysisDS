#pragma once

#include <chrono>
#include "linkedlist.h"
#include "word.h"
#include "review.h"

/// <summary>
/// Stores detailed data of EACH review.
/// </summary>
struct ReviewStats {
	Review review;
	LinkedList<Word> wordsPos;
	LinkedList<Word> wordsNeg;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	unsigned int numWords = 0;
	std::chrono::microseconds timeInMillis;
	int rawSentimentScore;

	void calculateSentimentScore();

	bool operator==(const ReviewStats& other) const;
	bool operator<(const ReviewStats& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ReviewStats& review);

	void log();
};
