#pragma once

#include <QWidget>

class QListWidget;
class QListWidgetItem;

class FileViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileViewWidget(QWidget *parent = nullptr);

signals:
    void fileSelected(const QString &filePath);

public slots:
    void addFilePath(const QString &filePath);      //경로받아 리스트에 추가

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);        //ui에 표시되게

private:
    QListWidget *previewList;
};
