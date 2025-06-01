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

    static void scanRecursive(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);

    static void checkEmptyCondition(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);
        
    static void checkLeafWithoutMovePath(const std::shared_ptr<Block>& block, const std::string& path, std::vector<std::string>& errors);

    static std::string filterTypeToString(FilterType type);  // 내부에서만 사용
};
