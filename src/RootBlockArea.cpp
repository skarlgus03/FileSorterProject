#include "Ui/RootBlockArea.h"
#include <algorithm>

RootBlockArea::RootBlockArea(QWidget* parent, int y)
    : QFrame(parent) {
    move(10, y);
    setFrameStyle(QFrame::Box);
    setStyleSheet("border: 1px solid white;");
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

void RootBlockArea::updateHeight() {
    setFixedHeight(getTotalHeight());
}

