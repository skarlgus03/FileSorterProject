#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "Block.h"
#include "BlockUtils.h"
#include "Ui/TestBlockPage.h"

#include <fstream>
#include <iostream>


using json = nlohmann::json;


class JsonManager {
public:

	static void saveAllToJson(const std::vector<std::shared_ptr<Block>> roots, const std::string& filePath);
	static std::vector<std::shared_ptr<Block>> loadAllFromJson(const std::string& filepath);

	bool isValidJson(const json& j);
	bool isFileExist(const std::string& filePath);

	void prettyPrintJson(const std::shared_ptr<Block>& block);
private:
	static json blockToJson(const std::shared_ptr<Block>& block);
	static std::shared_ptr<Block> jsonToBlock(const json& j);
	static void saveToJson(const std::shared_ptr<Block>& root, const std::string& filePath);
	static std::shared_ptr<Block> loadFromJson(const std::string& filePath);

	TestBlockPage* testBlockPage;

};