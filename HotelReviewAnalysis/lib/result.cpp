#include <iostream>

#include "review_stats.h"
#include "linkedlist.h"
#include "result.h"

void Result::log() {
	std::cout << "Number of Reviews: " << numReviews << std::endl;
	std::cout << "Number of Words: " << numWords << std::endl;
	std::cout << "Number of Positives: " << numPos << std::endl;
	std::cout << "Number of Negatives: " << numNeg << std::endl;
	std::cout << "Time elapsed: {}" << duration << std::endl;
}

Result& Result::operator+=(const ReviewStats rhs) {
	this->reviewStats.insertAtEnd(rhs);

	// Merge wordsPos
	Node<Word>* rhsPosNode = rhs.wordsPos.getHead();
	while (rhsPosNode) {
		Word rhsWord = rhsPosNode->value;
		Word* word = this->wordsPos.binarySearch(rhsWord);
		if (word) {
			word->count += 1;
		}
		else {
			this->wordsPos.insertSorted(rhsWord);
		}
		rhsPosNode = rhsPosNode->next;
	}

	// Merge wordsNeg
	Node<Word>* rhsNegNode = rhs.wordsNeg.getHead();
	while (rhsNegNode) {
		Word rhsWord = rhsNegNode->value;
		Word* word = this->wordsNeg.binarySearch(rhsWord);
		if (word) {
			word->count += 1;
		}
		else {
			this->wordsNeg.insertSorted(rhsWord);
		}
		rhsNegNode = rhsNegNode->next;
	}

	this->numPos += rhs.numPos;
	this->numNeg += rhs.numNeg;
	this->numWords += rhs.numWords;

	return *this;
}