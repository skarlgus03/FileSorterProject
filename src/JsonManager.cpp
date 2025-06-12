#include "JsonManager.h"
#include "Ui/TestBlockPage.h"


// 블럭을 JSON으로 저장
json JsonManager::blockToJson(const std::shared_ptr<Block>& block) {
	json j;
	j["filterType"] = filterTypeToString(block->getFilterType());
	j["condition"] = block->getCondition();
	j["movePath"] = block->getMovePath();

	j["children"] = json::array();
	for (const auto& child : block->getChildren()) {
		j["children"].push_back(blockToJson(child)); // 재귀 함수
	}

	return j;
}


// Json 객체를 블럭 객체로 변환
std::shared_ptr<Block> JsonManager::jsonToBlock(const json& j) {
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

// Json 객체를, Json 파일로 저장
void JsonManager::saveToJson(const std::shared_ptr<Block>& root, const std::string& filePath) {
	json j = blockToJson(root);
	std::ofstream out(filePath);
	if (!out) {
		throw std::runtime_error("파일 저장 실패: " + filePath);
	}

	out << j.dump(4);
}

// Json 파일을 Json 객체로 불러오기
std::shared_ptr<Block> JsonManager::loadFromJson(const std::string& filePath) {
	std::ifstream in(filePath);
	if (!in) {
		throw std::runtime_error("파일 열기 실패: " + filePath);
	}

	json j;
	in >> j;
	return jsonToBlock(j);
}



// 루트 블럭 순회하면서 json으로 저장 -> json파일 생성
void JsonManager::saveAllToJson(const std::vector<std::shared_ptr<Block>> roots, const std::string& filePath){
	json rootArray = json::array();
	for (const auto& rootBlock : roots) {
		rootArray.push_back(blockToJson(rootBlock));
	}

	std::ofstream out(filePath);
	out << rootArray.dump(4);
	out.close();
}

// json 순회하면서 블럭으로 만들기
std::vector<std::shared_ptr<Block>> JsonManager::loadAllFromJson(const std::string& filepath) {
	std::ifstream in(filepath);
	json rootArray;
	in >> rootArray;

	std::vector<std::shared_ptr<Block>> roots;
	for (const auto& item : rootArray) {
		roots.push_back(jsonToBlock(item));
	}
	return roots;
}




// 올바른 JSON 파일 형식이 맞는지 검사
bool JsonManager::isValidJson(const json& j) {
	return j.contains("filterType") && j.contains("condition") && j.contains("movePath") && j.contains("children");
}
// 파일 이름을 받아서, 그 파일이 존재 하는가 검사
bool JsonManager::isFileExist(const std::string& filePath) {
	std::ifstream f(filePath);
	return f.good();
}

// 디버깅용 block to Json 출력해주는 함수
void JsonManager::prettyPrintJson(const std::shared_ptr<Block>& block) {
	json j = blockToJson(block);
	std::cout << j.dump(4) << std::endl;
}

