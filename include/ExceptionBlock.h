#pragma once
#include <string>

class ExceptionBlock {
public:
    ExceptionBlock() = default;
    explicit ExceptionBlock(const std::string& movePath) : movePath(movePath) {}

    // 이동 경로 설정
    void setMovePath(const std::string& path) {
        movePath = path;
    }

    // 이동 경로 반환
    const std::string& getMovePath() const {
        return movePath;
    }

private:
    std::string movePath;
};
