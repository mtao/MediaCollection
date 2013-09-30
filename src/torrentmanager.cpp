#include "torrentmanager.h"
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QFile>
#include <QVBoxLayout>
#include <QPushButton>


TorrentFetcher::TorrentFetcher(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): DownloadManager(manager,parent) {
}


std::shared_ptr<DownloadTask> TorrentFetcher::createDownloadTask(QNetworkReply* reply) {
    auto&& reader = std::make_shared<TorrentDownloadTask>(reply);
    connect(reader.get(), SIGNAL(torrentContents(const QByteArray&)), parent(),SLOT(parseTorrentContents(const QByteArray&)));
    connect(reader.get(), SIGNAL(finished()), SLOT(cleanQueues()));
    return reader;
}

void TorrentFetcher::addTorrentUrl(const QUrl& url) {
    fetch(url);
}
void TorrentFetcher::addTorrentFile(const QString& filename) {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray ret = file.readAll();
    emit torrentContents(ret);


}

TorrentDownloadTask::TorrentDownloadTask(QNetworkReply* reply): DownloadTask(reply) {
    connect(reply, SIGNAL(finished()), this, SLOT(finish()));
}

void TorrentDownloadTask::finish() {
    emit torrentContents(reply->readAll());
    m_finished = true;
    emit finished();
    
}


TorrentManager::TorrentManager(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): QObject(parent), m_fetcher(new TorrentFetcher(manager,this)) {
    //connect(&m_fetcher, SIGNAL(torrentFile(const QByteArray&)), this, SLOT(parseTorrentFile(const QByteArray&)));
    connect(m_fetcher, SIGNAL(torrentContents(const QByteArray&)), this,SLOT(parseTorrentContents(const QByteArray&)));
    libtorrent::error_code ec;
    m_session.listen_on(std::make_pair(6881,6889),ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
        return;
    }
}

void TorrentManager::setPort(int beginRange, int endRange) {
    libtorrent::error_code ec;
    m_session.listen_on(std::make_pair(beginRange, endRange),ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
        return;
    }
}
TorrentOptionsMenu* TorrentManager::createOptions() {
    if(!m_options == 0) {
        return m_options;
    } else {
        return m_options = new TorrentOptionsMenu(this);
    }
}

QString TorrentManager::getDownloadPath(const libtorrent::lazy_entry& le) const {
    return "/mnt/3tb2/dump";

}
void TorrentManager::parseTorrentContents(const QByteArray& data) {
    libtorrent::lazy_entry le;
    libtorrent::error_code ec;
    int ret = libtorrent::lazy_bdecode(data.constData(), data.constData()+data.length(), le,ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
        return;
    }
    libtorrent::torrent_info * ti = new libtorrent::torrent_info(le, ec);
    if(ec) {
        std::cout << ec.message() << std::endl;
    }
    libtorrent::add_torrent_params p;
    p.save_path = getDownloadPath(le).toStdString();
    p.ti = ti;
    m_torrent_handles.emplace(m_session.add_torrent(p,ec));
    
    if(ec) {
        std::cout << ec.message() << std::endl;
    }
    
}


TorrentOptionsMenu::TorrentOptionsMenu(TorrentManager* manager): OptionsSubMenu(), m_manager(manager) {
    
    QVBoxLayout * layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(new QPushButton("Press me!", this));
}


void TorrentOptionsMenu::load() {
    
}

void TorrentOptionsMenu::save() {
}

void TorrentOptionsMenu::reset() {load();}
