#include <QApplication>
#include <QStyleFactory>
#include "Ui/mainwindow.h"

int main(int argc, char *argv[])
{
    
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow page;
    page.show();

    return app.exec();
    
}
