#include <iostream>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "cleaner.h"

inline void ltrim(std::string& s);
inline void rtrim(std::string & s);
void processWord(std::string& word);


void cleanWord(std::string& word) {
    processWord(word);
    ltrim(word);
    rtrim(word);
}

inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void processWord(std::string& word) {
	const char excludeChars[] = { ',', '\"', '\'', '.', '/', ':', ';', '!', '?', '*' };
	const size_t n = sizeof(excludeChars) / sizeof(char);

	std::transform(word.begin(), word.end(), word.begin(), ::tolower);

	word.erase(std::remove_if(word.begin(), word.end(), [&](char c) {
		return std::find(excludeChars, excludeChars + n, c) != excludeChars + n;
		}), word.end());
}