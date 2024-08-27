#include <iostream>;
#include <format>;
#include "linkedlist.h"
#include "csv_reader.h"
#include "sentiment_analysis.h"

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

void clearMem(std::string* arr);

int main() {

	/// As String
	// const std::string allReviewsStr = csvreader::asString("data/tripadvisor_hotel_reviews.csv");
	// const std::string positiveWordsSTr = csvreader::asString("data/negative-words.txt");
	// const std::string negativeWordsSTr = csvreader::asString("data/positive-words.txt");


	/// As Linked List
	//LinkedList allReviewsLL = csvreader::asLL(REVIEWS_FILE);
	//LinkedList positiveWordsLL = csvreader::asLL(POSITIVE_WORDS_FILE);
	//LinkedList negativeWordsLL = csvreader::asLL(NEGATIVE_WORDS_FILE);


	/// As Array
	//std::string* negativeWordsArr = nullptr;
	//std::string* positiveWordsArr = nullptr;
	//std::string* allReviewsArr = nullptr;
	//size_t lineCountNW, lineCountPW, lineCountR = 0;

	//csvreader::asArr(negativeWordsArr, lineCountNW, NEGATIVE_WORDS_FILE);
	//csvreader::asArr(positiveWordsArr, lineCountPW, POSITIVE_WORDS_FILE);
	//csvreader::asArr(allReviewsArr, lineCountR, REVIEWS_FILE);

	//clearMem(negativeWordsArr);
	//clearMem(positiveWordsArr);
	//clearMem(allReviewsArr);

	analyzer::run();

	return 0;
}

void clearMem(std::string* arr) {
	if (arr) {
		delete[] arr;
		std::cout << "Memory cleared.\n";
	}
}