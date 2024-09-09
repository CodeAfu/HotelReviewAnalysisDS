#pragma once

struct Word {
	std::string word;
	unsigned int count = 0;

	Word() { }
	Word(std::string word) : word(word) { }
	Word(std::string word, unsigned int count) : word(word), count(count) { }

	// Operator Overloads
	bool operator==(const Word& other) const {
		return word == other.word;
	}

	bool operator==(const std::string& other) const {
		return word == other;
	}

	Word& operator+=(const Word& rhs) {
		this->word += rhs.word;
		this->count += rhs.count;
		return *this;
	}

	bool operator<(const Word& other) const {
		return this->word < other.word;
	}

	friend std::ostream& operator<<(std::ostream& os, const Word& wordCount) {
		os << wordCount.word << " (" << wordCount.count << ")";
		return os;
	}
};