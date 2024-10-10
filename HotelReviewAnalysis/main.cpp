#include <iostream>
#include "sentiment_analysis.h"

#define LOG(x) std::cout << x << std::endl;

constexpr char NEGATIVE_WORDS_FILE[] = "data/negative-words.txt";
constexpr char POSITIVE_WORDS_FILE[] = "data/positive-words.txt";
constexpr char REVIEWS_FILE[] = "data/tripadvisor_hotel_reviews.csv";

int main(int argc, char** argv) {
	std::string revFile;
	std::string posFile;
	std::string negFile;
	
	std::cout << "PATH: " << argv[0] << std::flush;

	switch (argc) {
		case 1:
			LinkedListImpl::run(REVIEWS_FILE, POSITIVE_WORDS_FILE, NEGATIVE_WORDS_FILE);
			break;
		case 4:
			revFile = argv[1];
			posFile = argv[2];
			negFile = argv[3];
			LinkedListImpl::run(revFile, posFile, negFile);
			break;
		default:
			std::cerr << "Usage: " << argv[0] << " [reviews_file] [positive_words_file] [negative_words_file]" << std::endl;
			return 1;
	}
	
	return 0;
}
