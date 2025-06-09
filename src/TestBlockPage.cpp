#include "Ui/testblockpage.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>

TestBlockPage::TestBlockPage(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 상단 버튼 바
    auto* topBar = new QWidget(this);
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(10, 5, 10, 5);
    topLayout->setAlignment(Qt::AlignLeft);

    btnCreateRoot = new QPushButton("부모생성", topBar);
    btnCreateRoot->setFixedSize(100, 30);
    topLayout->addWidget(btnCreateRoot);

    mainLayout->addWidget(topBar);

    // 캔버스 + 스크롤 영역
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    canvas = new CanvasWidget(this);
    canvas->setMinimumSize(2000, 2000);
    scrollArea->setWidget(canvas);

    mainLayout->addWidget(scrollArea);

    connect(btnCreateRoot, &QPushButton::clicked, this, &TestBlockPage::createRootBlock);
}

void TestBlockPage::createRootBlock() {
    // 1. 루트 논리 노드 생성
    std::shared_ptr<Block> rootLogicBlock = std::make_shared<Block>();
    rootLogicBlocks.push_back(rootLogicBlock);

    // 2. 루트 영역 UI 생성
    auto* area = new RootBlockArea(canvas, nextRootY);
    area->show();
    rootAreas.append(area);
    static_cast<CanvasWidget*>(canvas)->addRootArea(area);

    // 3. 내부 BlockWidget 가져와서 연결
    BlockWidget* rootBlockWidget = area->getRootBlock();
    rootBlockWidget->setLogicBlock(rootLogicBlock);

    // 4. 레이아웃 및 시그널 연결
    connect(rootBlockWidget, &BlockWidget::resized, this, [=]() {
        recalculateAllLayout();
        area->updateSize();
        });

    connect(rootBlockWidget, &BlockWidget::requestDelete, this, [=](BlockWidget* self) {
        onDeleteBlock(self);
        });

    recalculateAllLayout();
}


void TestBlockPage::recalculateAllLayout() {
    int y = 10;  // 버튼 영역 아래 여백
    for (RootBlockArea* area : rootAreas) {
        area->updateSize();              //사이즈 조정
        area->move(10, y);                 // 위치 조정
        y += area->height() + 20;
    }
    nextRootY = y;
    canvas->update();                      // 선 갱신
    static_cast<CanvasWidget*>(canvas)->repaintAll();
}

void TestBlockPage::onDeleteBlock(BlockWidget* widget) {
    if (!widget) return;

    std::shared_ptr<Block> logicBlock = widget->getBlock();
    if (!logicBlock) return;

    if (QMessageBox::question(this, "삭제 확인", "이 블럭과 모든 자식을 삭제하시겠습니까?") != QMessageBox::Yes)
        return;

    // 1. UI 제거 먼저
    if (widget->getParentBlock()) {
        BlockWidget* parentWidget = widget->getParentBlock();
        parentWidget->removeChild(widget);
        widget->setParent(nullptr);    // 연결 해제
        widget->hide();
        widget->deleteLater();
    }
    else {
        for (int i = 0; i < rootAreas.size(); ++i) {
            RootBlockArea* area = rootAreas[i];
            if (area->getRootBlock() == widget) {
                area->hide();
                area->setParent(nullptr);
                area->deleteLater();
                rootAreas.removeAt(i);
                break;
            }
        }
    }

    // 2. 논리 블럭 구조에서 제거
    if (auto parent = logicBlock->getParent().lock()) {
        auto& siblings = parent->getChildrenMutable();
        siblings.erase(std::remove(siblings.begin(), siblings.end(), logicBlock), siblings.end());
    }
    else {
        rootLogicBlocks.erase(
            std::remove(rootLogicBlocks.begin(), rootLogicBlocks.end(), logicBlock),
            rootLogicBlocks.end());
    }

    recalculateAllLayout();
    canvas->repaintAll();
}


void TestBlockPage::removeRootBlockArea(RootBlockArea* area) {
    if (!area) return;
    rootAreas.removeOne(area);
    area->deleteLater();
    recalculateAllLayout(); // 레이아웃 재정렬
    canvas->repaintAll();   // 연결선 다시 그림
}