#include <iostream>
#include <string>

#include "review.h"

Review::Review(std::string& comment, unsigned int rating)
	: comment(comment), rating(rating) { }

Review::Review(std::string& reviewStr)
	: comment(parseComment(reviewStr)), rating(parseRating(reviewStr)) { }


bool Review::operator==(const Review& other) const {
	return comment == other.comment && rating == other.rating;
}

std::ostream& operator<<(std::ostream& os, const Review& review) {
	if (review.comment.length() > 10) {
		os << "Comment: " << review.comment.substr(0, 10) << "..., ";
	} else {
		os << "Comment: " << review.comment << ", ";
	}
	os << "Rating: " << review.rating;
	return os;
}

std::string Review::parseComment(std::string& reviewStr) {
	size_t lastComma = reviewStr.rfind(',');

	if (lastComma == std::string::npos) {
		throw std::runtime_error("Comma not found in the entry [Review.parseComment].");
	}

	return reviewStr.substr(0, lastComma);
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