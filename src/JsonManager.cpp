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


void JsonManager::saveAllToJson(const std::vector<std::shared_ptr<Block>>& roots, const QString& filePath, const QString& exceptionPath) {
    json j;
    j["exceptionPath"] = exceptionPath.toUtf8().constData();
    j["blocks"] = json::array();
    for (const auto& rootBlock : roots) {
        j["blocks"].push_back(blockToJson(rootBlock));
    }

    std::ofstream out(std::filesystem::path(filePath.toStdWString()));
    if (!out) {
        qWarning() << "[오류] 파일 열기 실패:" << filePath;
        return;
    }
    out << j.dump(4);
    out.close();
}

std::vector<std::shared_ptr<Block>> JsonManager::loadAllFromJson(const QString& filePath, QString& exceptionPathOut) {
    std::ifstream in(std::filesystem::path(filePath.toStdWString()));
    if (!in) {
        qWarning() << "[오류] JSON 파일 열기 실패:" << filePath;
        return {};
    }

    json j;
    in >> j;

    if (j.contains("exceptionPath")) {
        exceptionPathOut = QString::fromUtf8(j.at("exceptionPath").get<std::string>());
    }
    else {
        exceptionPathOut = "";
    }

    std::vector<std::shared_ptr<Block>> roots;
    if (j.contains("blocks")) {
        for (const auto& item : j.at("blocks")) {
            roots.push_back(jsonToBlock(item));
        }
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