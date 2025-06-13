#pragma once

#include <string>
#include <vector>
#include <memory>
#include <FileInfo.h>
#include <Qstring>

enum class FilterType {
    EXTENSION,
    KEYWORD,
    DATE,
    SIZE
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

    Block(FilterType filterType, const QString& condition, const QString& movePath = "");


    // getter 
    FilterType getFilterType() const;
    const QString& getCondition() const;
    const QString& getMovePath() const;
    SizeUnit getSizeUnit() const;
    const std::vector<std::shared_ptr<Block>>& getChildren() const;
    std::weak_ptr<Block> getParent() const;
    std::vector<std::shared_ptr<Block>>& getChildrenMutable() { return children; }

    bool isLeaf() const;

    void addChild(const std::shared_ptr<Block>& child);
    std::shared_ptr<Block> addEmptyChild();

    bool matches(const FileInfo& file)const;
    bool isDateInRange(const QString& range, const QString& target)const;
    bool isSizeInRange(const QString& range, const QString& targetSize)const;

    // 부모 설정 함수
    void setParent(const std::shared_ptr<Block>& parentBlock);

    // 자식 노드 삭제 함수
    void removeChild(const std::shared_ptr<Block>& child);

    // 빈노드 필터를 설정하게 하는 setter 함수
    void setFilterType(FilterType type);
    void setCondition(const QString& cond);
    void setMovePath(const QString& path);
    void setSizeUnit(SizeUnit unit);
private:
    FilterType filterType;
    QString condition;
    QString movePath;
    std::vector<std::shared_ptr<Block>> children;
    SizeUnit sizeUnit = SizeUnit::B;

    // 부모 노드를 가리키는 weak_ptr 멤버 추가
    std::weak_ptr<Block> parent;
};
