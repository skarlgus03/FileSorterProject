#include "JsonManager.h"



// 블럭을 JSON으로 저장
json JsonManager::blockToJson(const std::shared_ptr<Block>& block) {
	json j;
	j["filterType"] = filterTypeToSting(block->getFilterType());
	j["condition"] = block->getCondition();
	j["movePath"] = block->getMovePath();

	j["children"] = json::array();
	for (const auto& child : block->getChildren()) {
		j["children"].push_back(blockToJson(child)); // 재귀 함수
	}

	return j;
}

// Json 객체를, Json 파일로 저장
void JsonManager::saveToJson(const std::shared_ptr<Block>& root, const std::string& filePath)
{
	json j = blockToJson(root);
	std::ofstream out(filePath);
	if (!out) {
		throw std::runtime_error("파일 저장 실패: " + filePath);
	}

	out << j.dump(4);
}


// Json 객체를 블럭 객체로 변환
std::shared_ptr<Block> JsonManager::jsonToBlock(const json& j)
{
	FilterType type = stringToFilterType(j.at("filterType").get<std::string>());
	std::string condition = j.at("condition").get<std::string>();
	std::string movePath = j.at("movePath").get<std::string>();

	auto block = std::make_shared<Block>(type, condition, movePath);


	//  child가 있다면, 재귀적으로 블럭 생성
	if (j.contains("children")) {
		for (const auto& childJson : j.at("children")) {
			std::shared_ptr<Block> child = jsonToBlock(childJson);
			block->addChild(child);
		}
	}

	return block;
}

std::shared_ptr<Block> JsonManager::loadFromJson(const std::string& filePath)
{
	std::ifstream in(filePath);
	if (!in) {
		throw std::runtime_error("파일 열기 실패: " + filePath);
	}

	json j;
	in >> j;
	return jsonToBlock(j);
}



