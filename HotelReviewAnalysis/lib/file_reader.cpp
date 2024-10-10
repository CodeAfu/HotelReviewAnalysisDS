#include <iostream>
#include <fstream>
#include <string>
#include <format>

#include "linkedlist.h"
#include "file_reader.h"

namespace filereader {

	LinkedList<std::string> asLLString(const std::string& fileName) {
		LinkedList<std::string> ll;
		std::string line;
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
			throw std::runtime_error("Could not read file: " + fileName);
			std::cerr << "Unable to open file.\n";
			return ll;
		}

		while (getline(myFile, line)) {
			ll.insertAtEnd(line);
		}

		//std::cout << std::format("File Read Succesully: {}\n", fileName);
		std::cout << "File Read Succesully: " << fileName << std::endl;
		myFile.close();

		return ll;
	}

	LinkedList<Review> asLLReview(const std::string& fileName) {
		LinkedList<Review> ll;
		std::string line;
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
			throw std::runtime_error("Could not read file: " + fileName);
			std::cerr << "Unable to open file.\n";
			return ll;
		}

		// Skip header
		std::string header;
		std::getline(myFile, header);

		int idx = 1;
		while (getline(myFile, line)) {
			Review rev(line, idx);
			ll.insertAtEnd(rev);
			idx++;
		}
		//std::cout << std::format("File Read Succesully: {}\n", fileName);
		std::cout << "File Read Succesully: " << fileName << std::endl;
		myFile.close();

		return ll;
	}
}

