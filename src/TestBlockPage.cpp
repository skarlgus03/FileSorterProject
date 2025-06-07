#include "Ui/testblockpage.h"

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
    topLayout->setContentsMargins(10, 10, 10, 10);
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
    auto* area = new RootBlockArea(canvas, nextRootY);
    area->show();
    rootAreas.append(area);
    static_cast<CanvasWidget*>(canvas)->addRootArea(area);

    connect(area->getRootBlock(), &BlockWidget::resized, this, [=]() {
        recalculateAllLayout();   // ✅ 여기
        });

    recalculateAllLayout();       // ✅ 부모 생성 직후도 반드시 호출
}

void TestBlockPage::recalculateAllLayout() {
    int y = 50;  // 버튼 영역 아래 여백
    for (RootBlockArea* area : rootAreas) {
        area->updateHeight();              // 높이 재계산 (BlockWidget 기반)
        area->move(10, y);                 // 위치 조정
        y += area->height() + 20;
    }
    nextRootY = y;
    canvas->update();                      // 선 갱신
    static_cast<CanvasWidget*>(canvas)->repaintAll();
}

