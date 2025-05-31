// filedropwidget.cpp
#include "filedropwidget.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>

FileDropWidget::FileDropWidget(QWidget *parent)
    : QWidget(parent)
    , targetDir()
{
    setAcceptDrops(true);

    // 1) 드롭된 파일 태그(프레임) 표시용 컨테이너
    listContainer = new QWidget(this);
    listLayout    = new QHBoxLayout(listContainer);
    listLayout->setAlignment(Qt::AlignLeft);
    listLayout->setSpacing(5);

    // 2) 전체 레이아웃
    auto *mainL = new QVBoxLayout(this);
    mainL->addWidget(listContainer);

    // 3) 정리/비우기 버튼
    QWidget *btnBox = new QWidget(this);
    QHBoxLayout *btnL = new QHBoxLayout(btnBox);
    btnL->setAlignment(Qt::AlignLeft);
    btnL->setSpacing(10);

    organizeBtn = new QPushButton("정리하기", btnBox);
    clearBtn    = new QPushButton("비우기", btnBox);
    organizeBtn->setFixedSize(100,30);
    clearBtn   ->setFixedSize(100,30);
    btnL->addWidget(organizeBtn);
    btnL->addWidget(clearBtn);

    mainL->addWidget(btnBox);

    // 4) 파일 정보 리스트뷰
    fileList = new QListWidget(this);
    mainL->addWidget(fileList);

    // 슬롯 연결
    connect(clearBtn,    &QPushButton::clicked, this, &FileDropWidget::onClear);
    connect(organizeBtn, &QPushButton::clicked, this, &FileDropWidget::onOrganize);
}

void FileDropWidget::setTargetDir(const QString &dir) {
    targetDir = dir;
}

void FileDropWidget::onClear() {
    // 리스트 뷰 비우기
    fileList->clear();
    // 태그 프레임도 모두 제거
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}

void FileDropWidget::onOrganize() {
    if (targetDir.isEmpty()) {
        QMessageBox::warning(this, "경고", "경로를 설정해주세요");
        return;
    }
    // TODO: 분류 로직 호출
    qDebug() << "분류 실행, 대상 폴더:" << targetDir;
}

void FileDropWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void FileDropWidget::dropEvent(QDropEvent *event) {
    for (const QUrl &url : event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        QFileInfo info(filePath);

        // 1) 리스트뷰에도 추가
        QString detail = QString("%1 | %2 bytes | %3 | %4")
                             .arg(info.fileName())
                             .arg(info.size())
                             .arg(info.lastModified().toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(info.suffix());
        fileList->addItem(detail);

        // 2) 왼쪽 태그 프레임으로도 추가
        QFrame *frame = new QFrame(listContainer);
        frame->setFrameShape(QFrame::Box);
        frame->setStyleSheet("QFrame { background: #f0f0f0; }");
        QHBoxLayout *lay = new QHBoxLayout(frame);
        lay->setContentsMargins(5,2,5,2);

        QLabel *lbl = new QLabel(info.fileName(), frame);
        QPushButton *btnX = new QPushButton("x", frame);
        btnX->setFixedSize(16,16);
        lay->addWidget(lbl);
        lay->addWidget(btnX);

        connect(btnX, &QPushButton::clicked, frame, &QFrame::deleteLater);

        listLayout->addWidget(frame);
    }
    event->acceptProposedAction();
}
