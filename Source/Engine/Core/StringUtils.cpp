#include "StringUtils.h"
#include <sstream>

std::string ToUpper(std::string input) {
	std::string upperString;

	for (char c : input) {
		upperString += std::toupper(c);
	}

	return upperString;
}

std::string ToLower(std::string input) {
	std::string lowerString;

	for (char c : input) {
		lowerString += std::tolower(c);
	}

	return lowerString;
}

bool IsEqualIgnoreCase(std::string input1, std::string input2) {
	const char* one = input1.c_str();
	const char* two = input2.c_str();

	if (std::strcmp(one, two)) {
		return false;
	}

	return true;
}

std::string CreateUnique(std::string input) {
	std::stringstream ss;
	ss << intToAppend;
	std::string string = ss.str();

	std::string newString = input.append(" " + string);

	intToAppend++;

	return newString;
}
