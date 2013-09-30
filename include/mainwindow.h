#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QtWidgets/QMainWindow>
#include "scanner.h"
#include "rssmanager.h"
#include "torrentmanager.h"
class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget * parent = 0);
    private:
        void createMenu();
        void createOptionsDialog();
        void connectSockets();
    private:
        std::shared_ptr<QNetworkAccessManager> m_qnam;
        TorrentManager * m_torrentmanager;
        Scanner m_scanner;
        RSSManager * m_rssmanager;
        OptionsMenu * m_options;
    public slots:
        void getTorrentUrl(const QUrl& url);
        void openFile();
        void openFile_(const QString& url);
    signals:
        void gotTorrentUrl(const QUrl&);
        void gotTorrentFile(const QString&);
};
#endif
