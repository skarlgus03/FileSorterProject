#pragma once

#include <QMainWindow>
#include <QStackedWidget>

class TestBlockPage;
class FileDropWidget;
class SettingsWidget;
class FileViewWidget;
class LogPage;
class HelpPage;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void tryChangePage(int targetIndex);
    ~MainWindow();

private:
    Ui::MainWindow *ui;  // Designer .ui를 쓰지 않으면 nullptr 유지

    FileDropWidget *fileDropWidget;
    HelpPage* helpPage;
    FileViewWidget *fileViewWidget;
    TestBlockPage* testBlockPage;
    LogPage* logPage;

    QStackedWidget* stack;
};
