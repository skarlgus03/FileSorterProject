#include "BlockErrorDetector.h"
#include "BlockUtils.h"

QStringList BlockErrorDetector::scan(const std::shared_ptr<Block>& root) {
    QStringList errors;

    if (!root) {
        errors.append("❌ 블록 트리가 비어 있습니다.");
        return errors;
    }

    scanRecursive(root, "", errors, false);
    return errors;
}

void BlockErrorDetector::scanRecursive(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors, bool parentIsExtension) {
    if (!block) return;

    QString currentPath = path + "/" + filterTypeToString(block->getFilterType()) + ":" + block->getCondition();

    if (block->getFilterType() != FilterType::KEYWORD) {
        checkEmptyCondition(block, currentPath, errors);
    }

    checkLeafWithoutMovePath(block, currentPath, errors);

    if (!parentIsExtension && block->getFilterType() == FilterType::EXTENSION) {
        checkExtensionDuplication(block, currentPath, errors);
    }

    if (block->getFilterType() == FilterType::DATE) {
        checkDateFormat(block, currentPath, errors);
    }

    if (block->getFilterType() == FilterType::SIZE) {
        checkSizeFormat(block, currentPath, errors);
    }

    for (const auto& child : block->getChildren()) {
        scanRecursive(child, currentPath, errors, block->getFilterType() == FilterType::EXTENSION);
    }
}

void BlockErrorDetector::checkEmptyCondition(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors) {
    if (block->getCondition().isEmpty()) {
        errors.append(QString("⚠️ 조건이 비어 있음 → 경로: %1").arg(path));
    }
}

void BlockErrorDetector::checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors) {
    if (block->isLeaf() && block->getMovePath().isEmpty()) {
        errors.append(QString("⚠️ 이동 경로가 없는 말단 블록 → 경로: %1").arg(path));
    }
}

void BlockErrorDetector::checkExtensionDuplication(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors) {
    for (const auto& child : block->getChildren()) {
        if (child->getFilterType() == FilterType::EXTENSION) {
            errors.append(QString("⚠️ 중복된 EXTENSION 필터타입이 존재합니다 → 경로: %1").arg(path));
            return;
        }
    }
}

void BlockErrorDetector::checkDateFormat(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors) {
    if (!isValidDateRangeFormat(block->getCondition())) {
        errors.append(QString("⚠️ Date 필터타입의 입력값이 잘못되었습니다 → 경로: %1").arg(path));
    }
}

void BlockErrorDetector::checkSizeFormat(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors) {
    if (!isValidSizeRangeFormat(block->getCondition())) {
        errors.append(QString("⚠️ Size 필터타입의 입력값이 잘못되었습니다 → 경로: %1").arg(path));
    }
}
