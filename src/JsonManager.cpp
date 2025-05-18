#include "JsonManager.h"



// ���� JSON���� ����
json JsonManager::blockToJson(const std::shared_ptr<Block>& block) {
	json j;
	j["filterType"] = filterTypeToSting(block->getFilterType());
	j["condition"] = block->getCondition();
	j["movePath"] = block->getMovePath();

	j["children"] = json::array();
	for (const auto& child : block->getChildren()) {
		j["children"].push_back(blockToJson(child)); // ��� �Լ�
	}

	return j;
}

// Json ��ü��, Json ���Ϸ� ����
void JsonManager::saveToJson(const std::shared_ptr<Block>& root, const std::string& filePath)
{
	json j = blockToJson(root);
	std::ofstream out(filePath);
	if (!out) {
		throw std::runtime_error("���� ���� ����: " + filePath);
	}

	out << j.dump(4);
}


// Json ��ü�� �� ��ü�� ��ȯ
std::shared_ptr<Block> JsonManager::jsonToBlock(const json& j)
{
	FilterType type = stringToFilterType(j.at("filterType").get<std::string>());
	std::string condition = j.at("condition").get<std::string>();
	std::string movePath = j.at("movePath").get<std::string>();

	auto block = std::make_shared<Block>(type, condition, movePath);


	//  child�� �ִٸ�, ��������� �� ����
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
		throw std::runtime_error("���� ���� ����: " + filePath);
	}

	json j;
	in >> j;
	return jsonToBlock(j);
}



