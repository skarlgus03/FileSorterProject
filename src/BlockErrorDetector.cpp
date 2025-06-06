﻿#include "BlockErrorDetector.h"

std::vector<std::string> BlockErrorDetector::scan(const std::shared_ptr<Block>& root) {
    std::vector<std::string> errors;

    if (!root) {
        errors.push_back("블록 트리가 비어 있습니다.");
        return errors;
    }

    scanRecursive(root, "", errors);
    return errors;
}

// 블록 트리를 재귀적으로 순회하며 검사 함수를 호출한다.
void BlockErrorDetector::scanRecursive(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (!block) return;

    std::string currentPath = path + "/" + filterTypeToString(block->getFilterType()) + ":" + block->getCondition();

    checkEmptyCondition(block, currentPath, errors);
    checkLeafWithoutMovePath(block, currentPath, errors);
    checkMultipleExtensionInSameParent(block, currentPath, errors);

    for (const auto& child : block->getChildren()) {
        scanRecursive(child, currentPath, errors);
    }
}


// 조건이 비어 있는 경우
void BlockErrorDetector::checkEmptyCondition(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->getCondition().empty()) {
        errors.push_back("조건이 비어 있음 → 경로: " + path);
    }
}


//말단 블록인데 이동 경로가 없는 경우
void BlockErrorDetector::checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->isLeaf() && block->getMovePath().empty()) {
        errors.push_back("이동 경로가 없는 말단 블록 → 경로: " + path);
    }
}

// 하나의 트리에 확장자가 2개 이상인 경우
void BlockErrorDetector::checkMultipleExtensionInSameParent(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    // 자식이 없으면 검사할 필요 없음
    if (block->getChildren().empty()) return;

    int extensionCount = 0;

    for (const auto& child : block->getChildren()) {
        if (child && child->getFilterType() == FilterType::EXTENSION) {
            extensionCount++;
        }
    }

    if (extensionCount > 1) {
        errors.push_back("같은 부모 블록 아래에 확장자 조건이 2개 이상 존재함 → 경로: " + path);
    }
}


// FilterType enum을 문자열로 변환
std::string BlockErrorDetector::filterTypeToString(FilterType type) {
    switch (type) {
    case FilterType::EXTENSION: return "EXTENSION";
    case FilterType::KEYWORD:   return "KEYWORD";
    case FilterType::DATE:      return "DATE";
    case FilterType::EXCEPTION: return "EXCEPTION";
    case FilterType::SIZE:      return "SIZE";
    default: return "UNKNOWN";
    }
}
