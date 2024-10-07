#include <iostream>
#include <chrono>

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

void ReviewStats::calculateSentimentScore() {
	const int16_t rawSentiment = numPos - numNeg;
	const int16_t maxRawScore = numPos + numNeg;

	if (maxRawScore == 0) {
		this->rawSentimentScore = 3;
		return;
	}

	const double normalizedScore = static_cast<double>(rawSentiment) / maxRawScore;
	this->rawSentimentScore = static_cast<int>(3 + 2 * normalizedScore);
}

void ReviewStats::log() {
	std::cout << "Review #" << review.id << std::endl;
	std::cout << "Comment: " << review.comment << std::endl;
	std::cout << "User Rating: " << review.rating << std::endl;
	std::cout << "Sentiment Rating: " << rawSentimentScore << std::endl;
	std::cout << "Positive Words: "; wordsPos.display();
	std::cout << "Negative Words: "; wordsNeg.display();
	std::cout << "Positive Num: " << numPos << std::endl;
	std::cout << "Negative Num: " << numNeg << std::endl;
	std::cout << "Num Words: " << numWords << std::endl;
	std::cout << "Time Taken to Calculate: " << timeInMicroseconds << std::endl;
}

