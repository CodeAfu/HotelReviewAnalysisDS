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

	/// TODO: Add Semantic Analysis Data Here
	void calculateSentimentScore() {
		const int16_t rawSentiment = numPos - numNeg;
		const int16_t maxRawScore = numPos + numNeg;
		const int16_t minRawScore = -maxRawScore;
		const long normalizedScore = static_cast<double>(rawSentiment - minRawScore) / (maxRawScore - minRawScore);
		this->rawSentimentScore = static_cast<int>(1 + (4 * normalizedScore));
	}

	bool operator==(const ReviewStats& other) const;
	bool operator<(const ReviewStats& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ReviewStats& review);

	void log();
};
