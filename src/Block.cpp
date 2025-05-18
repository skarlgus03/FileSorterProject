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
    FilterType filterType;                      // ������ ���� (Ȯ����, Ű����, ��¥, ����ó��)
    std::string condition;                      // ���� ���� (��: "exe", "report", "20250518")
    std::string movePath;                       // ���� ����� ��, ������ �̵��� ���
    std::vector<std::shared_ptr<Block>> children; // �ڽ� ���� ���� (Ʈ�� ����)
};

#endif // BLOCK_H
