#include "BlockErrorDetector.h"

std::vector<std::string> BlockErrorDetector::scan(const std::shared_ptr<Block>& root) {
    std::vector<std::string> errors;

    if (!root) {
        errors.push_back("��� Ʈ���� ��� �ֽ��ϴ�.");
        return errors;
    }

    scanRecursive(root, "", errors);
    return errors;
}

// ��� Ʈ���� ��������� ��ȸ�ϸ� �˻� �Լ��� ȣ���Ѵ�.
void BlockErrorDetector::scanRecursive(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (!block) return;

    std::string currentPath = path + "/" + filterTypeToString(block->getFilterType()) + ":" + block->getCondition();

    checkEmptyCondition(block, currentPath, errors);
    checkLeafWithoutMovePath(block, currentPath, errors);

    for (const auto& child : block->getChildren()) {
        scanRecursive(child, currentPath, errors);
    }
}


// ������ ��� �ִ� ���
void BlockErrorDetector::checkEmptyCondition(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->getCondition().empty()) {
        errors.push_back("������ ��� ���� �� ���: " + path);
    }
}


//���� ����ε� �̵� ��ΰ� ���� ���
void BlockErrorDetector::checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors) {
    if (block->isLeaf() && block->getMovePath().empty()) {
        errors.push_back("�̵� ��ΰ� ���� ���� ��� �� ���: " + path);
    }
}


// FilterType enum�� ���ڿ��� ��ȯ
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
