#include "Classifier.h"
#include "Ui/TestBlockPage.h"

void Classifier::classifyFile(FileInfo& file, const std::vector<std::shared_ptr<Block>>& rootBlocks,const std::string& exceptionPath)
{
	for (const auto& root : rootBlocks) {
		if (root->matches(file)) {
			auto target = followBlock(file, root);
			file.moveToPath = target->getMovePath();
			return;
		}
	}
	if (file.moveToPath.empty() && !exceptionPath.empty()) {
		file.moveToPath = exceptionPath;
	}
}

std::shared_ptr<Block> Classifier::followBlock(const FileInfo& file, const std::shared_ptr<Block> current)
{
	for (const auto& child : current->getChildren()) {
		if (child->matches(file)) {
			return followBlock(file, child);
		}
	}
	return current;
}


