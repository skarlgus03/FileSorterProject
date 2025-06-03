#pragma once

#include <QWidget>

class QListWidget;
class QPushButton;
class QDragEnterEvent;
class QDropEvent;

class FileDropWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileDropWidget(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onOrganize();
    void onClear();

private:
    QListWidget *fileList;
    QString      targetDir;
};
