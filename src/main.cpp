#include <QApplication>
#include "Ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow page;
    page.show();

    return app.exec();

}
