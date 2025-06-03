#pragma once

#include <QMainWindow>

class FileDropWidget;
class SettingsWidget;
class FileViewWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;  // Designer .ui를 쓰지 않으면 nullptr 유지

    FileDropWidget *fileDropWidget;
    SettingsWidget *settingsWidget;
    FileViewWidget *fileViewWidget;
};
