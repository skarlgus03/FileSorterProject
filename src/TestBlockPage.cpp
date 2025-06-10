#include "Ui/testblockpage.h"
#include "styles/StyleSheet.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QFileDialog>

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
    topLayout->setSpacing(10);


    // 버튼 생성
    createRootBtn = new QPushButton("부모생성", topBar);
    createRootBtn->setFixedSize(100, 30);
    createRootBtn->setStyleSheet(defaultButtonStyle());
    
    exceptionBtn = new QPushButton("예외 경로 선택");
    exceptionBtn->setStyleSheet(defaultButtonStyle());
    exceptionBtn->setFixedSize(100, 30);

    exceptionPathLabel = new QLabel("경로 미설정");
    exceptionPathLabel->setStyleSheet("color: gray;");
    exceptionPathLabel->setFixedWidth(250);
    exceptionPathLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    //중간 텍스트 생략처리
    QString shortened = QFontMetrics(exceptionPathLabel->font())
        .elidedText(exceptionPath, Qt::ElideMiddle, 250);
    exceptionPathLabel->setText(shortened);

    settingSaveBtn = new QPushButton("설정 저장");
    settingSaveBtn->setStyleSheet(defaultButtonStyle());
    settingSaveBtn->setFixedSize(100, 30);

    topLayout->addWidget(createRootBtn);
    topLayout->addWidget(exceptionBtn);
    topLayout->addWidget(exceptionPathLabel);
    topLayout->addWidget(settingSaveBtn);

    mainLayout->addWidget(topBar);



    // 캔버스 + 스크롤 영역
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    canvas = new CanvasWidget(this);
    canvas->setMinimumSize(2000, 2000);
    scrollArea->setWidget(canvas);

    mainLayout->addWidget(scrollArea);

    // 버튼 연결
    connect(createRootBtn, &QPushButton::clicked, this, &TestBlockPage::createRootBlock);
    connect(exceptionBtn, &QPushButton::clicked, this, [=]() {
        QString dir = QFileDialog::getExistingDirectory(this, "예외 경로 선택");
        if (!dir.isEmpty()) {
            exceptionPath = dir;
            exceptionPathLabel->setText(dir);  //  표시 갱신
            exceptionPathLabel->setStyleSheet("color: white;");
        }
    });

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

    // 자식 있으면 삭제 금지
    if (!widget->getChildren().isEmpty()) {
        QMessageBox::warning(this, "삭제 불가", "자식이 있는 블럭은 삭제할 수 없습니다.");
        return;
    }

    // 루트 블럭이면: RootBlockArea 제거
    for (int i = 0; i < rootAreas.size(); ++i) {
        if (rootAreas[i]->getRootBlock() == widget) {
            auto* area = rootAreas[i];
            rootAreas.removeAt(i);
            static_cast<CanvasWidget*>(canvas)->removeRootArea(area);
            area->deleteLater();

            rootLogicBlocks.erase(
                std::remove(rootLogicBlocks.begin(), rootLogicBlocks.end(), widget->getBlock()),
                rootLogicBlocks.end()
            );

            recalculateAllLayout();
            QTimer::singleShot(0, canvas, SLOT(repaintAll()));
            return;
        }
    }

    // 루트가 아니면 → self 삭제
    widget->performSelfDelete();
}



void TestBlockPage::removeRootBlockArea(RootBlockArea* area) {
    if (!area) return;
    rootAreas.removeOne(area);
    area->deleteLater();
    recalculateAllLayout(); // 레이아웃 재정렬
    canvas->repaintAll();   // 연결선 다시 그림
}