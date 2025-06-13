#include "Block.h"
#include <stdexcept>
#include "BlockUtils.h"

// Block 클래스의 기본 생성자: 멤버 변수들을 기본값으로 초기화
Block::Block()
    : filterType(FilterType()), condition(""), movePath("") {
}

// Block 클래스의 매개변수 생성자: 전달받은 값으로 멤버 변수 초기화
Block::Block(FilterType filterType, const QString& condition, const QString& movePath)
    : filterType(filterType), condition(condition), movePath(movePath) {
}

// filterType 멤버 변수 값을 반환하는 getter 함수
FilterType Block::getFilterType() const { return filterType; }

// condition 멤버 변수 값을 반환하는 getter 함수
const QString& Block::getCondition() const { return condition; }

// movePath 멤버 변수 값을 반환하는 getter 함수
const QString& Block::getMovePath() const { return movePath; }


SizeUnit Block::getSizeUnit() const { return sizeUnit; }


// 자식 블록 목록을 반환하는 getter 함수
const std::vector<std::shared_ptr<Block>>& Block::getChildren() const { return children; }

// 부모 블록 반환
std::weak_ptr<Block> Block::getParent() const { return parent; }



// filterType 멤버 변수 값을 설정하는 setter 함수
void Block::setFilterType(FilterType type) { filterType = type; }

// condition 멤버 변수 값을 설정하는 setter 함수
void Block::setCondition(const QString& cond) { condition = cond; }

// movePath 멤버 변수 값을 설정하는 setter 함수
void Block::setMovePath(const QString& path) { movePath = path; }

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
        return file.fileName.contains(condition, Qt::CaseInsensitive);

    case FilterType::DATE:
        return isDateInRange(condition, file.date);

    case FilterType::SIZE:
        return isSizeInRange(condition, file.size);

    default:
        return false;
    }
}


// 날짜 비교 함수
bool Block::isDateInRange(const QString& range, const QString& targetDate) const {
    QString target = targetDate.trimmed();
    QString start, end;
    QStringList tokens = range.split('~');

    if (tokens.size() == 2) {
        start = tokens[0].trimmed();
        end = tokens[1].trimmed();
    }
    else {
        start = end = range.trimmed();
    }

    if (start.isEmpty() && end.isEmpty()) return false;
    if (!start.isEmpty() && target < start) return false;
    if (!end.isEmpty() && target > end) return false;

    return true;
}

bool Block::isSizeInRange(const QString& range, const QString& targetSizeStr) const {
    bool ok = false;
    quint64 targetSize = targetSizeStr.trimmed().toULongLong(&ok);
    if (!ok) return false;

    QString startSizeStr, endSizeStr;
    QStringList tokens = range.split('~');

    if (tokens.size() == 2) {
        startSizeStr = tokens[0].trimmed();
        endSizeStr = tokens[1].trimmed();
    }
    else {
        startSizeStr = endSizeStr = range.trimmed();
    }

    quint64 start = 0;
    quint64 end = std::numeric_limits<quint64>::max();

    if (!startSizeStr.isEmpty()) {
        bool okStart = false;
        start = applySizeUnit(startSizeStr.toULongLong(&okStart), getSizeUnit());
        if (!okStart) return false;
    }

    if (!endSizeStr.isEmpty()) {
        bool okEnd = false;
        end = applySizeUnit(endSizeStr.toULongLong(&okEnd), getSizeUnit());
        if (!okEnd) return false;
    }

    return (start <= targetSize && targetSize <= end);
}