#include "Ui/blockwidget.h"
#include <QPushButton>
#include <QLabel>

BlockWidget::BlockWidget(QWidget* canvas, BlockWidget* parent, int depth, int y)
    : QFrame(canvas), canvasRef(canvas), parentBlock(parent), currentDepth(depth) {


    setFrameStyle(QFrame::Box);
    setFixedSize(BLOCK_WIDTH, BLOCK_HEIGHT); 


    QString bgColor;
    switch (depth) {
    case 0: bgColor = "#1f1f28"; break;
    case 1: bgColor = "#2a2f3a"; break;
    case 2: bgColor = "#35404d"; break;
    case 3: bgColor = "#3f4f61"; break;
    case 4: bgColor = "#4c5d73"; break;
    default: bgColor = "#5b6d86"; break;
    }

    setStyleSheet(QString(
        "background-color: %1;"
        "border: 1px solid #dddddd;"
        "border-radius: 6px;"
        "color: white;"
    ).arg(bgColor));

    int x = parent ? parent->x() + H_SPACING : 10;
    move(x, y);

    addChildBtn = new QPushButton("조건추가", this);
    addChildBtn->setGeometry(10, 10, 60, 25);
    connect(addChildBtn, &QPushButton::clicked, this, &BlockWidget::addChild);

    label = new QLabel(QString("depth %1").arg(depth), this);
    label->setStyleSheet("color: lightblue;");
    label->move(10, 45);
}

void BlockWidget::addChild() {
    
    
    int newY = y() + nextChildY;
    auto* child = new BlockWidget(canvasRef, this, currentDepth + 1, newY);
    child->show();
    children.append(child);

    nextChildY += child->getTotalHeight() + V_SPACING;

    BlockWidget* root = this;
    while (root->parentBlock)
        root = root->parentBlock;

    root->relayoutChildren();
    emit root->resized(); // 루트 기준
}

int BlockWidget::getTotalHeight() const {
    if (children.isEmpty())
        return height();

    int total = 0;
    for (const auto* child : children)
        total += child->getTotalHeight() + V_SPACING;

    total -= V_SPACING; // 마지막 여백 제거
    return std::max(height(), total);
}


void BlockWidget::relayoutChildren() {
    int currentY = y();

    for (auto* child : children) {
        int childY = currentY;

        // 블럭 하나일 때는 자기 위치 기준
        if (children.size() == 1)
            childY = y();

        child->move(x() + H_SPACING, childY);
        currentY += child->getTotalHeight() + V_SPACING;

        child->relayoutChildren();
    }
}

void BlockWidget::updateLayoutFromChildGrowth() {
    nextChildY = 0;
    for (auto* child : children)
        nextChildY += child->getTotalHeight() + V_SPACING;

    const auto& siblings = parentBlock ? parentBlock->children : QVector<BlockWidget*>();
    int index = parentBlock ? siblings.indexOf(this) : -1;

    if (parentBlock) {
        int y = parentBlock->y() + parentBlock->nextChildY;

        for (int i = index + 1; i < siblings.size(); ++i) {
            siblings[i]->move(siblings[i]->x(), y);
            y += siblings[i]->getTotalHeight() + V_SPACING;
            siblings[i]->relayoutChildren();
        }
        parentBlock->updateLayoutFromChildGrowth();
    }
}

QVector<BlockWidget*> BlockWidget::getChildren() const {
    return children;
}
