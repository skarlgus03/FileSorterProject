// settingswidget.cpp
#include "settingswidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QScrollArea>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void SettingsWidget::setupUi()
{
    auto *mainL = new QVBoxLayout(this);
    mainL->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mainL->setSpacing(10);

    // 1) 상단 버튼
    {
        auto *topL = new QHBoxLayout;
        topL->setAlignment(Qt::AlignLeft);
        topL->setSpacing(10);

        btnSave      = new QPushButton("저장하기", this);
        btnReset     = new QPushButton("초기화", this);
        btnAddParent = new QPushButton("+ 부모 추가", this);
        btnSave->setFixedSize(100,30);
        btnReset->setFixedSize(100,30);
        btnAddParent->setFixedSize(100,30);

        topL->addWidget(btnSave);
        topL->addWidget(btnReset);
        topL->addWidget(btnAddParent);
        mainL->addLayout(topL);

        connect(btnSave,      &QPushButton::clicked, this, &SettingsWidget::saveSettings);
        connect(btnReset,     &QPushButton::clicked, this, &SettingsWidget::resetAll);
        connect(btnAddParent, &QPushButton::clicked, this, &SettingsWidget::addParentBlock);
    }

    // 2) 블록 스크롤 영역 (가로 전폭 채움)
    {
        blocksScroll = new QScrollArea(this);
        blocksScroll->setWidgetResizable(true);
        blocksScroll->setMinimumHeight(300);

        blocksContainer = new QWidget;
        blocksLayout    = new QVBoxLayout(blocksContainer);
        blocksLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        blocksLayout->setSpacing(5);

        blocksScroll->setWidget(blocksContainer);
        mainL->addWidget(blocksScroll);  // ← stretch=1, 정렬 플래그 제거
    }

    // 3) 하단: 남는 파일 지정 버튼을 오른쪽 끝에
    {
        auto *bottomL = new QHBoxLayout;
        bottomL->addStretch();

        btnLeftoverDir = new QPushButton("남는 파일 지정", this);
        btnLeftoverDir->setFixedSize(120,30);
        lblLeftoverDir = new QLabel("남는 파일: 없음", this);
        lblLeftoverDir->setFrameStyle(QFrame::Panel | QFrame::Sunken);

        bottomL->addWidget(btnLeftoverDir);
        bottomL->addWidget(lblLeftoverDir);
        mainL->addLayout(bottomL);

        connect(btnLeftoverDir, &QPushButton::clicked,
                this, &SettingsWidget::chooseLeftoverDir);
    }
}

// 재귀로 자식 총 개수 세기
static int countAllDescendants(
    const QMap<QFrame*, QList<QFrame*>> &map, QFrame *f)
{
    int cnt = map[f].size();
    for (auto *c : map[f])
        cnt += countAllDescendants(map, c);
    return cnt;
}

void SettingsWidget::addParentBlock()
{
    createBlock(nullptr);
}

void SettingsWidget::createBlock(QFrame *parentFrame)
{
    // 새 프레임 생성
    QFrame *frame = new QFrame(blocksContainer);
    frame->setFrameShape(QFrame::Box);
    frame->setStyleSheet("QFrame { background-color: #808080; border-radius: 5px; }");
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  // ← 가로 확장

    auto *hl = new QHBoxLayout(frame);
    hl->setAlignment(Qt::AlignLeft);
    hl->setSpacing(8);

    // 들여쓰기 + ㄴ 마커
    int depth = 0;
    QFrame *anc = parentFrame;
    while (anc) {
        depth++;
        anc = parentMap.value(anc, nullptr);
    }
    if (depth > 0) {
        hl->addSpacing(20 * depth);
        hl->addWidget(new QLabel("ㄴ", frame));
    }

    // 콤보박스
    auto *combo = new QComboBox(frame);
    combo->addItem("선택해 주세요");
    QSet<QString> used;
    anc = parentFrame;
    while (anc) {
        if (auto *c = anc->findChild<QComboBox*>())
            used.insert(c->currentText());
        anc = parentMap.value(anc, nullptr);
    }
    for (auto &opt : allOptions)
        if (!used.contains(opt))
            combo->addItem(opt);
    hl->addWidget(combo, 0, Qt::AlignLeft);

    // 파일 이름 텍스트 필터
    auto *edit = new QLineEdit(frame);
    edit->setPlaceholderText("텍스트 필터");
    edit->setFixedSize(150,30);
    edit->setVisible(false);
    edit->setAlignment(Qt::AlignLeft);
    hl->addWidget(edit, 0, Qt::AlignLeft);

    // 확장자 콤보
    auto *extCombo = new QComboBox(frame);
    extCombo->addItems({"pdf","txt","ppt","hwp"});
    extCombo->setFixedSize(80,30);
    extCombo->setVisible(false);
    hl->addWidget(extCombo, 0, Qt::AlignLeft);

    // 날짜 범위 입력
    auto *fromEdit = new QLineEdit(frame);
    fromEdit->setPlaceholderText("YYYY-MM-DD");
    fromEdit->setFixedSize(120,30);
    fromEdit->setVisible(false);
    fromEdit->setAlignment(Qt::AlignLeft);
    hl->addWidget(fromEdit, 0, Qt::AlignLeft);

    auto *toEdit = new QLineEdit(frame);
    toEdit->setPlaceholderText("YYYY-MM-DD");
    toEdit->setFixedSize(120,30);
    toEdit->setVisible(false);
    toEdit->setAlignment(Qt::AlignLeft);
    hl->addWidget(toEdit, 0, Qt::AlignLeft);

    // 파일 지정 버튼
    QPushButton *fileBtn = new QPushButton("파일 지정", frame);
    fileBtn->setObjectName("fileBtn");
    fileBtn->setFixedSize(80,30);
    hl->addWidget(fileBtn, 0, Qt::AlignLeft);

    // + 자식 버튼
    QPushButton *addChildBtn = new QPushButton("+ 자식", frame);
    addChildBtn->setFixedSize(80,30);
    hl->addWidget(addChildBtn, 0, Qt::AlignLeft);

    // 삭제 버튼
    QPushButton *delBtn = new QPushButton("x", frame);
    delBtn->setFixedSize(20,20);
    hl->addWidget(delBtn, 0, Qt::AlignLeft);

    // blocksLayout에 추가
    if (parentFrame) {
        int idx    = blocksLayout->indexOf(parentFrame);
        int offset = countAllDescendants(childrenMap, parentFrame);
        blocksLayout->insertWidget(idx + offset + 1, frame);
    } else {
        blocksLayout->addWidget(frame);
    }

    // 맵 갱신
    childrenMap[frame] = {};
    if (parentFrame) {
        parentMap[frame] = parentFrame;
        childrenMap[parentFrame].append(frame);
        if (auto *pBtn = parentFrame->findChild<QPushButton*>("fileBtn"))
            pBtn->setEnabled(false);
    } else {
        parentBlocks.append(frame);
    }

    // 콤보박스 변경
    connect(combo, &QComboBox::currentTextChanged, this,
            [=](const QString &txt){
                edit    ->setVisible(txt == "파일 이름");
                extCombo->setVisible(txt == "확장자");
                bool isDate = (txt == "생성날짜");
                fromEdit->setVisible(isDate);
                toEdit  ->setVisible(isDate);
            });

    // 파일 지정
    connect(fileBtn, &QPushButton::clicked, this, [=](){
        QString d = QFileDialog::getExistingDirectory(this, "대상 폴더 선택");
        if (!d.isEmpty()) {
            targetDir = d;
            fileBtn->setText("☑");
        }
    });

    // + 자식
    connect(addChildBtn, &QPushButton::clicked, this, [=](){
        createBlock(frame);
    });

    // 삭제
    connect(delBtn, &QPushButton::clicked, this, [=](){
        for (auto *c : childrenMap[frame]) {
            blocksLayout->removeWidget(c);
            c->deleteLater();
        }
        childrenMap.remove(frame);
        parentMap.remove(frame);

        if (parentFrame) {
            childrenMap[parentFrame].removeAll(frame);
            if (childrenMap[parentFrame].isEmpty()) {
                if (auto *pBtn = parentFrame->findChild<QPushButton*>("fileBtn"))
                    pBtn->setEnabled(true);
            }
        } else {
            parentBlocks.removeAll(frame);
        }

        blocksLayout->removeWidget(frame);
        frame->deleteLater();
    });
}

void SettingsWidget::saveSettings()
{
    QMessageBox::information(this, "저장", "설정이 저장되었습니다.");
}

void SettingsWidget::resetAll()
{
    for (auto *p : parentBlocks) {
        for (auto *c : childrenMap[p]) {
            blocksLayout->removeWidget(c);
            c->deleteLater();
        }
        blocksLayout->removeWidget(p);
        p->deleteLater();
    }
    parentBlocks.clear();
    childrenMap.clear();
    parentMap.clear();
    lblLeftoverDir->setText("남는 파일: 없음");
}

void SettingsWidget::chooseLeftoverDir()
{
    QString d = QFileDialog::getExistingDirectory(this, "남는 파일 대상 지정");
    if (!d.isEmpty()) {
        leftoverDir = d;
        lblLeftoverDir->setText("남는 파일: " + d);
    }
}
