#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "Block.h"
#include "BlockUtils.h"
#include <fstream>


using json = nlohmann::json;


class JsonManager {
public:
	static void saveToJson(const std::shared_ptr<Block>& root, const std::string& filePath);
	static std::shared_ptr<Block> loadFromJson(const std::string& filePath);
	
	bool isValidJson(const json& j);
	bool isFileExist(const std::string& filePath);

	void prettyPrintJson(const std::shared_ptr<Block>& block);
private:
	static json blockToJson(const std::shared_ptr<Block>& block);
	static std::shared_ptr<Block> jsonToBlock(const json& j);

};