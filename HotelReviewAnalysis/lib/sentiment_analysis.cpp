#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <format>
#include <chrono>
#include <ios>
#include <thread>

#include "review.h"
#include "linkedlist.h"
#include "csv_reader.h"
#include "review_stats.h"
#include "word.h"
#include "result.h"
#include "sentiment_analysis.h"

#define LOG(x) std::cout << x << std::endl

using Ms = std::chrono::milliseconds;
using Mu = std::chrono::microseconds;
using Timer = std::chrono::high_resolution_clock;


struct Data {
	LinkedList<Review>& reviews;
	LinkedList<std::string>& positiveWords;
	LinkedList<std::string>& negativeWords;

	Data(LinkedList<Review>& rev, LinkedList<std::string>& pos, LinkedList<std::string>& neg)
		: reviews(rev), positiveWords(pos), negativeWords(neg) { }
};

#if 0
struct Result {
	LinkedList<ReviewStats> reviewStats; // Information about each review, iterate through each to get details
	LinkedList<Word> wordsPos;
	LinkedList<Word> wordsNeg;
	unsigned int numReviews = 0;
	unsigned int numWords = 0;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	Ms duration;

	void log();

	Result& operator+=(const ReviewStats rhs) {
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
};
#endif

namespace LinkedListImpl {
	Result analyze(const Data& data);
	void buildResultBinary(const std::string& word, const Data& data, Result& res, ReviewStats& stats);
	void buildResultLinear(const std::string& word, const Data& data, Result& res);
	//void statsProcessor(ReviewStats& stats, Result& res);
	void processWord(std::string& word);

	void run(const std::string& revFile, const std::string& posFile, const std::string negFile) {

		/// Load File Data
		LinkedList<Review> reviews = csvreader::asLLReview(revFile);
		LinkedList<std::string> positiveWords = csvreader::asLLString(posFile);
		LinkedList<std::string> negativeWords = csvreader::asLLString(negFile);
		Data data(reviews, positiveWords, negativeWords);

		//std::cin.get();
		std::system("cls");
		
		/// Main Analysis runs here
		Result res = analyze(data);
		
		std::system("cls");
		res.log();

		//res.wordsPos.display();
		//res.wordsNeg.display();
		//res.reviewStats.getHead()->next->next->value.wordsPos.display();
	}

	Result analyze(const Data& data) {
		const int DEBUG_LIMIT = -1; // set to -1 for real use
		int iterations = 0;
		
		const auto start = Timer::now();

		/// Process Reviews
		Result res;
		while (true) {
			const auto reviewTimer = Timer::now();

			ReviewStats stats;
			//std::system("cls");
			std::cout << "\rReview #" << iterations + 1 << std::flush;

			//std::cout << "Review: " << data.reviews.getValue().comment << std::endl;
			//std::cout << "Rating: " << data.reviews.getValue().rating << std::endl;
			//std::cin.get();

			/// Split string and build review
			std::istringstream iss(data.reviews.getValue().comment);
			std::string s;

			/// Get each word in comment
			while (std::getline(iss, s, ' ')) {
				processWord(s);
				//buildResultLinear(s, data, res);
				buildResultBinary(s, data, res, stats);
			}
			
			/// Append stats to result
			res += stats;
			res.numReviews++;

			/// Calculate Sentiment Score for each review
			stats.calculateSentimentScore();

			/// Limit iterations for debug
			iterations++;
			if (iterations == DEBUG_LIMIT) {
				break;
			}

			//std::cin.get();
			
			/// Break Loop
			if (!data.reviews.hasNext()) {
				break;
			}
			data.reviews.next();

			stats.timeInMillis = std::chrono::duration_cast<Mu>(Timer::now() - start);
		}

		res.duration = std::chrono::duration_cast<Ms>(Timer::now() - start);
		return res;
	}
	
	void buildResultBinary(const std::string& word, const Data& data, Result& res, ReviewStats& stats) {
		stats.review = data.reviews.getValue();
		stats.numWords++;

		/// Calculate Positive
		// if check for 'Data' struct: builds final result
		const std::string* pwPtr = data.positiveWords.binarySearch(word);
		if (pwPtr != nullptr) {
			Word posWordStruct(*pwPtr);
			// if check for 'ReviewStats' struct: builds individual review stats
			Word* statsWordPtr = stats.wordsPos.binarySearch(posWordStruct);
			if (statsWordPtr == nullptr) {
				posWordStruct.count = posWordStruct.count + 1;
				stats.wordsPos.insertSorted(posWordStruct);
			}
			else {
				statsWordPtr->count = statsWordPtr->count + 1;
			}

			//std::cout << word << "+";

			stats.numPos++;
			return;
		}

		/// Calculate Negative
		// if check for 'Data' struct: builds final result
		const std::string* nwPtr = data.negativeWords.binarySearch(word);
		if (nwPtr != nullptr) {
			Word negWordStruct(*nwPtr);
			// if check for 'ReviewStats' struct: builds individual review stats
			Word* statsWordPtr = stats.wordsNeg.binarySearch(negWordStruct);
			if (statsWordPtr == nullptr) {
				negWordStruct.count = negWordStruct.count + 1;
				stats.wordsNeg.insertSorted(negWordStruct);
			} else {
				statsWordPtr->count = statsWordPtr->count + 1;
			}

			//std::cout << word << "-";

			stats.numNeg++;
		}
	}

	void processWord(std::string& word) {
		const char excludeChars[] = { ',', '\"', '\'', '.', '/', ':', ';', '!', '?' };
		const size_t n = sizeof(excludeChars) / sizeof(char);

		std::transform(word.begin(), word.end(), word.begin(), ::tolower);

		word.erase(std::remove_if(word.begin(), word.end(), [&](char c) {
			return std::find(excludeChars, excludeChars + n, c) != excludeChars + n;
			}), word.end());
	}

/// This block need refactoring
#if 0
	void buildResultLinear(const std::string& word, const Data& data, Result& res) {
		/// Calc Positive
		while (data.positiveWords.getCurrentNode()) {

			const std::string& s = data.positiveWords.getValue();

			if (s == word) {
				Word word();
				res.wordsPos.insertAtEnd(word);
				res.numPos++;
				std::cout << s << "+";
				//break;
			}

			if (!data.positiveWords.hasNext()) 
				break;

			data.positiveWords.next();
		}

		/// Calc Negative
		while (data.negativeWords.getCurrentNode()) {

			const std::string& s = data.negativeWords.getValue();

			if (s == word) {
				res.wordsNeg.insertAtEnd(word);
				res.numNeg++;
				std::cout << s << "-";
				//break;
			}

			if (!data.negativeWords.hasNext())
				break;

			data.negativeWords.next();
		}

		// Reset Position of LinkedList Current Pointer
		data.positiveWords.reset();
		data.negativeWords.reset();

		res.numWords++;
	}
#endif
}