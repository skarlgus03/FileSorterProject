#include "Block.h"

Block::Block(FilterType filterType, const std::string& condition, const std::string& movePath)
    : filterType(filterType), condition(condition), movePath(movePath) {
}

FilterType Block::getFilterType() const {
    return filterType;
}

const std::string& Block::getCondition() const {
    return condition;
}

const std::string& Block::getMovePath() const {
    return movePath;
}

bool Block::isLeaf() const {
    return children.empty();
}

void Block::addChild(const std::shared_ptr<Block>& child) {
    children.push_back(child);
}

const std::vector<std::shared_ptr<Block>>& Block::getChildren() const {
    return children;
}
