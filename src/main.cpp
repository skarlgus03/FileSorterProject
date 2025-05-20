

#include <iostream>
#include "Block.h"
#include "JsonManager.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);



    auto root = std::make_shared<Block>(FilterType::EXTENSION, "exe", "C:/exe");
    auto child = root->addEmptyChild();


    try {
        JsonManager::saveToJson(root, "text.json");
        std::cout << "저장 성공" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "저장 실패: " << e.what() << std::endl;
    }

    return 0;
}