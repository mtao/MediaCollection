#include <QApplication>
#include "../include/mainwindow.h"

int main(int argc, char * argv[]) {
    QApplication app(argc,argv);
    app.setApplicationName("Music Collection Browser");
    MainWindow window;
    window.show();
    return app.exec();
}

