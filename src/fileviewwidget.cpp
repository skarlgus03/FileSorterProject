#include "fileviewwidget.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QUrl>

FileViewWidget::FileViewWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(10);

    previewList = new QListWidget(this);
    layout->addWidget(previewList);

    connect(previewList, &QListWidget::itemDoubleClicked,
            this, &FileViewWidget::onItemDoubleClicked);
}

void FileViewWidget::addFilePath(const QString &filePath)
{
    previewList->addItem(filePath);
}

void FileViewWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    QString path = item->text();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
