#include "JsonManager.h"
#include "Ui/TestBlockPage.h"

// 블럭 객체를 JSON으로 저장
json JsonManager::blockToJson(const std::shared_ptr<Block>& block) {
    json j;
    j["filterType"] = filterTypeToString(block->getFilterType()).toUtf8().constData();
    j["condition"] = block->getCondition().toUtf8().constData();
    j["movePath"] = block->getMovePath().toUtf8().constData();

    if (block->getFilterType() == FilterType::SIZE) {
        j["sizeUnit"] = sizeUnitToString(block->getSizeUnit()).toUtf8().constData();
    }

    j["children"] = json::array();
    for (const auto& child : block->getChildren()) {
        j["children"].push_back(blockToJson(child));
    }

    return j;
}

// JSON 객체를 블럭 객체로 변환
std::shared_ptr<Block> JsonManager::jsonToBlock(const json& j) {
    FilterType type = stringToFilterType(QString::fromUtf8(j.at("filterType").get<std::string>()));
    QString condition = QString::fromUtf8(j.at("condition").get<std::string>());
    QString movePath = QString::fromUtf8(j.at("movePath").get<std::string>());

    auto block = std::make_shared<Block>(type, condition, movePath);

    if (type == FilterType::SIZE && j.contains("sizeUnit")) {
        block->setSizeUnit(stringToSizeUnit(QString::fromUtf8(j.at("sizeUnit").get<std::string>())));
    }

    if (j.contains("children")) {
        for (const auto& childJson : j.at("children")) {
            std::shared_ptr<Block> child = jsonToBlock(childJson);
            block->addChild(child);
        }
    }

    return block;
}

// JSON 저장 (단일 루트)
void JsonManager::saveToJson(const std::shared_ptr<Block>& root, const QString& filePath) {
    json j = blockToJson(root);
    std::ofstream out(filePath.toStdString());
    if (!out) {
        throw std::runtime_error("파일 저장 실패: " + filePath.toStdString());
    }
    out << j.dump(4);
}

// JSON 불러오기 (단일 루트)
std::shared_ptr<Block> JsonManager::loadFromJson(const QString& filePath) {
    std::ifstream in(filePath.toStdString());
    if (!in) {
        throw std::runtime_error("파일 열기 실패: " + filePath.toStdString());
    }

    json j;
    in >> j;
    return jsonToBlock(j);
}

// 여러 루트 저장
void JsonManager::saveAllToJson(const std::vector<std::shared_ptr<Block>>& roots, const QString& filePath) {
    json rootArray = json::array();
    for (const auto& rootBlock : roots) {
        rootArray.push_back(blockToJson(rootBlock));
    }
    std::ofstream out(std::filesystem::path(filePath.toStdWString()));
    if (!out) {
        qWarning() << "[오류] 파일 열기 실패:" << filePath;
        return;
    }
    out << rootArray.dump(4);
    out.close();
}

// 여러 루트 불러오기
std::vector<std::shared_ptr<Block>> JsonManager::loadAllFromJson(const QString& filePath) {
    std::ifstream in(std::filesystem::path(filePath.toStdWString()));
    if (!in) {
        qWarning() << "[오류] JSON 파일 열기 실패:" << filePath;
        return {};
    }
    json rootArray;
    in >> rootArray;

    std::vector<std::shared_ptr<Block>> roots;
    for (const auto& item : rootArray) {
        roots.push_back(jsonToBlock(item));
    }
    return roots;
}

// JSON 유효성 검사
bool JsonManager::isValidJson(const json& j) {
    return j.contains("filterType") && j.contains("condition") && j.contains("movePath") && j.contains("children");
}

// 파일 존재 여부
bool JsonManager::isFileExist(const QString& filePath) {
    std::ifstream f(filePath.toStdString());
    return f.good();
}

// 디버그용 출력
void JsonManager::prettyPrintJson(const std::shared_ptr<Block>& block) {
    json j = blockToJson(block);
    std::cout << j.dump(4) << std::endl;
}