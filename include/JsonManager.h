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

	
	static void saveAllToJson(const std::vector<std::shared_ptr<Block>>& roots, const QString& filePath, const QString& exceptionPath);

	static std::vector<std::shared_ptr<Block>> loadAllFromJson(const QString& filepath, QString& exceptionPathOut);

	bool isValidJson(const json& j);
	static bool isFileExist(const QString& filePath);

	void prettyPrintJson(const std::shared_ptr<Block>& block);
private:
	static json blockToJson(const std::shared_ptr<Block>& block);
	static std::shared_ptr<Block> jsonToBlock(const json& j);

	TestBlockPage* testBlockPage;

};