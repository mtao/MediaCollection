#ifndef RSSMANAGER_H
#define RSSMANAGER_H
#include "download_manager.h"
#include "torrentmanager.h"
#include "optionsmenu.h"
#include <QXmlStreamReader>
#include <QSettings>

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
class RSSOptionsMenu;
class RSSManager: public DownloadManager {

    Q_OBJECT
    public:
        RSSManager(const std::shared_ptr<QNetworkAccessManager> &manager = std::make_shared<QNetworkAccessManager>(), TorrentManager * parent = 0);
        ~RSSManager();
    public slots:
        void addFeed(const QString& url);
        void update();
        void save();
        RSSOptionsMenu* createOptions();
       const QSet<QString>& urls() const{return m_urls;}
       void setUrls(const QSet<QString>& urls);
    private:
       TorrentManager* m_torrentmanager;
       RSSOptionsMenu * m_options;
       QSet<QString> m_urls;
        std::shared_ptr<DownloadTask> createDownloadTask(QNetworkReply*);
        QSettings m_settings;
};

//TODO: Switch this to a tablewidget so we can introduce aliases
//OR create an additional options dialog for modifying options for a feed 
//This can be fetch frequency, alias, actual url etc
class QListWidget;
class QLineEdit;
class RSSOptionsMenu: public OptionsSubMenu {
    Q_OBJECT
    public:
        RSSOptionsMenu(RSSManager* manager);
    private:
        RSSManager * m_manager;
        QListWidget * m_list;
        QLineEdit * m_lineedit;
        void load();
    protected:
        void save();
        void reset();
        public slots:
            void addUrl();
            void remove();



};
#endif
