#pragma once

#include <QWidget>
#include <QFrame>
#include <QList>
#include <QMap>
#include <QStringList>

class QPushButton;
class QLabel;
class QScrollArea;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QLineEdit;

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

private slots:
    void addParentBlock();
    void saveSettings();
    void resetAll();
    void chooseLeftoverDir();

private:
    void setupUi();
    void createBlock(QFrame *parentFrame);

    QPushButton *btnSave;
    QPushButton *btnReset;
    QPushButton *btnAddParent;

    QScrollArea *blocksScroll;
    QWidget     *blocksContainer;
    QVBoxLayout *blocksLayout;

    QPushButton *btnLeftoverDir;
    QLabel      *lblLeftoverDir;

    QList<QFrame*>                parentBlocks;
    QMap<QFrame*, QList<QFrame*>> childrenMap;
    QMap<QFrame*, QFrame*>        parentMap;

    const QStringList allOptions = {"파일 이름", "확장자", "생성날짜"};

    QString targetDir;
    QString leftoverDir;
};
