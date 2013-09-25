#include "torrentmanager.h"
#include <QtNetwork/QNetworkReply>


TorrentFetcher::TorrentFetcher(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): DownloadManager(manager,parent) {
}


std::shared_ptr<DownloadTask> TorrentFetcher::createDownloadTask(QNetworkReply* reply) {
    auto&& reader = std::make_shared<DownloadTask>(reply);
    return reader;
}

void TorrentFetcher::addTorrent(const QUrl& url) {
    fetch(url);
}

TorrentDownloadTask::TorrentDownloadTask(QNetworkReply* reply): DownloadTask(reply) {
}

void TorrentDownloadTask::finish() {
    emit torrentFile(reply->readAll());
    
}


TorrentManager::TorrentManager(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): QObject(parent), m_fetcher(manager,this) {
    connect(parent, SIGNAL(addTorrentSignal(const QUrl&)), &m_fetcher, SLOT(addTorrent(const QUrl&)));
    connect(&m_fetcher, SIGNAL(torrentFile(const QByteArray&)), this, SLOT(parseTorrentFile(const QByteArray&)));
    m_session.listen_on(std::make_pair(6881,6889));
}

void TorrentManager::setPort(int beginRange, int endRange) {
    m_session.listen_on(std::make_pair(beginRange, endRange));
}

QString TorrentManager::getDownloadPath(const libtorrent::lazy_entry& le) const {
    return "/mnt/3tb2/dump";

}
void TorrentManager::parseTorrentFile(const QByteArray& data) {
    libtorrent::lazy_entry le;
    libtorrent::error_code ec;
    int ret = libtorrent::lazy_bdecode(data.constData(), data.constData()+data.length(), le);
    libtorrent::torrent_info * ti = new libtorrent::torrent_info(le, ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
    }
    libtorrent::add_torrent_params p;
    p.save_path = getDownloadPath(le);
    p.ti = ti;
    m_session.add_torrent(p,ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
    }
    
}
