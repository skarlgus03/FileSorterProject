#include "Ui/mainwindow.h"
#include "Ui/filedropwidget.h"
#include "Ui/TestBlockPage.h"
#include "Ui/LogPage.h"
#include "Ui/HelpPage.h"
#include "styles/stylesheet.h"

#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr) // Designer UI가 없으므로 nullptr
{
    // 중앙 위젯 설정
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    central->setStyleSheet("background-color: #212121;");

    auto *mainL = new QVBoxLayout(central);
    mainL->setContentsMargins(0,0,0,0);
    mainL->setSpacing(0);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, central);
    splitter->setStyleSheet("QSplitter::handle { background-color: #112240; width: 2px; }");

    // 왼쪽 메뉴
    QWidget *leftMenu = new QWidget;
    leftMenu->setStyleSheet(R"(
        background-color: #1a1a1a;
    )");
    QVBoxLayout *menuLayout = new QVBoxLayout(leftMenu);
    menuLayout->setContentsMargins(10, 10, 10, 10);         //여백을 얼마나 간격을 할것인가
    menuLayout->setSpacing(20);          //위 간격 함수
    menuLayout->setAlignment(Qt::AlignTop);     //배치설정

    //버튼 생성
    QPushButton *btnPage1 = new QPushButton("정리 창", leftMenu);
    QPushButton *btnPage2 = new QPushButton("블록 설정", leftMenu);
    QPushButton *btnPage3 = new QPushButton("로그 창", leftMenu);
    QPushButton* btnPage4 = new QPushButton("도움말", leftMenu);

    QSize buttonSize(120, 40);
    btnPage1->setFixedSize(buttonSize);
    btnPage2->setFixedSize(buttonSize);
    btnPage3->setFixedSize(buttonSize);
    btnPage4->setFixedSize(buttonSize);

    btnPage1->setStyleSheet(defaultButtonStyle());
    btnPage2->setStyleSheet(defaultButtonStyle());
    btnPage3->setStyleSheet(defaultButtonStyle());
    btnPage4->setStyleSheet(defaultButtonStyle());

    menuLayout->addWidget(btnPage1);
    menuLayout->addWidget(btnPage2);
    menuLayout->addWidget(btnPage3);
    menuLayout->addWidget(btnPage4);
    menuLayout->addStretch();

    // 스택 위젯 버튼연결
    stack = new QStackedWidget;

    fileDropWidget = new FileDropWidget;
    logPage = new LogPage;
    testBlockPage = new TestBlockPage;
    helpPage = new HelpPage;

    stack->addWidget(fileDropWidget);
    stack->addWidget(testBlockPage);
    stack->addWidget(logPage);
    stack->addWidget(helpPage);

    fileDropWidget->setLogPage(logPage);
    fileDropWidget->setTestBlockPage(testBlockPage);

    connect(btnPage1, &QPushButton::clicked, this, [=](){ tryChangePage(0); });
    connect(btnPage2, &QPushButton::clicked, this, [=](){ tryChangePage(1); });
    connect(btnPage3, &QPushButton::clicked, this, [=](){ tryChangePage(2); });
    connect(btnPage4, &QPushButton::clicked, this, [=](){ tryChangePage(3); });

    splitter->addWidget(leftMenu);
    splitter->addWidget(stack);
    splitter->setStretchFactor(1,1);

    mainL->addWidget(splitter);

    resize(1024, 700);
    setMinimumSize(800, 600);
}

void MainWindow::tryChangePage(int targetIndex) {
    QWidget* current = stack->currentWidget();

    if (current == testBlockPage) {
        if (!testBlockPage->validate()) {
            return;
        }
    }
    stack->setCurrentIndex(targetIndex);
}


MainWindow::~MainWindow()
{
}
