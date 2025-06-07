#include "Ui/blockwidget.h"
#include "UIConstants.h"
#include <QFileDialog>

BlockWidget::BlockWidget(QWidget* canvas, BlockWidget* parent, int depth, int y)
    : QFrame(canvas), canvasRef(canvas), parentBlock(parent), currentDepth(depth) {

    // 블럭 디자인
    setFrameStyle(QFrame::NoFrame);
    setStyleSheet(R"(
        background-color: #1c1e26;
        border: 1px solid #2e3b55;
        border-radius: 14px;
        color: #dcdfe4;
        font-size: 12px;
        font-family: 'Segoe UI', sans-serif;
    )");
    setFixedSize(BLOCK_WIDTH, BLOCK_HEIGHT);

    int x = parent ? parent->x() + H_SPACING : 10;
    move(x, y);

    // 필터 박스 버튼
    filterTypeBox = new QComboBox(this); 
    filterTypeBox->setStyleSheet(R"(
        QComboBox {
            background-color: #2b2f40;
            color: #dcdfe4;
            border: 1px solid #3b4c6e;
            border-radius: 6px;
            padding-left: 6px;
        }
        QComboBox:hover {
            border-color: #3b78ff;
        }
        QComboBox::drop-down {
            border: none;
        }
    )");
    filterTypeBox->setGeometry(10, 10, 150, 20);
    filterTypeBox->addItems({ "EXTENSION", "KEYWORD", "DATE", "EXCEPTION", "SIZE" });
    connect(filterTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BlockWidget::onFilterTypeChanged);


    // condition값 설정텍스트박스
    conditionEdit = new QLineEdit(this);
    conditionEdit->setGeometry(10, 40, 120, 20);
    connect(conditionEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (logicBlock) logicBlock->setCondition(text.toStdString());
        });

    // 경로선택 버튼
    movePathBtn = new QPushButton("경로선택", this);
    movePathBtn->setGeometry(10, 70, 50, 20);
    connect(movePathBtn, &QPushButton::clicked, this, &BlockWidget::choosePath);

    // 경로 보여주는 곳
    movePathLabel = new QLabel("(미지정)", this);
    movePathLabel->setGeometry(65, 70, 95, 20);
    movePathLabel->setStyleSheet("color: lightgray;");

    // 이상 or 이하 이런거 정하는 콤보박스
    comparisonBox = new QComboBox(this);
    comparisonBox->setGeometry(130, 40, 40, 20);
    comparisonBox->addItems({ ">=", "<=", " >", " <", " =" });
    comparisonBox->hide();
    connect(comparisonBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (logicBlock)
            logicBlock->setComparisonType(static_cast<ComparisonType>(index));
        });

    // 자식블럭 추가 버튼
    addChildBtn = new QPushButton("조건추가", this);
    addChildBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2b2f40;
            color: #dcdfe4;
            border: 1px solid #3b4c6e;
            border-radius: 8px;
            padding: 4px 8px;
        }
        QPushButton:hover {
            background-color: #3b78ff;
            color: white;
        }
    )");

    addChildBtn->setGeometry(10, 95, 60, 20);
    connect(addChildBtn, &QPushButton::clicked, this, &BlockWidget::addChild);

}


void BlockWidget::addChild() {
    std::shared_ptr<Block> newLogicBlock = logicBlock->addEmptyChild();

    int newY = y() + nextChildY;
    auto* child = new BlockWidget(canvasRef, this, currentDepth + 1, newY);
    child->logicBlock = newLogicBlock;
    child->show();
    children.append(child);

    nextChildY += child->getTotalHeight() + V_SPACING;

    BlockWidget* root = this;
    while (root->parentBlock)
        root = root->parentBlock;

    // ✅ 루트 기준으로 부모 위젯인 RootBlockArea를 찾아서 updateSize() 호출
    if (auto* rootArea = qobject_cast<RootBlockArea*>(root->parentWidget())) {
        rootArea->updateSize();
    }

    root->relayoutChildren();
    emit root->resized();  // 루트 기준
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


void BlockWidget::setLogicBlock(const std::shared_ptr<Block>& block) {
    logicBlock = block;
    filterTypeBox->setCurrentIndex(static_cast<int>(block->getFilterType()));
    conditionEdit->setText(QString::fromStdString(block->getCondition()));
    movePathLabel->setText(QString::fromStdString(block->getMovePath()));
    comparisonBox->setCurrentIndex(static_cast<int>(block->getComparisonType()));
    comparisonBox->setVisible(block->getFilterType() == FilterType::SIZE);
}

void BlockWidget::choosePath() {
    QString path = QFileDialog::getExistingDirectory(this, "경로 선택", "");
    if (!path.isEmpty()) {
        movePathLabel->setText(path);
        if (logicBlock)
            logicBlock->setMovePath(path.toStdString());
    }
}

void BlockWidget::onFilterTypeChanged(int index) {
    if (!logicBlock) return;
    auto type = static_cast<FilterType>(index);
    logicBlock->setFilterType(type);
    comparisonBox->setVisible(type == FilterType::SIZE);
}