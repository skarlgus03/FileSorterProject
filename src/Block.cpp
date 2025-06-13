#include "Block.h"
#include <stdexcept>
#include "BlockUtils.h"

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

SizeUnit Block::getSizeUnit() const { return sizeUnit; }


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

void Block::setSizeUnit(SizeUnit unit) { sizeUnit = unit; }

// 자식 블록이 없는지 확인하는 함수
bool Block::isLeaf() const {
    return children.empty();
}

// 자식 블록을 추가하는 함수 (확장자 필터 중복 방지)
void Block::addChild(const std::shared_ptr<Block>& child) {

    child->setParent(shared_from_this());
    children.push_back(child);
}

// 비어있는(예외 타입) 자식 블록을 추가하고 반환하는 함수
std::shared_ptr<Block> Block::addEmptyChild() {
    auto child = std::make_shared<Block>(FilterType::EXTENSION, "", "");
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
        return isDateInRange(condition,file.date);
    case FilterType::SIZE:
        return isSizeInRange(condition,file.size);
    default:
        return false;
    }
}


// 날짜 비교 함수
bool Block::isDateInRange(const std::string& range, const std::string& targetDate) const {

    std::string target = trim(targetDate);
    std::string start, end;
    auto tokens = split(range, '~');
    
    if (tokens.size() == 2) {
        start = trim(tokens[0]);
        end = trim(tokens[1]);
    }
    else {
        start = end = trim(range);
    }
    

    if (start.empty() && end.empty()) return false; // 시작, 끝이 "" 면 오류
    if (!start.empty() && target < start) return false; // 시작값이 있을때 입력값이 시작 보다 작으면 false
    if (!end.empty() && target > end) return false; // 끝값이 있을때  입력값이 끝 값보다 크면 false

    return true;
}

bool Block::isSizeInRange(const std::string& range, const std::string& targetSizeStr) const {
    
    size_t targetSize;
    try {
        targetSize = std::stoull(trim(targetSizeStr));
    }
    catch (...) {
        return false;
    }

    std::string startSizeStr, endSizeStr;
    auto tokens = split(range, '~');

    if (tokens.size() == 2) {
        startSizeStr = trim(tokens[0]);
        endSizeStr = trim(tokens[1]);
    }
    else {
        startSizeStr = endSizeStr = trim(range);
    }
    

    size_t start = 0;
    size_t end = SIZE_MAX;
    try {
        if (!startSizeStr.empty()) start = applySizeUnit(std::stoull(startSizeStr),getSizeUnit());
        if (!endSizeStr.empty())   end = applySizeUnit(std::stoull(endSizeStr), getSizeUnit());
    }
    catch (...) {
        return false;
    }
    return (start <= targetSize && targetSize <= end);
}