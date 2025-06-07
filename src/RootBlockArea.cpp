#include "Ui/RootBlockArea.h"
#include "Ui/BlockWidget.h"
#include "UIConstants.h"
#include <algorithm>

RootBlockArea::RootBlockArea(QWidget* parent, int y)
    : QFrame(parent) {
    move(10, y);
    setFrameStyle(QFrame::Box); 
    setStyleSheet("background-color: rgba(18, 20, 28, 150);");
    setFixedWidth(1100);

    rootBlock = new BlockWidget(this, nullptr, 0, 10); // 루트 노드
    rootBlock->show();
    
    setGeometry(10, y, 1100, rootBlock->height() + 20); //  초기엔 최소 높이로
}


BlockWidget* RootBlockArea::getRootBlock() const {
    return rootBlock;
}

int RootBlockArea::getTotalHeight() const {
    return std::max(rootBlock->getTotalHeight() + 20, BLOCK_HEIGHT + 20);

}

void RootBlockArea::updateSize() {
    setFixedSize(getTotalWidth(),getTotalHeight());
}

int RootBlockArea::calculateMaxDepth(BlockWidget* node) const {
    if (node->getChildren().isEmpty())
        return node->getCurrentDepth();

    int maxChildDepth = node->getCurrentDepth();
    for (auto* child : node->getChildren()) {
        maxChildDepth = std::max(maxChildDepth, calculateMaxDepth(child));
    }
    return maxChildDepth;
}

int RootBlockArea::getTotalWidth() const {
    return std::max(1100, rootBlock->getMaxRight() + 20);
}