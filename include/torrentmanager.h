#ifndef TORRENTMANAGER_H
#define TORRENTMANAGER_H
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/session.hpp>
#include "download_manager.h"

class TorrentFetcher;

class TorrentFetcher: public DownloadManager {
    Q_OBJECT
    public:
        TorrentFetcher(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
        std::shared_ptr<DownloadTask> createDownloadTask(QNetworkReply*);
    public slots:
        void addTorrent(const QUrl &);
    signals:
        void torrentContents(const libtorrent::lazy_entry &);
};

class TorrentDownloadTask: public DownloadTask {
    Q_OBJECT
    public: 
        TorrentDownloadTask(QNetworkReply* reply);
        bool isFinished() const {return finished;}
    private:
        bool finished = false;
    public slots:
        void finish();
    signals:
        void torrentFile(const QByteArray&);
};
class TorrentManager: public QObject {
    Q_OBJECT
    public:
        TorrentManager(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
        void setPort(int beginRange, int endRange);
        QString getDownloadPath(const libtorrent::lazy_entry& le) const;
    public slots:
        void parseTorrentFile(const QByteArray&);
    private:
        TorrentFetcher * m_fetcher;

        libtorrent::session m_session;
};
#endif
