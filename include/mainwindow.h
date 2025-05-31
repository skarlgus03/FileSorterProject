// mainwindow.h
#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FileDropWidget;
class SettingsWidget;
class FileViewWidget;
class QStackedWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow    *ui;
    QStackedWidget    *stack;
    FileDropWidget    *fileDropWidget;
    SettingsWidget    *settingsWidget;
    FileViewWidget    *fileViewWidget;
};
