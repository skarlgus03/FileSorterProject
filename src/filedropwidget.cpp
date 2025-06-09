#include "filedropwidget.h"
#include "FileInfo.h"
#include "Ui/TestBlockPage.h"
#include "LogPage.h"

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

    // 아이콘, 글씨 크기
    fileList->setIconSize(QSize(32, 32));
    QFont font = fileList->font();
    font.setPointSize(12);
    fileList->setFont(font);

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
        QFileInfo qfileInfo(filePath);
        

        // icon
        QIcon fileIcon = iconProvider.icon(qfileInfo);

        FileInfo f;
        f.fileName = qfileInfo.fileName().toStdString();
        f.filePath = filePath.toStdString();
        f.extension = qfileInfo.suffix().toStdString();
        f.size = std::to_string(qfileInfo.size());
        f.date = qfileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss").toStdString();

        // 디버그용
        qDebug() << "파일 드롭됨:";
        qDebug() << "  fileName   =" << QString::fromStdString(f.fileName);
        qDebug() << "  filePath   =" << QString::fromStdString(f.filePath);
        qDebug() << "  extension  =" << QString::fromStdString(f.extension);
        qDebug() << "  size       =" << QString::fromStdString(f.size);
        qDebug() << "  date       =" << QString::fromStdString(f.date);

        droppedFiles.push_back(f);
        // 아이콘 + 파일이름 표시하게하기
        auto* item = new QListWidgetItem(fileIcon, QString::fromStdString(f.fileName));
        fileList->addItem(item);
        
        
    }
}

// 정리 하는 함수
void FileDropWidget::onOrganize()
{

    auto blocks = testBlockPage->getRootBlocks();
    if (blocks.empty()) {
        QMessageBox::warning(this, "정리 실패", "정리 기준 블럭이 없습니다.");
        return;
    }

    if (droppedFiles.empty()) {
        QMessageBox::warning(this, "정리 실패", "정리할 파일이 없습니다.");
        return;
    }

    for (auto& file : droppedFiles) {
        if (file.filePath.empty()) {
            qDebug() << "❌ filePath 비어 있음: " << QString::fromStdString(file.fileName);
            continue;
        }

        Classifier::classifyFile(file, blocks, nullptr);

        qDebug() << "  movePath  =" << QString::fromStdString(file.moveToPath);

        QString result = FileManager::moveFile(file);
        if (logPage) logPage->appendLog(result);
    }

    QMessageBox::information(this, "정리 완료", "파일이 정리되었습니다.");
    droppedFiles.clear();
    fileList->clear();
}


void FileDropWidget::onClear()
{
    fileList->clear();
}

void FileDropWidget::setLogPage(LogPage* page) {
    this->logPage = page;
}