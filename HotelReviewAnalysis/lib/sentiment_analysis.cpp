#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <format>
#include <chrono>
#include <ios>

#include "sentiment_analysis.h"
#include "linkedlist.h"
#include "csv_reader.h"

using Ms = std::chrono::milliseconds;
using Timer = std::chrono::high_resolution_clock;

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

struct Result {
	LinkedList pos;
	LinkedList neg;
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

struct Review {
	const std::string comment;
	const int rating;

	Review(const std::string comment, int rating) 
		: comment(comment), rating(rating) { }
};

struct Data {
	LinkedList& reviews;
	LinkedList& positiveWords;
	LinkedList& negativeWords;

	Data(LinkedList& rev, LinkedList& pos, LinkedList& neg)
		: reviews(rev), positiveWords(pos), negativeWords(neg) 
	{ }
};

namespace analyzer {

	Result analyze(const Data& data);
	void processWord(std::string& word);
	void buildResult(const std::string& word, const Data& data, Result& res);
	Review buildReview(const std::string& review);

	void run() {
		LinkedList reviews = csvreader::asLL(REVIEWS_FILE);
		LinkedList positiveWords = csvreader::asLL(POSITIVE_WORDS_FILE);
		LinkedList negativeWords = csvreader::asLL(NEGATIVE_WORDS_FILE);

		// Pause and Clear console
		//std::cin.get();
		std::system("cls");

		Data data(reviews, positiveWords, negativeWords);
		Result res = analyze(data);
		std::system("cls");

		res.summary();
	}

	Result analyze(const Data& data) {
		const auto start = Timer::now();
		const int DEBUG_LIMIT = 4; // set to -1 for real use

		int iterations = 0;

		// Skip Header Contents
		data.reviews.next();

		// [DEBUG] Build Review Struct
		const std::string& reviewStr = data.reviews.getValue();
		Review review = buildReview(reviewStr);
		std::cout << review.comment << std::endl;
		std::cout << "Rating: " << review.rating << std::endl;
		std::cin.get();

		// Init
		Result res;

#if 0
		/// Wrap these in while loop
		while (data.reviews.getCurrentNode()) {
			std::system("cls");

			std::string review = data.reviews.getValue();
			std::cout << "Review:" << std::endl;
			std::cout << review << std::endl;

			// Split String and build review
			std::istringstream iss(review);
			std::string s;
			while (std::getline(iss, s, ' ')) {
				processWord(s);
				buildResult(s, data, res);
			}
			res.numReviews++;
			std::cout << std::endl;
			data.reviews.next();


			/// Remove After Testing
			iterations++;
			if (iterations == DEBUG_LIMIT) {
				break;
			}
		}
		/// endloop
#endif

		// Calculate Time
		const auto duration = std::chrono::duration_cast<Ms>(Timer::now() - start);
		res.duration = duration;
		return res;
	}

	Review buildReview(const std::string& review) {
		size_t lastComma = review.rfind(',');

		std::string rev;
		int rating;

		if (lastComma != std::string::npos) {
			rev = review.substr(0, lastComma);
			std::string strRating = review.substr(lastComma + 1);

			// Trim
			strRating.erase(0, strRating.find_first_not_of(" \t\n\r\f\v"));
			strRating.erase(strRating.find_last_not_of(" \t\n\r\f\v") + 1);

			rating = std::stoi(strRating);
		} else {
			throw std::runtime_error("Comma not found in the entry [buildReview]");
		}

		return Review(rev, rating);
	}

	void buildResult(const std::string& word, const Data& data, Result& res) {

		// Calc Positive
		while (data.positiveWords.getCurrentNode()) {

			const std::string& s = data.positiveWords.getValue();

			if (s == word) {
				res.pos.insertAtEnd(word);
				res.numPos++;
				std::cout << s << ":";
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
				std::cout << s << ":";
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