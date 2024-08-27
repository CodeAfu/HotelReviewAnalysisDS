#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ios>

#include "sentiment_analysis.h"
#include "linkedlist.h"
#include "csv_reader.h"

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

struct Result {
	LinkedList pos;
	LinkedList neg;
	unsigned int numReviews;
	unsigned int numPos;
	unsigned int numNeg;
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
	void matchWords(const std::string&, const Data& data, Result& res);

	void run() {
		LinkedList reviews = csvreader::asLL(REVIEWS_FILE);
		LinkedList positiveWords = csvreader::asLL(POSITIVE_WORDS_FILE);
		LinkedList negativeWords = csvreader::asLL(NEGATIVE_WORDS_FILE);

		// Pause and Clear console
		//std::cin.get();
		std::system("cls");

		Data data(reviews, positiveWords, negativeWords);
		Result f = analyze(data);
	}

	Result analyze(const Data& data) {
		// Skip Header Contents
		data.reviews.next();

		// Init
		Result res;

		/// Wrap these in while loop

		// Split String
		std::string review = data.reviews.getValue();
		std::istringstream iss(review);
		std::string s;
		while (std::getline(iss, s, ' ')) {
			processWord(s);
			matchWords(s, data, res);
			std::cout << s << std::endl;
		}

		data.reviews.next();

		/// endloop

		return res;
	}

	void matchWords(const std::string&, const Data& data, Result& res) {
		
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