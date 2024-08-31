#include <iostream>;
#include <format>;
#include "linkedlist.h"
#include "csv_reader.h"
#include "sentiment_analysis.h"

#define LOG(x) std::cout << x << std::endl;

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

void clearMem(std::string* arr);

int main(int argc, char** argv) {

	/// As String
	// const std::string allReviewsStr = csvreader::asString("data/tripadvisor_hotel_reviews.csv");
	// const std::string positiveWordsSTr = csvreader::asString("data/negative-words.txt");
	// const std::string negativeWordsSTr = csvreader::asString("data/positive-words.txt");


	/// As Linked List
	//LinkedList allReviewsLL = csvreader::asLLString(REVIEWS_FILE);
	//LinkedList positiveWordsLL = csvreader::asLLString(POSITIVE_WORDS_FILE);
	//LinkedList negativeWordsLL = csvreader::asLLString(NEGATIVE_WORDS_FILE);


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

	std::string revFile;
	std::string posFile;
	std::string negFile;

	switch (argc) {
		case 1:
			analyzer::run(REVIEWS_FILE, POSITIVE_WORDS_FILE, NEGATIVE_WORDS_FILE);
			break;
		case 4:
			revFile = argv[1];
			posFile = argv[2];
			negFile = argv[3];
			analyzer::run(revFile, posFile, negFile);
			break;
		default:
			std::cerr << "Usage: " << argv[0] << " [reviews_file] [positive_words_file] [negative_words_file]" << std::endl;
			return 1;
	}
	
	return 0;
}

void clearMem(std::string* arr) {
	if (arr) {
		delete[] arr;
		std::cout << "Memory cleared.\n";
	}
}