// LogPage.cpp
#include "Ui/LogPage.h"
#include <QVBoxLayout>
#include <QDateTime>

LogPage::LogPage(QWidget* parent)
    : QWidget(parent), logEdit(new QPlainTextEdit(this))
{
    logEdit->setReadOnly(true);
    logEdit->setStyleSheet("background-color: #1e1e1e; color: white; font-family: Consolas; font-size: 12px;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(logEdit);
}

void LogPage::appendLog(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
    logEdit->appendPlainText(timestamp + message);
}
