#ifndef TORRENTMANAGER_H
#define TORRENTMANAGER_H
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/session.hpp>
#include "download_manager.h"
#include "optionsmenu.h"

class TorrentFetcher;

class TorrentFetcher: public DownloadManager {
    Q_OBJECT
    public:
        TorrentFetcher(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
        std::shared_ptr<DownloadTask> createDownloadTask(QNetworkReply*);
    public slots:
        void addTorrentUrl(const QUrl &);
        void addTorrentFile(const QString &);
    signals:
        void torrentContents(const QByteArray&);
};

class TorrentDownloadTask: public DownloadTask {
    Q_OBJECT
    public: 
        TorrentDownloadTask(QNetworkReply* reply);
        bool isFinished() const {return m_finished;}
    private:
        bool m_finished = false;
    public slots:
        void finish();
    signals:
        void torrentContents(const QByteArray&);
        void finished();
};

class TorrentManager;
class TorrentOptionsMenu: public OptionsSubMenu {
    Q_OBJECT
    public:
        TorrentOptionsMenu(TorrentManager* manager);
    private:
        TorrentManager * m_manager;
        void load();
    protected:
        void save();
        void reset();
    public slots:
};


class TorrentManager: public QObject {
    Q_OBJECT
    public:
        TorrentManager(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
        ~TorrentManager();
        void setPort(int beginRange, int endRange);
        TorrentFetcher* fetcher() {return m_fetcher;}
        TorrentOptionsMenu* createOptions();
        QString getDownloadPath(const libtorrent::lazy_entry& le) const;
        libtorrent::session& getSession() {return m_session;}
    private:
        void loadSession();
        void saveSession() const;
    public slots:
        void parseTorrentContents(const QByteArray&);
    private:
        TorrentFetcher * m_fetcher;
        TorrentOptionsMenu * m_options = 0;

        libtorrent::session m_session;
        std::vector<torrent_handle> m_torrent_handles;
};
#endif
