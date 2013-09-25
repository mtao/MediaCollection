#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QCoreApplication>
#include "include/mainwindow.h"
#include "drivers/music/include/tag.h"
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) 
      ,m_qnam(std::make_shared<QNetworkAccessManager>())
      ,m_scanner("/home/mtao/Music")
     ,m_rssmanager(m_qnam,this)
{
    auto&& arguments = QCoreApplication::arguments();
    for(auto&& a: arguments) {
        qWarning() << a;
    }

    m_rssmanager.addFeed(
            "https://broadcasthe.net/feeds.php?feed=torrents_notify_10252_trgub5w8xlr6v357cyfat5fvetswoc1g&user=1485564&auth=36ebcd4c8a0e8887473dae1986be8366&passkey=trgub5w8xlr6v357cyfat5fvetswoc1g&authkey=62aec26375c8c2d85cdf31985d50f3d1&name=Series+notifications"
            );
}

void MainWindow::getTorrentUrl(const QUrl& url) {
    emit addTorrentSignal(url);
}
