#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <memory>

enum class FilterType {
    EXTENSION,
    KEYWORD,
    DATE,
    EXCEPTION
};

class Block {
public:
    Block(FilterType filterType,
        const std::string& condition,
        const std::string& movePath = "");

    FilterType getFilterType() const;
    const std::string& getCondition() const;
    const std::string& getMovePath() const;
    bool isLeaf() const;

    void addChild(const std::shared_ptr<Block>& child);
    const std::vector<std::shared_ptr<Block>>& getChildren() const;

private:
    FilterType filterType;                      // 필터의 종류 (확장자, 키워드, 날짜, 예외처리)
    std::string condition;                      // 필터 조건 (예: "exe", "report", "20250518")
    std::string movePath;                       // 말단 노드일 때, 파일을 이동할 경로
    std::vector<std::shared_ptr<Block>> children; // 자식 필터 노드들 (트리 구조)
};

#endif // BLOCK_H
