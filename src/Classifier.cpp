#include "Classifier.h"
#include "Ui/TestBlockPage.h"

void Classifier::classifyFile(FileInfo& file, const std::vector<std::shared_ptr<Block>>& rootBlocks, const QString& exceptionPath)
{
    for (const auto& root : rootBlocks) {
        if (root->matches(file)) {
            auto target = followBlock(file, root);
            if (target && !target->getMovePath().isEmpty()) {
                file.moveToPath = target->getMovePath();
                return;
            }
        }
    }

    if (file.moveToPath.isEmpty() && !exceptionPath.isEmpty()) {
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
