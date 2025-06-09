#pragma once

#include <QWidget>
#include <QFileIconProvider>
#include "FileInfo.h"
#include "Classifier.h"
#include "FileManager.h"
#include <vector>

class QListWidget;
class QPushButton;
class QDragEnterEvent;
class QDropEvent;
class TestBlockPage;
class LogPage;

class FileDropWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileDropWidget(QWidget *parent = nullptr);
    void setTestBlockPage(TestBlockPage* page) { testBlockPage = page; }
    void setLogPage(LogPage* page);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onOrganize();
    void onClear();

private:
    TestBlockPage* testBlockPage = nullptr;
    LogPage* logPage = nullptr;
    QListWidget *fileList;
    QString      targetDir;
    QFileIconProvider iconProvider;
    std::vector<FileInfo> droppedFiles;
};
