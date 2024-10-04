#include <iostream>
#include <fstream>
#include <string>
#include <format>

#include "linkedlist.h"
#include "file_reader.h"

namespace filereader {

	std::string asString(const std::string& fileName) {
		std::string contents = "";
		std::string line;
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
			std::cerr << "Unable to open file.\n";
			return contents;
		}

		while (getline(myFile, line)) {
			contents += line + "\n";
		}

		std::cout << std::format("File Read Succesully: {}\n", fileName);
		myFile.close();

		return contents;
	}

	void asArr(std::string*& arr, size_t& count, const std::string& fileName) {
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
			std::cerr << "Unable to open file.\n";
		}

		size_t lineCount = 0;
		std::string line;
		while (getline(myFile, line)) {
			lineCount++;
		}

		arr = new std::string[lineCount];
		std::cout << std::format("[WARNING] Allocating Dynamic Array, Remember to clear heap memory.\n");
		count = lineCount;

		myFile.clear();
		myFile.seekg(std::ios::beg);

		size_t index = 0;
		while (std::getline(myFile, line)) {
			arr[index++] = line;
		}
		std::cout << std::format("File Read Succesfully: {}\n", fileName);

		myFile.close();
	}

	LinkedList<std::string> asLLString(const std::string& fileName) {
		LinkedList<std::string> ll;
		std::string line;
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
			std::cerr << "Unable to open file.\n";
			return ll;
		}

		while (getline(myFile, line)) {
			ll.insertAtEnd(line);
		}

		std::cout << std::format("File Read Succesully: {}\n", fileName);
		myFile.close();

		return ll;
	}

	LinkedList<Review> asLLReview(const std::string& fileName) {
		LinkedList<Review> ll;
		std::string line;
		std::ifstream myFile(fileName);

		if (!myFile.is_open()) {
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

		std::cout << std::format("File Read Succesully: {}\n", fileName);
		myFile.close();

		return ll;
	}
}

