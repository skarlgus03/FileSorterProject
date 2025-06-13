#pragma once

#include <string>
#include <vector>
#include <memory>
#include <FileInfo.h>

enum class FilterType {
    EXTENSION,
    KEYWORD,
    DATE,
    SIZE
};

enum class ComparisonType {
    GREATER_EQUAL, // 이상 >=
    LESS_EQUAL,    // 이하 <=
    GREATER,       // 초과 > 
    LESS,          // 미만 <
    EQUAL          // 동일 =
};

enum class SizeUnit {
    B,
    KB,
    MB,
    GB
};

class Block : public std::enable_shared_from_this<Block> {
public:
    Block();

    Block(FilterType filterType, const std::string& condition, const std::string& movePath = "");


    // getter 
    FilterType getFilterType() const;
    const std::string& getCondition() const;
    const std::string& getMovePath() const;
    ComparisonType getComparisonType() const;
    SizeUnit getSizeUnit() const;
    const std::vector<std::shared_ptr<Block>>& getChildren() const;
    std::weak_ptr<Block> getParent() const;
    std::vector<std::shared_ptr<Block>>& getChildrenMutable() { return children; }

    bool isLeaf() const;

    void addChild(const std::shared_ptr<Block>& child);
    std::shared_ptr<Block> addEmptyChild();

    bool matches(const FileInfo& file)const;
    bool matchSizeCondition(const FileInfo& file) const;
    bool isDateInRange(const std::string& range, const std::string& target)const;
    bool isSizeInRange(const std::string& range, const std::string& targetSize)const;

    // 부모 설정 함수
    void setParent(const std::shared_ptr<Block>& parentBlock);

    // 자식 노드 삭제 함수
    void removeChild(const std::shared_ptr<Block>& child);

    // 빈노드 필터를 설정하게 하는 setter 함수
    void setFilterType(FilterType type);
    void setCondition(const std::string& cond);
    void setMovePath(const std::string& path);
    void setComparisonType(ComparisonType type);
    void setSizeUnit(SizeUnit unit);
private:
    FilterType filterType;
    std::string condition;
    std::string movePath;
    std::vector<std::shared_ptr<Block>> children;
    ComparisonType comparisonType = ComparisonType::GREATER_EQUAL; // 기본값
    SizeUnit sizeUnit = SizeUnit::B;

    // 부모 노드를 가리키는 weak_ptr 멤버 추가
    std::weak_ptr<Block> parent;
};
