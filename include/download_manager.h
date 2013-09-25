
#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QSet>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QLinkedList>
#include <memory>
#include <QXmlStreamReader>

class QNetworkReply;
class DownloadTask;
class DownloadManager: public QObject {

    Q_OBJECT
    public:
        DownloadManager(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
    public slots:
        void fetch(const QString& url);
        void fetch(const QUrl& url);
        void cleanQueues();
        virtual std::shared_ptr<DownloadTask> createDownloadTask(QNetworkReply*) = 0;
    private:
       std::shared_ptr<QNetworkAccessManager> m_qnam;
       QLinkedList<std::shared_ptr<DownloadTask> > m_readers;
       QLinkedList<QNetworkRequest> m_pending_requests;
       int num_concurrent_readers = 5;
};


class DownloadTask: public QObject {
    Q_OBJECT
    public:
        DownloadTask(QNetworkReply * reply);
        virtual bool isFinished() const = 0;
    protected:
        QNetworkReply * reply;
};

#endif
