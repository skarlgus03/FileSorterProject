#pragma once

#include <string>
#include "Block.h"
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>

// 블럭 유틸리티 클래스 입니다.



// Enum FilterType 을 문자열로 변환 
inline std::string filterTypeToString(FilterType type) {
	switch (type)
	{
	case FilterType::EXTENSION: return "EXTENSION";
	case FilterType::KEYWORD: return "KEYWORD";
	case FilterType::DATE: return "DATE";
	case FilterType::EXCEPTION: return "EXCEPTION";
	case FilterType::SIZE: return "SIZE";
	default: return "UNKNOWN";
	}
}

// 문자열을 Enum FilterType 으로 변환
inline FilterType stringToFilterType(const std::string& str) {
	if (str == "EXTENSION") return FilterType::EXTENSION;
	if (str == "KEYWORD") return FilterType::KEYWORD;
	if (str == "DATE") return FilterType::DATE;
	if (str == "EXCEPTION") return FilterType::EXCEPTION;
	if (str == "SIZE") return FilterType::SIZE;

	throw std::invalid_argument("Unknown FilterType: " + str);
}

// 문자 하나를 기준으로 앞, 뒤 분해하는것
inline std::vector<std::string> split(const std::string& str, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delim)) {
		result.push_back(token);
	}

	if (str.back() == delim) result.push_back("");

	return result;
}


inline std::string trim(const std::string& s) {
	auto start = std::find_if_not(s.begin(), s.end(), ::isspace);
	auto end = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();

	if (start >= end) return "";

	return std::string(start, end);
}