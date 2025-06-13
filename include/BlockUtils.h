#pragma once

#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include "Block.h"

// FilterType enum → 문자열
inline QString filterTypeToString(FilterType type) {
    switch (type) {
    case FilterType::EXTENSION: return "EXTENSION";
    case FilterType::KEYWORD: return "KEYWORD";
    case FilterType::DATE: return "DATE";
    case FilterType::SIZE: return "SIZE";
    default: return "UNKNOWN";
    }
}

inline FilterType stringToFilterType(const QString& str) {
    if (str == "EXTENSION") return FilterType::EXTENSION;
    if (str == "KEYWORD") return FilterType::KEYWORD;
    if (str == "DATE") return FilterType::DATE;
    if (str == "SIZE") return FilterType::SIZE;
    throw std::invalid_argument("Unknown FilterType: " + str.toStdString());
}

inline QString sizeUnitToString(SizeUnit unit) {
    switch (unit) {
    case SizeUnit::B: return "Byte";
    case SizeUnit::KB: return "KB";
    case SizeUnit::MB: return "MB";
    case SizeUnit::GB: return "GB";
    default: return "UNKNOWN";
    }
}

inline SizeUnit stringToSizeUnit(const QString& str) {
    if (str == "B") return SizeUnit::B;
    if (str == "KB") return SizeUnit::KB;
    if (str == "MB") return SizeUnit::MB;
    if (str == "GB") return SizeUnit::GB;
    throw std::invalid_argument("Unknown SizeUnit: " + str.toStdString());
}

// 문자열 분할
inline QStringList split(const QString& str, QChar delim) {
    return str.split(delim);
}

// 문자열 trim
inline QString trim(const QString& s) {
    return s.trimmed();
}

// 날짜 형식 검사 (ex. 2025-06-13)
inline bool isValidDateFormat(const QString& date) {
    QRegularExpression re("^\\d{4}-\\d{2}-\\d{2}$");
    return re.match(date).hasMatch();
}

// 날짜 범위 형식 검사 (ex. 2025-01-01~2025-12-31)
inline bool isValidDateRangeFormat(const QString& range) {
    auto tokens = split(range, '~');
    if (tokens.size() == 2) {
        QString start = trim(tokens[0]);
        QString end = trim(tokens[1]);
        if (start.isEmpty() && end.isEmpty()) return false;
        return (start.isEmpty() || isValidDateFormat(start)) &&
            (end.isEmpty() || isValidDateFormat(end));
    }
    return isValidDateFormat(trim(range));
}

// 크기 단위 변환
inline uint64_t applySizeUnit(uint64_t value, SizeUnit unit) {
    switch (unit) {
    case SizeUnit::KB: return value * 1024ULL;
    case SizeUnit::MB: return value * 1024ULL * 1024ULL;
    case SizeUnit::GB: return value * 1024ULL * 1024ULL * 1024ULL;
    default: return value;
    }
}

// 크기 범위 형식 검사
inline bool isValidSizeRangeFormat(const QString& range) {
    auto tokens = split(range, '~');
    auto isNumeric = [](const QString& s) {
        return !s.isEmpty() && std::all_of(s.begin(), s.end(), [](QChar c) { return c.isDigit(); });
        };

    if (tokens.size() == 2) {
        QString start = trim(tokens[0]);
        QString end = trim(tokens[1]);
        if (start.isEmpty() && end.isEmpty()) return false;
        return (start.isEmpty() || isNumeric(start)) &&
            (end.isEmpty() || isNumeric(end));
    }

    QString single = trim(range);
    return isNumeric(single);
}

// 문자열 배열 합치기
inline QString join(const QStringList& parts, const QString& delimiter = "\n") {
    return parts.join(delimiter);
}
