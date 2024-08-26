#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include "linkedlist.h"

namespace csvreader {

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

	LinkedList asLL(const std::string& fileName) {
		LinkedList ll;
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
}

