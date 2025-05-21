#pragma once

#include <string>
#include "Block.h"

// 블럭 유틸리티 클래스 입니다.



// Enum FilterType 을 문자열로 변환 
inline std::string filterTypeToSting(FilterType type) {
	switch (type)
	{
	case FilterType::EXTENSION: return "EXTENSION";
	case FilterType::KEYWORD: return "KEYWORD";
	case FilterType::DATE: return "DATE";
	case FilterType::EXCEPTION: return "EXCEPTION";
	default: return "UNKNOWN";
	}
}

// 문자열을 Enum FilterType 으로 변환
inline FilterType stringToFilterType(const std::string& str) {
	if (str == "EXTENSION") return FilterType::EXTENSION;
	if (str == "KEYWORD") return FilterType::KEYWORD;
	if (str == "DATE") return FilterType::DATE;
	if (str == "EXCEPTION") return FilterType::EXCEPTION;

	throw std::invalid_argument("Unknown FilterType: " + str);
}