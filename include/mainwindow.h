#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QtWidgets/QMainWindow>
#include "scanner.h"
#include "rssmanager.h"
class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget * parent = 0);
    private:
        std::shared_ptr<QNetworkAccessManager> m_qnam;
        Scanner m_scanner;
        RSSManager m_rssmanager;
    public slots:
        void getTorrentUrl(const QUrl& url);
    signals:
        void addTorrentSignal(const QUrl&);
};
#endif
