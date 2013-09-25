#ifndef RSSMANAGER_H
#define RSSMANAGER_H
#include "download_manager.h"
#include <QXmlStreamReader>

class RSSReader;
class RSSManager: public DownloadManager {

    Q_OBJECT
    public:
        RSSManager(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), QObject * parent = 0);
    public slots:
        void addFeed(const QString& url);
        void update();
    private:
       QSet<QString> m_urls;
        std::shared_ptr<DownloadTask> createDownloadTask(QNetworkReply*);
};


class RSSReader: public DownloadTask {
    Q_OBJECT
    public:
        RSSReader(QNetworkReply * reply);
        bool isFinished() const {return finished;}
    private:
        QXmlStreamReader reader; 
        QNetworkReply * reply;
        bool finished = false;
    public slots:
        void readyReadSlot();
        void finish();
        void readProgress(qint64,qint64);
    signals:
        void torrentUrl(const QUrl&);


};

#endif
