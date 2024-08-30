#pragma once

struct Review {
	std::string comment;
	unsigned int rating;

	Review() : comment(""), rating(0) { };
	Review(std::string& comment, unsigned int rating);
	Review(std::string& reviewStr);

	bool operator==(const Review& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Review& review);

private: 
	static std::string parseComment(std::string& reviewStr);
	static unsigned int parseRating(std::string& reviewStr);
};