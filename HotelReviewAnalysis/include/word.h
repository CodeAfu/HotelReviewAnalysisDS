#pragma once

struct Word {
	std::string word;
	unsigned int count;

	// Operator Overloads
	bool operator==(const Word& other) const {
		return word == other.word && count == other.count;
	}

	Word& operator+=(const Word& rhs) {
		word += rhs.word;
		count += rhs.count;
	}

	bool operator<(const Word& other) const {
		return this->count < other.count;
	}

	friend std::ostream& operator<<(std::ostream& os, const Word& wordCount) {
		os << wordCount.word;
		return os;
	}
};