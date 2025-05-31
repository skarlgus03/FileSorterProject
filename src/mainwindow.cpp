// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filedropwidget.h"
#include "settingswidget.h"
#include "fileviewwidget.h"

#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1) Create pages
    fileDropWidget   = new FileDropWidget(this);
    settingsWidget   = new SettingsWidget(this);
    fileViewWidget   = new FileViewWidget(this);

    stack = new QStackedWidget(this);
    stack->addWidget(fileDropWidget);
    stack->addWidget(settingsWidget);
    stack->addWidget(fileViewWidget);

    // 2) Left menu
    QWidget *leftMenu = new QWidget(this);
    QVBoxLayout *menuL = new QVBoxLayout(leftMenu);
    menuL->setAlignment(Qt::AlignTop);
    QStringList titles = { "정리 창", "설정 창", "파일 창" };
    for (int i = 0; i < titles.size(); ++i) {
        QPushButton *btn = new QPushButton(titles[i], leftMenu);
        btn->setFixedSize(120, 40);
        menuL->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, i]() {
            stack->setCurrentIndex(i);
        });
    }
    menuL->addStretch();

    // 3) Splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftMenu);
    splitter->addWidget(stack);
    splitter->setStretchFactor(1, 1);
    splitter->setStyleSheet("QSplitter::handle { background-color: lightgray; width: 2px; }");

    // 4) Place into centralwidget
    auto *mainL = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (!mainL) {
        mainL = new QVBoxLayout(ui->centralwidget);
        mainL->setContentsMargins(0,0,0,0);
    }
    mainL->addWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
