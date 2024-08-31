#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <format>
#include <chrono>
#include <ios>

#include "review.h"
#include "linkedlist.h"
#include "csv_reader.h"
#include "sentiment_analysis.h"

#define LOG(x) std::cout << x << std::endl

using Ms = std::chrono::milliseconds;
using Timer = std::chrono::high_resolution_clock;

struct Data {
	LinkedList<Review>& reviews;
	LinkedList<std::string>& positiveWords;
	LinkedList<std::string>& negativeWords;

	Data(LinkedList<Review>& rev, LinkedList<std::string>& pos, LinkedList<std::string>& neg)
		: reviews(rev), positiveWords(pos), negativeWords(neg) { }
};


struct Result {
	LinkedList<std::string> pos;
	LinkedList<std::string> neg;
	unsigned int numReviews = 0;
	unsigned int numWords = 0;
	unsigned int numPos = 0;
	unsigned int numNeg = 0;
	Ms duration;

	void summary() {
		std::cout << std::format("Number of Reviews: {}\n", numReviews);
		std::cout << std::format("Number of Words: {}\n", numWords);
		std::cout << std::format("Number of Positives: {}\n", numPos);
		std::cout << std::format("Number of Negatives: {}\n", numNeg);
		std::cout << std::format("Time elapsed: {}\n", duration);
	}
};


namespace analyzer {
	Result analyze(const Data& data);
	void buildResultBinary(const std::string& word, const Data& data, Result& res);
	void buildResultLinear(const std::string& word, const Data& data, Result& res);
	void processWord(std::string& word);

	void run(const std::string& revFile, const std::string& posFile, const std::string negFile) {
		LinkedList<Review> reviews = csvreader::asLLReview(revFile);
		LinkedList<std::string> positiveWords = csvreader::asLLString(posFile);
		LinkedList<std::string> negativeWords = csvreader::asLLString(negFile);

		/// Debug
		//reviews.display();
		//positiveWords.display();
		//negativeWords.display();

		//std::cin.get();
		std::system("cls");

		Data data(reviews, positiveWords, negativeWords);
		Result res = analyze(data);
		std::system("cls");

		res.summary();
	}

	Result analyze(const Data& data) {
		const int DEBUG_LIMIT = -1; // set to -1 for real use
		int iterations = 0;

		const auto start = Timer::now();

		// Process Reviews
		Result res;
		while (data.reviews.getCurrentNode()) {
			std::system("cls");

			std::cout << "Review: " << data.reviews.getValue().comment << std::endl;
			std::cout << "Rating: " << data.reviews.getValue().rating << std::endl;
			//std::cin.get();

			// Split String and build review
			std::istringstream iss(data.reviews.getValue().comment);
			std::string s;

			// Get each word in comment
			while (std::getline(iss, s, ' ')) {
				processWord(s);
				//buildResultLinear(s, data, res);
				buildResultBinary(s, data, res);
			}

			res.numReviews++;
			std::cout << std::endl;
			data.reviews.next();

			// Limit Iterations for Debug
			iterations++;
			if (iterations == DEBUG_LIMIT) {
				break;
			}

			if (data.reviews.getCurrentNode() == nullptr) {
				break;
			}

			//std::cin.get();
		}

		res.duration = std::chrono::duration_cast<Ms>(Timer::now() - start);
		return res;
	}
	
	void buildResultBinary(const std::string& word, const Data& data, Result& res) {
		// Calc Positive
		const std::string* pcPtr = data.positiveWords.binarySearch(word);

		if (pcPtr != nullptr) {
			res.pos.insertAtEnd(*pcPtr);
			res.numPos++;
			std::cout << *pcPtr << "+";

			// return early if positive word found
			//return;
		}

		// Calc Negative
		const std::string* ncPtr = data.negativeWords.binarySearch(word);
		if (ncPtr != nullptr) {
			res.neg.insertAtEnd(*ncPtr);
			res.numNeg++;
			std::cout << *ncPtr << "-";
			
			// return early if positive word found
			//return;
		}
		res.numWords++;
	}

	void buildResultLinear(const std::string& word, const Data& data, Result& res) {
		// Calc Positive
		while (data.positiveWords.getCurrentNode()) {

			const std::string& s = data.positiveWords.getValue();

			if (s == word) {
				res.pos.insertAtEnd(word);
				res.numPos++;
				std::cout << s << "+";
				//break;
			}

			if (!data.positiveWords.hasNext()) 
				break;

			data.positiveWords.next();
		}

		// Calc Negative
		while (data.negativeWords.getCurrentNode()) {

			const std::string& s = data.negativeWords.getValue();

			if (s == word) {
				res.neg.insertAtEnd(word);
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

	void processWord(std::string& word) {
		const char excludeChars[] = { ',', '\"', '\'', '.', '/', ':', ';', '!', '?' };
		const size_t n = sizeof(excludeChars) / sizeof(char);

		std::transform(word.begin(), word.end(), word.begin(), ::tolower);

		word.erase(std::remove_if(word.begin(), word.end(), [&](char c) {
			return std::find(excludeChars, excludeChars + n, c) != excludeChars + n;
		}), word.end());
	}
}