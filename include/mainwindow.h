#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QMainWindow>
#include "scanner.h"
class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget * parent = 0);
    private:
        Scanner m_scanner;
};
#endif
