#include "../include/mainwindow.h"
#include <QApplication>

int main(int argc, char * argv[]) {
    QApplication app(argc,argv);
    QCoreApplication::setOrganizationName("mtao");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("MediaCollection");
    MainWindow window;
    window.show();
    return app.exec();
}

