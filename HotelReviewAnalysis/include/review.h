#pragma once

struct Review {
	int id;
	std::string comment;
	unsigned int rating;

	Review() : id(-1), comment(""), rating(0) { };
	Review(const int id, std::string& comment, unsigned int rating);
	Review(std::string& reviewStr, const int id);

	bool operator==(const Review& other) const;
	bool operator<(const Review& other) const;
	std::string& operator*() const;
	friend std::ostream& operator<<(std::ostream& os, const Review& review);

private: 
	static std::string parseComment(std::string& reviewStr);
	static unsigned int parseRating(std::string& reviewStr);
};