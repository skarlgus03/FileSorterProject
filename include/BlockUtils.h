#pragma once

#include <string>
#include "Block.h"

// �� ��ƿ��Ƽ Ŭ���� �Դϴ�.



// Enum FilterType �� ���ڿ��� ��ȯ 
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

// ���ڿ��� Enum FilterType ���� ��ȯ
inline FilterType stringToFilterType(const std::string& str) {
	if (str == "EXTENSION") return FilterType::EXTENSION;
	if (str == "KEYWORD") return FilterType::KEYWORD;
	if (str == "DATE") return FilterType::DATE;
	if (str == "EXCEPTION") return FilterType::EXCEPTION;

	throw std::invalid_argument("Unknown FilterType: " + str);
}