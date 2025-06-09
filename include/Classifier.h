#pragma once

#include <vector>
#include <memory>
#include <Block.h>
#include <FileInfo.h>


class Classifier {
public:
	static void classifyFile(FileInfo& file,
							 const std::vector<std::shared_ptr<Block>>& rootBlocks,
							 const std::string& exceptionPath);

private:
	static std::shared_ptr<Block> followBlock(const FileInfo& file, const std::shared_ptr<Block> current);
};