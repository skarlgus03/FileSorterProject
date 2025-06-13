#include "Ui/testblockpage.h"
#include "styles/StyleSheet.h"
#include "JsonManager.h"
#include "BlockErrorDetector.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QCoreApplication>
#include <QSettings>

TestBlockPage::TestBlockPage(QWidget* parent)
    : QWidget(parent)
{
    canvas = new CanvasWidget(this);
    scrollArea = new QScrollArea(this);

    QSettings settings("Maengo", "FileSorter");
    QString lastPath = settings.value("lastUsedSetting").toString();

    qDebug() << "[최근 설정 경로]" << lastPath;
    if (!lastPath.isEmpty() && JsonManager::isFileExist(lastPath)) {
        auto loadedRoots = JsonManager::loadAllFromJson(lastPath);
        for (const auto& block : loadedRoots) {
            addRootBlock(block);
        }
    }

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);

    // 상단 버튼 바
    auto* topBar = new QWidget(this);
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(0, 0, 0, 0);
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
    exceptionPathLabel->setFixedWidth(270);
    exceptionPathLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    //중간 텍스트 생략처리
    QString shortened = QFontMetrics(exceptionPathLabel->font())
        .elidedText(exceptionPath, Qt::ElideMiddle, 270);
    exceptionPathLabel->setText(shortened);

    settingSaveBtn = new QPushButton("설정 저장");
    settingSaveBtn->setStyleSheet(defaultButtonStyle());
    settingSaveBtn->setFixedSize(100, 30);

    settingLoadBtn = new QPushButton("설정 불러오기");
    settingLoadBtn->setStyleSheet(defaultButtonStyle());
    settingLoadBtn->setFixedSize(100, 30);

    topLayout->addWidget(createRootBtn);
    topLayout->addWidget(exceptionBtn);
    topLayout->addWidget(exceptionPathLabel);
    topLayout->addWidget(settingSaveBtn);
    topLayout->addWidget(settingLoadBtn);

    mainLayout->addWidget(topBar);



    // 캔버스 + 스크롤 영역
    
    scrollArea->setWidgetResizable(true);

    
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
    connect(settingSaveBtn, &QPushButton::clicked, this, &TestBlockPage::onSaveJsonToSettings);
    connect(settingLoadBtn, &QPushButton::clicked, this, &TestBlockPage::onLoadJsonFromSettings);

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


// 블럭에어리아 제거
void TestBlockPage::removeRootBlockArea(RootBlockArea* area) {
    if (!area) return;
    rootAreas.removeOne(area);
    area->deleteLater();
    recalculateAllLayout(); // 레이아웃 재정렬
    canvas->repaintAll();   // 연결선 다시 그림
    QTimer::singleShot(0, canvas, SLOT(update()));
}

void TestBlockPage::addRootBlock(const std::shared_ptr<Block>& rootBlock) {
    if (!canvas) {
        qWarning() << "[오류] canvas가 nullptr입니다!";
        return;
    }
    rootLogicBlocks.push_back(rootBlock);

    // 1. RootBlockArea 생성
    auto* area = new RootBlockArea(canvas, nextRootY);
    area->show();
    rootAreas.append(area);
    // 소멸되면 리스트에서 제거
    connect(area, &QObject::destroyed, this, [=]() {
        rootAreas.removeOne(area);
    });

    static_cast<CanvasWidget*>(canvas)->addRootArea(area);

    BlockWidget* rootBlockWidget = area->getRootBlock();
    rootBlockWidget->setLogicBlock(rootBlock);
    rootBlockWidget->buildSubtreeFromLogic(); 
    // 3. 신호 연결
    connect(rootBlockWidget, &BlockWidget::resized, this, [=]() {
        recalculateAllLayout();
        area->updateSize();
        });

    connect(rootBlockWidget, &BlockWidget::requestDelete, this, [=](BlockWidget* self) {
        onDeleteBlock(self);
        });

    recalculateAllLayout();
}

// 조건 검사  scan함수 호출
bool TestBlockPage::validate() {
    auto roots = getRootBlocks();
    QStringList errors;

    for (const auto& root : roots) {
        if (!root) continue;

        QStringList partial = BlockErrorDetector::scan(root);
        errors += partial;  
    }

    if (!errors.isEmpty()) {
        QString msg = errors.join("\n");  
        QMessageBox::warning(this, "블록 오류", msg);
        return false;
    }

    return true;
}
// Json 저장 버튼 누르면 나오는 창
void TestBlockPage::onSaveJsonToSettings() {
    QString fileName = QInputDialog::getText(this, "설정 저장", "파일 이름 입력 (확장자 생략):");
    if (fileName.isEmpty()) return;

    QString path = QDir(QCoreApplication::applicationDirPath() + "/settings").filePath(fileName + ".json");
    QDir().mkpath(QFileInfo(path).absolutePath());

    JsonManager::saveAllToJson(rootLogicBlocks, path);
    QMessageBox::information(this, "저장 완료", "설정이 저장되었습니다:\n" + path);

    QSettings settings("Maengo", "FileSorter");
    settings.setValue("lastUsedSetting", path);
}
// Json 불러오기 버튼 누르면 나오는 창
void TestBlockPage::onLoadJsonFromSettings() {
    QDir settingsDir(QCoreApplication::applicationDirPath() + "/settings");
    QStringList fileList = settingsDir.entryList(QStringList() << "*.json", QDir::Files);

    if (fileList.isEmpty()) {
        QMessageBox::information(this, "파일 없음", "settings 폴더에 저장된 설정 파일이 없습니다.");
        return;
    }

    bool ok = false;
    QString selectedFile = QInputDialog::getItem(this, "설정 선택", "불러올 설정 파일을 선택하세요:", fileList, 0, false, &ok);
    if (!ok || selectedFile.isEmpty()) return;

    QString path = settingsDir.filePath(selectedFile);

    // 기존 블럭 정리
    for (auto* area : rootAreas) area->deleteLater();
    rootAreas.clear();
    rootLogicBlocks.clear();

    // 불러오기
    auto loadedRoots = JsonManager::loadAllFromJson(path);
    for (const auto& block : loadedRoots) {
        addRootBlock(block);
    }
    QSettings settings("Maengo", "FileSorter");
    settings.setValue("lastUsedSetting", path);
}
