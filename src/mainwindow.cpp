#include <QtCore/QDebug>
#include <QStringList>
#include <QCoreApplication>
#include "include/mainwindow.h"
#include "drivers/music/include/tag.h"
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) 
      ,m_scanner("/home/mtao/Music")
{
    auto&& arguments = QCoreApplication::arguments();
    for(auto&& a: arguments) {
        qWarning() << a;
    }





}
