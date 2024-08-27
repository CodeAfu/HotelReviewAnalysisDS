#include <iostream>
#include <string>

#include "sentiment_analysis.h"
#include "linkedlist.h"
#include "csv_reader.h"

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

namespace analyzer {

	float analyze(LinkedList& reviews, LinkedList& positiveReviews, LinkedList& negativeReviews);

	void run() {
		LinkedList allReviews = csvreader::asLL(REVIEWS_FILE);
		LinkedList positiveWords = csvreader::asLL(POSITIVE_WORDS_FILE);
		LinkedList negativeWords = csvreader::asLL(NEGATIVE_WORDS_FILE);
		float f = analyze(allReviews, positiveWords, negativeWords);
	}

	float analyze(LinkedList& reviews, LinkedList& positiveReviews, LinkedList& negativeReviews) {
		// Skip Header
		reviews.next();

		std::string review = reviews.getValue();
		std::cout << review << std::endl;

		reviews.next();

		std::string review2 = reviews.getValue();
		std::cout << review2 << std::endl;

		return 0;
	}

}