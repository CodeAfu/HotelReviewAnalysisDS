#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <functional>
#include <ios>
#include <regex>

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

enum Selection {
	binary, linear
};

struct Data {
	LinkedList<Review>& reviews;
	LinkedList<std::string>& positiveWords;
	LinkedList<std::string>& negativeWords;

	Data(LinkedList<Review>& rev, LinkedList<std::string>& pos, LinkedList<std::string>& neg)
		: reviews(rev), positiveWords(pos), negativeWords(neg) { }
};

namespace LinkedListImpl {

	Result analyze(const Data& data);

	std::function<void(const std::string, const Data&, Result&, ReviewStats&)> getAlgorithm();

	void buildResultBinary(const std::string& word, const Data& data, Result& res, ReviewStats& stats);
	void buildResultLinear(const std::string& word, const Data& data, Result& res, ReviewStats& stats);

	void menuLoop(Result& res, bool& runFlag);
	void displayPositiveWords(Result& res);
	void displayNegativeWords(Result& res);
	void iterateStats(Result& res);
	void sentimentAnalysis(Result& res);
	void sortWordsByAlpabet(Result& res);
	void sortWordsByCount(Result& res, bool& sortedByCount);

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

		/// Menu
		bool runFlag = true;
		std::cout << std::endl;
		menuLoop(res, runFlag);

		/// Rerun Menu
		while (runFlag) {
			res = analyze(data);
			std::system("cls");
			res.log();
			std::cout << std::endl;
			menuLoop(res, runFlag);
		}
	}

	Result analyze(const Data& data) {
		const int DEBUG_LIMIT = 300; // set to -1 for real use
		int iterations = 0;
		Result res;

		const std::function<void(const std::string, const Data&, Result&, ReviewStats&)> searchAlgorithm = getAlgorithm();
		
		/// Process Reviews
		const auto start = Timer::now();
		
		while (true) {
			const auto reviewTimer = Timer::now();

			ReviewStats stats;
			std::cout << "\rReview #" << iterations + 1 << std::flush;

			/// Split string and build review
			std::istringstream iss(data.reviews.getValue().comment);
			std::string s;

			/// Get each word in comment
			while (std::getline(iss, s, ' ')) {
				processWord(s);
				searchAlgorithm(s, data, res, stats);
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

	std::function<void(const std::string, const Data&, Result&, ReviewStats&)> getAlgorithm() {
		Selection selection;

		std::string menus[] = {
			"1 - Binary Search",
			"2 - Linear Search"
		};
		const uint16_t size = sizeof(menus) / sizeof(menus[0]);

		while (true) {
			std::string s;
			for (int i = 0; i < size; i++) {
				std::cout << menus[i] << std::endl;
			}

			std::cout << ">> ";
			std::getline(std::cin, s);

			if (!isNumber(s)) {
				system("cls");
				std::cout << "Please enter a number.\n";
				continue;
			}

			const uint16_t num = std::stoi(s);

			if (num < 1 || num > size) {
				system("cls");
				std::cout << "Please select a valid menu.\n";
				continue;
			}

			/// Menus
			if (num == 1) {
				selection = Selection::binary;
				break;
			}
			else if (num == 2) {
				selection = Selection::linear;
				break;
			}
		}

		/// Select search algorithm
		std::function<void(const std::string, const Data&, Result&, ReviewStats&)> searchAlgorithm;

		if (selection == Selection::binary) {
			system("cls");
			std::cout << "Running Binary Search...\n";
			searchAlgorithm = buildResultBinary;
		}
		else if (selection == Selection::linear) {
			system("cls");
			std::cout << "Running Linear Search...\n";
			searchAlgorithm = buildResultLinear;
		}

		return searchAlgorithm;
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

	void buildResultLinear(const std::string& word, const Data& data, Result& res, ReviewStats& stats) {
		stats.review = data.reviews.getValue();
		stats.numWords++;

		/// Calculate Positive
		// if check for 'Data' struct: builds final result
		const std::string* pwPtr = data.positiveWords.linearSearch(word);
		if (pwPtr != nullptr) {
			Word posWordStruct(*pwPtr);
			// if check for 'ReviewStats' struct: builds individual review stats
			Word* statsWordPtr = stats.wordsPos.linearSearch(posWordStruct);
			if (statsWordPtr == nullptr) {
				posWordStruct.count = posWordStruct.count + 1;
				stats.wordsPos.insertSorted(posWordStruct);
			}
			else {
				statsWordPtr->count = statsWordPtr->count + 1;
			}

			stats.numPos++;
			return;
		}

		/// Calculate Negative
		// if check for 'Data' struct: builds final result
		const std::string* nwPtr = data.negativeWords.linearSearch(word);
		if (nwPtr != nullptr) {
			Word negWordStruct(*nwPtr);
			// if check for 'ReviewStats' struct: builds individual review stats
			Word* statsWordPtr = stats.wordsNeg.linearSearch(negWordStruct);
			if (statsWordPtr == nullptr) {
				negWordStruct.count = negWordStruct.count + 1;
				stats.wordsNeg.insertSorted(negWordStruct);
			}
			else {
				statsWordPtr->count = statsWordPtr->count + 1;
			}

			stats.numNeg++;
		}
	}


	void menuLoop(Result& res, bool& runFlag) {
		runFlag = false; // For menu 6
		bool sortedByCount = false;

		std::string menus[] = {
			"1 - Display Positive Words",
			"2 - Display Negative Words",
			"3 - Generate Sentiment Analysis",
			"4 - Sort Words by Count",
			"5 - Print Summary of Search",
			"6 - Change Search Algorithm",
			"0 - Exit Application"
		};
		const size_t size = sizeof(menus) / sizeof(menus[0]);

		while (true) {
			std::string s;

			for (int i = 0; i < size; i++) {
				std::cout << menus[i] << std::endl;
			}

			std::cout << ">> ";
			std::cin >> s;

			if (s == "cls") {
				system("cls");
				continue;
			}

			if (!isNumber(s)) {
				std::cout << "Please enter a valid input.\n";
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
				sentimentAnalysis(res);
				break;

			case 4:
				sortWordsByCount(res, sortedByCount);
				break;

			case 5:
				system("cls");
				res.log();
				std::cout << std::endl;
				break;

			case 6:
				runFlag = true;
				return;

			case 0:
				return;
			}
		}
	}


	/// Menus
	void handleDisplay(LinkedList<Word>& words) {
		system("cls");
		words.display();
		std::cout << "\nPress Enter to continue...\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
		std::cin.get();
		system("cls");
	}

	void displayPositiveWords(Result& res) {
		handleDisplay(res.wordsPos);
	}

	void displayNegativeWords(Result& res) {
		handleDisplay(res.wordsNeg);
	}

	void iterateStats(Result& res) {
		res.reviewStats.reset();
		while (res.reviewStats.hasNext()) {
			system("cls");
			
			std::string s;
			res.reviewStats.getValue().log();

			std::cout << "Press (Q) to exit loop.\n";
			std::cout << "Press Enter to continue...\n";
			std::getline(std::cin, s);

			if (s.size() == 1 && tolower(s[0]) == 'q') {
				res.reviewStats.reset();
				system("cls");
				return;
			}
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

	void sortWordsByAlpabet(Result& res) {
		/// TODO: POSSIBLE UNNECESSARY NULL CHECK
		if (&res.wordsPos != nullptr) {
			res.wordsPos.sort();
		} else {
			std::cout << "Positive Words List is null.\n";
		}

		if (&res.wordsNeg != nullptr) {
			res.wordsNeg.sort();
		} else {
			std::cout << "Negative Words List is null.\n";
		}
	}

	void sortWordsByCount(Result& res, bool& sortedByCount) {
		system("cls");
		bool isBubbleSort;
		
		std::string menus[] = {
			"1 - Bubble Sort",
			"2 - Quick Sort",
			"0 - Back"
		};
		const size_t size = sizeof(menus) / sizeof(menus[0]);

		/// Loop for menu
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

			if (val == 0) {
				system("cls");
				return;
			}

			/// Assert Sorting Algorithm
			if (val == 1) {
				isBubbleSort = true;
				break;
			} else if (val == 2) {
				isBubbleSort = false;
				break;
			}
		}

		/// Unsort if already sorted
		if (sortedByCount) {
			const auto start = Timer::now();
			std::cout << "Words are already sorted by Word Count.\n";
			std::cout << "Sorting by Alphabet...\n";

			sortWordsByAlpabet(res);
			sortedByCount = false; // Not needed but stays true to the context.

			const auto duration = std::chrono::duration_cast<Mu>(Timer::now() - start);
			std::cout << "Sorting Completed. (" << duration << ").\n";
		}

		/// Sorting
		LinkedList<Word>* wordsPosPtr = &res.wordsPos;
		LinkedList<Word>* wordsNegPtr = &res.wordsNeg;

		if (wordsPosPtr == nullptr || wordsNegPtr == nullptr) {
			throw std::runtime_error("[ERROR] Attempting to sort a Word struct that points to nullptr. (sortWordsByCount)");
		}
		
		const auto start = Timer::now();
		system("cls");
		std::cout << "Sorting...\n";

		if (isBubbleSort) {
			wordsPosPtr->bubbleSort();
			wordsNegPtr->bubbleSort();
			sortedByCount = true;
		} else {
			wordsPosPtr->quickSort();
			wordsNegPtr->quickSort();
			sortedByCount = true;
		}

		const auto duration = std::chrono::duration_cast<Mu>(Timer::now() - start);

		std::cout << "Operation completed in " << duration << std::endl;
		std::cout << "\nPress any key to continue...";

		std::cin.get();
		system("cls");
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
		system("cls");

		while (true) {
			std::string s;
			std::cout << "Which Review Number do you want to view? (Q to exit): ";

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

			if (val > stats.getSize()) {
				std::cout << "Index out of range [" << val << ":" << stats.getSize() << "].\n";
				continue;
			}

			for (int i = 1; i < val; i++) {
				if (!stats.hasNext()) {
					throw std::out_of_range("[ERROR] LinkedList next pointer points to a nullptr.");
				}
				stats.next();
			}

			system("cls");

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