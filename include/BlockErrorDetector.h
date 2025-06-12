#pragma once

#include "Block.h"
#include <vector>
#include <string>
#include <memory>



class BlockErrorDetector {
public:
    static std::vector<std::string> scan(const std::shared_ptr<Block>& root);

private:
    BlockErrorDetector() = delete;

    static void scanRecursive(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors, bool parentIsExtension);

    static void checkEmptyCondition(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);    
    static void checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);
    static void checkExtensionDuplication(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);
    static void checkDateFormat(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);
    static void checkSizeFormat(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);
};
