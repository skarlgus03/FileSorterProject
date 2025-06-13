#pragma once

#include <string>
#include "Block.h"
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <regex>


// 블럭 유틸리티 클래스 입니다.



// Enum FilterType 을 문자열로 변환 
inline std::string filterTypeToString(FilterType type) {
	switch (type)
	{
	case FilterType::EXTENSION: return "EXTENSION";
	case FilterType::KEYWORD: return "KEYWORD";
	case FilterType::DATE: return "DATE";
	case FilterType::SIZE: return "SIZE";
	default: return "UNKNOWN";
	}
}

// 문자열을 Enum FilterType 으로 변환
inline FilterType stringToFilterType(const std::string& str) {
	if (str == "EXTENSION") return FilterType::EXTENSION;
	if (str == "KEYWORD") return FilterType::KEYWORD;
	if (str == "DATE") return FilterType::DATE;
	if (str == "SIZE") return FilterType::SIZE;

	throw std::invalid_argument("Unknown FilterType: " + str);
}

// 문자열 <-> SizeUnit
inline std::string sizeUnitToString(SizeUnit unit) {
	switch (unit)
	{
	case SizeUnit::B: return "Byte";
	case SizeUnit::KB: return "KB";
	case SizeUnit::MB: return "MB";
	case SizeUnit::GB: return "GB";
	default: return "UNKNOWN";
	}
}
inline SizeUnit stringToSizeUnit(const std::string& str) {
	if (str == "B") return SizeUnit::B;
	if (str == "KB") return SizeUnit::KB;
	if (str == "MB") return SizeUnit::MB;
	if (str == "GB") return SizeUnit::GB;

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

// 문자열 공백 제거
inline std::string trim(const std::string& s) {
	auto start = std::find_if_not(s.begin(), s.end(), ::isspace);
	auto end = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();

	if (start >= end) return "";

	return std::string(start, end);
}


// 올바르게 Date 값 입력했는가? ex) 2025-06-13 만 검사하는 함수
inline bool isValidDateFormat(const std::string& date) {
	std::regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");
	return std::regex_match(date, pattern);
}
// Date 범위 입력값 전부 스캔하기
inline bool isValidDateRangeFormat(const std::string& range) {
	auto tokens = split(range, '~');

	if (tokens.size() == 2) {
		std::string start = trim(tokens[0]);
		std::string end = trim(tokens[1]);

		if (start.empty() && end.empty()) return false;

		return (start.empty() || isValidDateFormat(start)) &&
			(end.empty() || isValidDateFormat(end));
	}
	// 단일 날짜
	return isValidDateFormat(trim(range));
}

// 다른단위를 Byte단위로 변경해주기
inline uint64_t applySizeUnit(uint64_t value, SizeUnit unit) {
	switch (unit) {
	case SizeUnit::KB: return value * 1024ULL;
	case SizeUnit::MB: return value * 1024ULL * 1024ULL;
	case SizeUnit::GB: return value * 1024ULL * 1024ULL * 1024ULL;
	default: return value;
	}
}

// 올바르게 Size 포맷 입력했나 (숫자 아니거나 아예 비어있는 경우)
inline bool isValidSizeRangeFormat(const std::string& range) {
	auto tokens = split(range, '~');

	if (tokens.size() == 2) {
		std::string start = trim(tokens[0]);
		std::string end = trim(tokens[1]);

		// 둘 다 empty면 false
		if (start.empty() && end.empty()) return false;

		// 비어있지 않은 값은 반드시 숫자여야 함
		return (start.empty() || std::all_of(start.begin(), start.end(), ::isdigit)) &&
			(end.empty() || std::all_of(end.begin(), end.end(), ::isdigit));
	}

	// 단일 숫자
	std::string trimmed = trim(range);
	return !trimmed.empty() && std::all_of(trimmed.begin(), trimmed.end(), ::isdigit);
}

// vector<string>을 어떤 문자열 로 구분하여 연결하는 함수 기본값 = \n
inline std::string join(const std::vector<std::string>& parts, const std::string delimiter = "\n") {
	std::ostringstream oss;

	for (size_t i = 0; i < parts.size(); ++i) {
		oss << parts[i];
		if (i != parts.size() - 1) {
			oss << delimiter;
		}
	}

	return oss.str();
}







