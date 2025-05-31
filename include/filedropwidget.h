// filedropwidget.h
#pragma once

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

class QDragEnterEvent;
class QDropEvent;

class FileDropWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileDropWidget(QWidget *parent = nullptr);

public slots:
    void setTargetDir(const QString &dir);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onOrganize();
    void onClear();

private:
    // UI 구성 요소
    QListWidget   *fileList;
    QWidget       *listContainer;
    QHBoxLayout   *listLayout;
    QString        targetDir;

    QPushButton   *organizeBtn;
    QPushButton   *clearBtn;
};
