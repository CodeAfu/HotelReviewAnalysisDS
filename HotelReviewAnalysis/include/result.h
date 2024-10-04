#pragma once

#include <chrono>

/// <summary>
/// Stores detailed data of ALL reviews.
/// </summary>
struct Result {
	LinkedList<ReviewStats> reviewStats; // Information about each review, iterate through each to get details
	LinkedList<std::string> wordsPosRaw;
	LinkedList<std::string> wordsNegRaw;
	LinkedList<Word> wordsPos;
	LinkedList<Word> wordsNeg;
	unsigned int numReviews = 0;
	unsigned int numWords = 0;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	std::chrono::milliseconds duration;

	void log();

	Result& operator+=(const ReviewStats rhs);
};