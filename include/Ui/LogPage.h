// LogPage.h
#pragma once

#include <QWidget>
#include <QPlainTextEdit>

class LogPage : public QWidget {
    Q_OBJECT

public:
    explicit LogPage(QWidget* parent = nullptr);
    void appendLog(const QString& message);

private:
    QPlainTextEdit* logEdit;
};
