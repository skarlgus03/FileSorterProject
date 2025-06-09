#include "Block.h"
#include <stdexcept>

// Block 클래스의 기본 생성자: 멤버 변수들을 기본값으로 초기화
Block::Block()
    : filterType(FilterType()), condition(""), movePath("") {
}

// Block 클래스의 매개변수 생성자: 전달받은 값으로 멤버 변수 초기화
Block::Block(FilterType filterType, const std::string& condition, const std::string& movePath)
    : filterType(filterType), condition(condition), movePath(movePath) {
}

// filterType 멤버 변수 값을 반환하는 getter 함수
FilterType Block::getFilterType() const { return filterType; }

// condition 멤버 변수 값을 반환하는 getter 함수
const std::string& Block::getCondition() const { return condition; }

// movePath 멤버 변수 값을 반환하는 getter 함수
const std::string& Block::getMovePath() const { return movePath; }

ComparisonType Block::getComparisonType() const { return comparisonType; }

// 자식 블록 목록을 반환하는 getter 함수
const std::vector<std::shared_ptr<Block>>& Block::getChildren() const { return children; }

// 부모 블록 반환
std::weak_ptr<Block> Block::getParent() const { return parent; }



// filterType 멤버 변수 값을 설정하는 setter 함수
void Block::setFilterType(FilterType type) { filterType = type; }

// condition 멤버 변수 값을 설정하는 setter 함수
void Block::setCondition(const std::string& cond) { condition = cond; }

// movePath 멤버 변수 값을 설정하는 setter 함수
void Block::setMovePath(const std::string& path) { movePath = path; }

void Block::setComparisonType(ComparisonType type) { comparisonType = type; }

// 자식 블록이 없는지 확인하는 함수
bool Block::isLeaf() const {
    return children.empty();
}

// 자식 블록을 추가하는 함수 (확장자 필터 중복 방지)
void Block::addChild(const std::shared_ptr<Block>& child) {
    if (this->filterType == FilterType::EXTENSION && child->filterType == FilterType::EXTENSION) {
        throw std::invalid_argument("확장자는 다중 선택할 수 없습니다.");
    }
    child->setParent(shared_from_this());
    children.push_back(child);
}

// 비어있는(예외 타입) 자식 블록을 추가하고 반환하는 함수
std::shared_ptr<Block> Block::addEmptyChild() {
    auto child = std::make_shared<Block>(FilterType::EXCEPTION, "", "");
    child->setParent(shared_from_this());     //  부모 등록
    children.push_back(child);                
    return child;
}


// 부모 설정 함수 구현
void Block::setParent(const std::shared_ptr<Block>& parentBlock) {
    parent = parentBlock;
}

// 자식 노드 삭제 함수 구현
void Block::removeChild(const std::shared_ptr<Block>& child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}

// File의 값과 비교하는 함수
bool Block::matches(const FileInfo& file) const
{
    switch (filterType)
    {
    case FilterType::EXTENSION:
        return file.extension == condition;
    case FilterType::KEYWORD:
        return file.fileName.find(condition) != std::string::npos;
    case FilterType::DATE:
        return file.date == condition;
    case FilterType::EXCEPTION:
        return false;
    case FilterType::SIZE:
        return matchSizeCondition(file);
    default:
        return false;
    }
}

// 크기비교 함수
bool Block::matchSizeCondition(const FileInfo& file) const
{
    std::uintmax_t fileSize = std::stoull(file.size);
    std::uintmax_t conditionSize = std::stoull(condition);
    switch (comparisonType)
    {
    case ComparisonType::GREATER_EQUAL:
        return fileSize >= conditionSize;
    case ComparisonType::LESS_EQUAL:
        return fileSize <= conditionSize;
    case ComparisonType::GREATER:
        return fileSize > conditionSize;
    case ComparisonType::LESS:
        return fileSize < conditionSize;
    case ComparisonType::EQUAL:
        return fileSize == conditionSize;
    default:
        return false;
    }
}
