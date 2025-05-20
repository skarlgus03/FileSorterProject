#include "Block.h"
#include <stdexcept>

Block::Block()
    : filterType(FilterType()), condition(""), movePath("") {
}

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
    if (this->filterType == FilterType::EXTENSION && child->filterType == FilterType::EXTENSION) {
        throw std::invalid_argument("확장자는 다중 선택할 수 없습니다.");
    }
    children.push_back(child);
}

std::shared_ptr<Block> Block::addEmptyChild() {
    auto child = std::make_shared<Block>(FilterType::EXCEPTION, "", "");
    children.push_back(child);
    return child;
}

const std::vector<std::shared_ptr<Block>>& Block::getChildren() const {
    return children;
}
