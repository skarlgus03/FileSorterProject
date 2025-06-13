#pragma once

#include "Block.h"
#include <QString>
#include <QStringList>
#include <memory>

class BlockErrorDetector {
public:
    static QStringList scan(const std::shared_ptr<Block>& root);

private:
    BlockErrorDetector() = delete;

    static void scanRecursive(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors, bool parentIsExtension);

    static void checkEmptyCondition(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors);
    static void checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors);
    static void checkExtensionDuplication(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors);
    static void checkDateFormat(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors);
    static void checkSizeFormat(const std::shared_ptr<Block>& block, const QString& path, QStringList& errors);
};
