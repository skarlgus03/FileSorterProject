#include "Ui/blockwidget.h"
#include "UIConstants.h"
#include <QFileDialog>


BlockWidget::BlockWidget(QWidget* canvas, BlockWidget* parent, int depth, int y)
    : QFrame(canvas), canvasRef(canvas), parentBlock(parent), currentDepth(depth) {

    QFont baseFont("Noto Sans KR", 11);
    this->setFont(baseFont);

    setFrameStyle(QFrame::NoFrame);
    setStyleSheet(R"(
        background-color: #1c1e26;
        border: 1px solid #2e3b55;
        border-radius: 14px;
        color: #dcdfe4;
        font-size: 12px;
        font-family: 'Noto Sans KR', sans-serif;
    )");
    setFixedSize(BLOCK_WIDTH, BLOCK_HEIGHT);

    int x = parent ? parent->x() + H_SPACING : 10;
    move(x, y);

    filterTypeBox = new QComboBox(this);
    filterTypeBox->setFont(baseFont);
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
    filterTypeBox->setGeometry(10, 10, 120, 20);
    filterTypeBox->addItems({ "EXTENSION", "KEYWORD", "DATE", "EXCEPTION", "SIZE(BYTE)" });
    connect(filterTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BlockWidget::onFilterTypeChanged);

    conditionEdit = new QLineEdit(this);
    conditionEdit->setFont(baseFont);
    conditionEdit->setGeometry(10, 40, 120, 20);
    connect(conditionEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (logicBlock) logicBlock->setCondition(text.toStdString());
        });

    movePathBtn = new QPushButton("경로선택", this);
    movePathBtn->setFont(baseFont);
    movePathBtn->setGeometry(10, 70, 50, 20);
    movePathBtn->setToolTip("말단 블럭에서만 경로를 설정할 수 있습니다.");
    connect(movePathBtn, &QPushButton::clicked, this, &BlockWidget::choosePath);

    movePathLabel = new QLabel("(미지정)", this);
    movePathLabel->setFont(baseFont);
    movePathLabel->setGeometry(65, 70, 95, 20);
    movePathLabel->setStyleSheet("color: lightgray;");
    movePathLabel->setToolTip("이 블럭이 말단일 때만 설정 가능합니다.");

    comparisonBox = new QComboBox(this);
    comparisonBox->setFont(baseFont);
    comparisonBox->setStyleSheet(R"(
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
    comparisonBox->setGeometry(131, 40, 40, 20);
    comparisonBox->addItems({ ">=", "<=", " >", " <", " =" });
    comparisonBox->hide();
    connect(comparisonBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (logicBlock)
            logicBlock->setComparisonType(static_cast<ComparisonType>(index));
        });

    addChildBtn = new QPushButton("＋", this);
    addChildBtn->setFont(baseFont);
    addChildBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2e3440;
            color: white;
            border: 1px solid #4c566a;
            border-radius: 4px;      
            min-width: 16px;
            min-height: 16px;
            max-width: 20px;
            max-height: 20px;
            padding: 0;
        }
        QPushButton:hover {
            background-color: #3b4252;
            border-color: #88c0d0;
        }
    )");

    addChildBtn->setGeometry(150, 70, 20, 20);
    connect(addChildBtn, &QPushButton::clicked, this, &BlockWidget::addChild);

    deleteBtn = new QPushButton("X",this);
    deleteBtn->setFont(baseFont);
    deleteBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2e3440;
            color: white;
            border: 1px solid #4c566a;
            border-radius: 4px;      
            min-width: 16px;
            min-height: 16px;
            max-width: 20px;
            max-height: 20px;
            padding: 0;
        }
        QPushButton:hover {
            background-color: #3b4252;
            border-color: #88c0d0;
        }
    )");
    deleteBtn->setGeometry(140, 10, 20, 20);
    connect(deleteBtn, &QPushButton::clicked, this, [=]() {emit requestDelete(this); });

    updateEnabledStates();
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

    if (auto* rootArea = qobject_cast<RootBlockArea*>(root->parentWidget())) {
        rootArea->updateSize();
    }

    root->relayoutChildren();
    emit root->resized();

    updateEnabledStates();
    for (auto* childWidget : children) {
        childWidget->updateEnabledStates();
    }
}

int BlockWidget::getTotalHeight() const {
    if (children.isEmpty())
        return height();

    int total = 0;
    for (const auto* child : children)
        total += child->getTotalHeight() + V_SPACING;

    total -= V_SPACING;
    return std::max(height(), total);
}

int BlockWidget::getMaxRight() const {
    int right = x() + width();
    for (const auto* child : children)
        right = std::max(right, child->getMaxRight());
    return right;
}

void BlockWidget::relayoutChildren() {
    int currentY = y();

    for (auto* child : children) {
        int childY = currentY;
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
    updateEnabledStates();
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
    updateEnabledStates();
}

void BlockWidget::updateEnabledStates() {
    bool isLeaf = logicBlock && logicBlock->isLeaf();
    movePathBtn->setEnabled(isLeaf);
    movePathLabel->setEnabled(isLeaf);

    movePathBtn->setStyleSheet(isLeaf ? R"(
        QPushButton {
            background-color: #2b2f40;
            color: #dcdfe4;
            border: 1px solid #3b4c6e;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #3b4252;
            border-color: #88c0d0;
        }
    )" : R"(
        QPushButton {
            background-color: #2e2e2e;
            color: gray;
            border: 1px solid #444;
            border-radius: 4px;
        }
    )");
}

void BlockWidget::removeChild(BlockWidget* child) {
    children.removeOne(child);
}