#include "filedropwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>

FileDropWidget::FileDropWidget(QWidget* parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5); // ✅ 여백 맞춤
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->setSpacing(10);

    {
        auto* h = new QHBoxLayout;
        h->setContentsMargins(0, 0, 0, 0); // ✅ 내부 마진 제거
        h->setAlignment(Qt::AlignLeft);
        h->setSpacing(10);

        QPushButton* organizeBtn = new QPushButton("정리하기", this);
        organizeBtn->setFixedSize(100, 30);
        h->addWidget(organizeBtn);

        QPushButton* clearBtn = new QPushButton("비우기", this);
        clearBtn->setFixedSize(100, 30);
        h->addWidget(clearBtn);

        layout->addLayout(h);

        connect(organizeBtn, &QPushButton::clicked, this, &FileDropWidget::onOrganize);
        connect(clearBtn, &QPushButton::clicked, this, &FileDropWidget::onClear);
    }

    fileList = new QListWidget(this);
    layout->addWidget(fileList);
}

void FileDropWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void FileDropWidget::dropEvent(QDropEvent* event)
{
    const auto urls = event->mimeData()->urls();
    for (const QUrl& url : urls) {
        const QString filePath = url.toLocalFile();
        QFileInfo info(filePath);

        QString fileDetail = QString("파일명: %1 | 크기: %2 bytes | 수정일: %3 | 확장자: %4")
            .arg(info.fileName())
            .arg(info.size())
            .arg(info.lastModified().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(info.suffix());

        fileList->addItem(fileDetail);
    }
}

void FileDropWidget::onOrganize()
{
    if (targetDir.isEmpty()) {
        QMessageBox::warning(this, "경로 미설정", "먼저 설정 창에서 대상 경로를 지정해주세요.");
        return;
    }
    QMessageBox::information(this, "정리", "파일이 정리되었습니다. (로직 미구현)");
}

void FileDropWidget::onClear()
{
    fileList->clear();
}