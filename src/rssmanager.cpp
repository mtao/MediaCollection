#include "rssmanager.h"
#include <QtNetwork/QNetworkReply>
#include <QDebug>

RSSManager::RSSManager(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): DownloadManager(manager, parent) {
}


void RSSManager::addFeed(const QString& url) {
    fetch(*m_urls.insert(url));
}

void RSSManager::update() {
    for(auto&& url: m_urls) {
        fetch(url);
    }
}
std::shared_ptr<DownloadTask> RSSManager::createDownloadTask(QNetworkReply* reply) {
    auto&& reader = std::make_shared<RSSReader>(reply);
    connect(reader.get(), SIGNAL(torrentUrl(const QUrl&)), parent(), SLOT(getTorrentUrl(const QUrl&)));
    return reader;
}


RSSReader::RSSReader(QNetworkReply* reply): DownloadTask(reply), reader(reply) {
    connect(reply, SIGNAL(finished()),
            this, SLOT(finish()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(readyReadSlot()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(readProgress(qint64,qint64)));

}

void RSSReader::finish() {
    finished = true;
}

void RSSReader::readProgress(qint64 rec, qint64 total) {
    qDebug() << " " << rec << "/" << total;
}

void RSSReader::readyReadSlot(){
    bool finished = true;
    while(!reader.atEnd()) {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "link") {
            emit torrentUrl(QUrl(reader.readElementText()));
        }
    }

}
