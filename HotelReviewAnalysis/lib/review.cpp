#include <iostream>
#include <string>
#include <algorithm>

#include "review.h"

Review::Review(const int id, std::string& comment, unsigned int rating)
	: id(id), comment(comment), rating(rating) { }

Review::Review(std::string& reviewStr, const int id)
	: id(id), comment(parseComment(reviewStr)), rating(parseRating(reviewStr)) { }


bool Review::operator==(const Review& other) const {
	return id == other.id && comment == other.comment && rating == other.rating;
}

bool Review::operator<(const Review& other) const {
	return this->comment < other.comment;
}

std::ostream& operator<<(std::ostream& os, const Review& review) {
	const int LIMIT = 50;
	if (review.comment.length() > LIMIT) {
		os << "Comment: " << review.comment.substr(0, LIMIT) << "..., ";
	} else {
		os << "Comment: " << review.comment << ", ";
	}
	os << "Rating: " << review.rating;
	return os;
}

std::string& Review::operator*() const {
	return const_cast<std::string&>(comment);
}

std::string Review::parseComment(std::string& reviewStr) {
	size_t lastComma = reviewStr.rfind(',');

	if (lastComma == std::string::npos) {
		throw std::runtime_error("Comma not found in the entry [Review.parseComment].");
	}

	return reviewStr.substr(1, lastComma - 1);
}

unsigned int Review::parseRating(std::string& reviewStr) {
	size_t lastComma = reviewStr.rfind(',');

	if (lastComma == std::string::npos) {
		throw std::runtime_error("Comma not found in the entry [Review.parseRating].");
	}

	std::string strRating = reviewStr.substr(lastComma + 1);

	// Trim whitespace
	strRating.erase(0, strRating.find_first_not_of(" \t\n\r\f\v"));
	strRating.erase(strRating.find_last_not_of(" \t\n\r\f\v") + 1);

	return static_cast<unsigned int>(std::stoi(strRating));
}