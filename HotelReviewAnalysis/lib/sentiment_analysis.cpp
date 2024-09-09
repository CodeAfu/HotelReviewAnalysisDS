#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <format>
#include <chrono>
#include <functional>
#include <ios>

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

namespace LinkedListImpl {

	Result analyze(const Data& data);
	void buildResultBinary(const std::string& word, const Data& data, Result& res, ReviewStats& stats);
	void buildResultFibonacci(const std::string word, const Data& data, Result& res, ReviewStats& stats);

	void menuLoop(Result& res);
	void displayPositiveWords(Result& res);
	void displayNegativeWords(Result& res);
	void iterateStats(Result& res);
	void sentimentAnalysis(Result& res);

	void printAllSentimentScores(LinkedList<ReviewStats>& stats);
	void printIterateSentimentScores(LinkedList<ReviewStats>& stats);
	void printSelectedSentimentScore(LinkedList<ReviewStats>& stats);

	bool isNumber(const std::string& s);
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
		
		/// Prints final result details
		std::system("cls");
		res.log();

		/// Prompts
		menuLoop(res);
		
		/// Debug
	}

	Result analyze(const Data& data) {
		const int DEBUG_LIMIT = 100; // set to -1 for real use
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

			/// Calculate Sentiment Score for each review
			stats.calculateSentimentScore();
			
			/// Append stats to result
			res += stats;
			res.numReviews++;

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

		data.reviews.reset(); // Reset current pointer to head
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


	void buildResultFibonacci(const std::string word, const Data& data, Result& res, ReviewStats& stats) {

	}


	void menuLoop(Result& res) {
		const std::string menus[] = {
			"1 - Display Positive Words",
			"2 - Display Negative Words",
			"3 - Iterate through ReviewStats",
			"4 - Generate Sentiment Analysis",
			"0 - Exit Application"
		};
		const size_t size = sizeof(menus) / sizeof(menus[0]);

		while (true) {
			std::string s;

			std::cout << std::endl;
			for (int i = 0; i < size; i++) {
				std::cout << menus[i] << std::endl;
			}

			std::cout << ">> ";
			std::cin >> s;

			if (!isNumber(s)) {
				std::cout << "Please enter a number.\n";
				continue;
			}
			const uint16_t val = std::stoi(s);

			switch (val) {
			case 1:
				displayPositiveWords(res);
				break;

			case 2:
				displayNegativeWords(res);
				break;

			case 3:
				iterateStats(res);
				break;

			case 4:
				sentimentAnalysis(res);
				break;
			case 0:
				return;
			}
		}
	}


	/// Menus
	void displayPositiveWords(Result& res) {
		res.wordsPos.display();
	}

	void displayNegativeWords(Result& res) {
		res.wordsNeg.display();
	}

	void iterateStats(Result& res) {
		res.reviewStats.reset();
		while (res.reviewStats.hasNext()) {
			res.reviewStats.getValue().log();
			std::cin.get();
			res.reviewStats.next();
		}
		res.reviewStats.reset();
	}

	void sentimentAnalysis(Result& res) {
		system("cls");
		const std::string menus[] {
			"1 - Get Sentiment Scores for all reviews.",
			"2 - Iterate Through Each Review.",
			"3 - Select Review by Review Number.",
			"0 - Back"
		};

		const size_t size = sizeof(menus) / sizeof(menus[0]);

		while (true) {
			std::string s;

			for (int i = 0; i < size; i++) {
				std::cout << menus[i] << std::endl;
			}
			std::cout << ">> ";
			std::cin >> s;

			// Remove newline from input buffer
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (!isNumber(s)) {
				std::cout << "Please enter a number.\n";
				continue;
			}

			const uint16_t val = stoi(s);

			switch (val) {
			case 1:
				printAllSentimentScores(res.reviewStats);
				break;

			case 2:
				printIterateSentimentScores(res.reviewStats);
				break;

			case 3:
				printSelectedSentimentScore(res.reviewStats);
				break;

			case 0:
				system("cls");
				return;

			default:
				std::cout << "Value " << val << " is out of range (1-" << size << ").\n";
				break;
			}
		}
	}


	/// Linked List Print Functions
	void printAllSentimentScores(LinkedList<ReviewStats>& stats) {
		while (stats.hasNext()) {
			stats.getValue().log();
			stats.next();
		}
		stats.reset();
	}

	void printIterateSentimentScores(LinkedList<ReviewStats>& stats) {
		while (stats.hasNext()) {
			std::string s;

			system("cls");
			stats.getValue().log();

			std::cout << std::endl;
			std::cout << "Press (Q) to exit loop.\n";
			std::cout << "Press Enter to continue...\n";
			std::getline(std::cin, s);

			if (s.size() == 1 && tolower(s[0]) == 'q') {
				stats.reset();
				system("cls");
				std::cout << "Exiting loop.\n";
				return;
			}

			stats.next();
			
			if (s.empty()) {
				continue;
			}
		}
		system("cls");
		stats.reset();
	}

	void printSelectedSentimentScore(LinkedList<ReviewStats>& stats) {
		while (true) {
			std::string s;
			std::cout << "Which Review Number do you want to view?: ";

			std::cin >> s;

			if (s.size() == 1 && tolower(s[0]) == 'q') {
				system("cls");
				return;
			}

			if (!isNumber(s)) {
				std::cout << "Please enter a number.\n";
				continue;
			}

			const size_t val = std::stoi(s);

			for (int i = 1; i < val; i++) {
				if (!stats.hasNext()) {
					std::out_of_range("[ERROR] LinkedList next pointer points to a nullptr.");
				}
				stats.next();
			}
			stats.getValue().log();
			std::cout << std::endl;
			stats.reset();
		}
	}


	/// Helper Functions
	bool isNumber(const std::string& s) {
		return !s.empty() && std::find_if(s.begin(),
			s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
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