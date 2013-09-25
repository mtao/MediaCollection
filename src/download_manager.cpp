#include "rssmanager.h"
#include <QtNetwork/QNetworkReply>
#include <QDebug>

DownloadManager::DownloadManager(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent): QObject(parent), m_qnam(manager) {
}



void DownloadManager::fetch(const QString& url) {
    fetch(QUrl(url));
}
void DownloadManager::fetch(const QUrl& url) {
    qDebug() << "Fetching url: " << url;
    m_pending_requests.push_back(QNetworkRequest());
    QNetworkRequest& request = m_pending_requests.back();
    request.setUrl(url);
    request.setRawHeader("User-Agent", "Wololo 1.0");

    cleanQueues();
}

void DownloadManager::cleanQueues() {
    qDebug() << "ActiveRequests/PendingRequests: " << m_readers.size() << "/" << m_pending_requests.size();
    typedef QLinkedList<std::shared_ptr<DownloadTask> > DownloadLL;
    typedef QLinkedList<QNetworkRequest> NetReqLL;
    DownloadLL::iterator it = m_readers.begin();
    while(it != m_readers.end()) {
        if((*it)->isFinished()) {
            m_readers.erase(it++);
        } else {
            ++it;
        }
    }
    int new_responses = qMin(num_concurrent_readers-m_readers.size(), m_pending_requests.size());
    for(int i=0; i < new_responses; ++i) {
        auto&& x = m_qnam->get(m_pending_requests.takeFirst());
        m_readers.push_back(createDownloadTask(x));
    }
}

DownloadTask::DownloadTask(QNetworkReply* reply): reply(reply) {
}

