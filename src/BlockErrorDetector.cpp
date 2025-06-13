#include "BlockErrorDetector.h"
#include "BlockUtils.h"

std::vector<std::string> BlockErrorDetector::scan(const std::shared_ptr<Block>& root) {
    std::vector<std::string> errors;

    if (!root) {
        errors.push_back("블록 트리가 비어 있습니다.");
        return errors;
    }

    scanRecursive(root, "", errors,false);
    return errors;
}

// 블록 트리를 재귀적으로 순회하며 검사 함수를 호출한다.
void BlockErrorDetector::scanRecursive(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors, bool parentIsExtension) {
    if (!block) return;

    


    std::string currentPath = path + "/" + filterTypeToString(block->getFilterType()) + ":" + block->getCondition();
    if (block->getFilterType() != FilterType::KEYWORD) {
        checkEmptyCondition(block, currentPath, errors);
    }
    checkLeafWithoutMovePath(block, currentPath, errors);
    if (!parentIsExtension && block->getFilterType() == FilterType::EXTENSION) {
        checkExtensionDuplication(block,currentPath,errors);
    }
    if (block->getFilterType() == FilterType::DATE) {
        checkDateFormat(block,currentPath,errors);
    }
    if (block->getFilterType() == FilterType::SIZE) {
        checkSizeFormat(block, currentPath, errors);
    }


    for (const auto& child : block->getChildren()) {
        scanRecursive(child, currentPath, errors, block->getFilterType() == FilterType::EXTENSION);
    }
}

// 조건이 비어 있는 경우
void BlockErrorDetector::checkEmptyCondition(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->getCondition().empty()) {
        errors.push_back("⚠️ 조건이 비어 있음 → 경로: " + path);
        return;
    }
}

//말단 블록인데 이동 경로가 없는 경우
void BlockErrorDetector::checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->isLeaf() && block->getMovePath().empty()) {
        errors.push_back("⚠️ 이동 경로가 없는 말단 블록 → 경로: " + path);
        return;
    }
}

void BlockErrorDetector::checkExtensionDuplication(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    for (const auto& child : block->getChildren()) {
        if (child->getFilterType() == FilterType::EXTENSION) {
            errors.push_back("⚠️ 중복된 EXTENSION 필터타입이 존재합니다 → 경로: " + path);
            return;
        }
    }
}

void BlockErrorDetector::checkDateFormat(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (!isValidDateRangeFormat(block->getCondition())) {
        errors.push_back("⚠️ Date 필터타입의 입력값이 잘못되었습니다. → 경로:" + path);
        return;
    }
}

void BlockErrorDetector::checkSizeFormat(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (!isValidSizeRangeFormat(block->getCondition())) {
        errors.push_back("⚠️ Size 필터타입의 입력값이 잘못되었습니다. → 경로: " + path);
        return;
    }
}








