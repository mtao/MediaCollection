#include "rssmanager.h"
#include "mainwindow.h"
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>

RSSManager::RSSManager(const std::shared_ptr<QNetworkAccessManager> &manager, QObject * parent)
: DownloadManager(manager, parent)
, m_settings("MediaCollection","rssmanager")
{
    m_settings.beginGroup("Feeds");
    int size = m_settings.beginReadArray("feeds");
    for(int i=0; i < size; ++i) {
        m_settings.setArrayIndex(i);
        m_urls.insert(m_settings.value("url").toString());
    }
    m_settings.endArray();
    m_settings.endGroup();
}


RSSManager::~RSSManager() {
    if(m_options) 
        delete m_options;
    save();
}
void RSSManager::save() {
    m_settings.beginGroup("Feeds");
    m_settings.beginWriteArray("feeds");
    int i=0;

    for(auto&& url: m_urls) {
        m_settings.setArrayIndex(i++);
        m_settings.setValue("url", url);
    }
    m_settings.endArray();
    m_settings.endGroup();
    m_settings.sync();
}

void RSSManager::addFeed(const QString& url) {
    *m_urls.insert(url);
}

RSSOptionsMenu* RSSManager::createOptions() {
    if(!m_options) {
        return m_options;
    } else {
        return m_options = new RSSOptionsMenu(this);
    }
}

void RSSManager::setUrls(const QSet<QString>& urls) {
    m_urls = urls;
    update();
}

void RSSManager::update() {
    for(auto&& url: m_urls) {
        fetch(url);
    }
}
std::shared_ptr<DownloadTask> RSSManager::createDownloadTask(QNetworkReply* reply) {
    auto&& reader = std::make_shared<RSSReader>(reply);
    connect(reader.get(), &RSSReader::torrentUrl, qobject_cast<const MainWindow*>(parent()), &MainWindow::getTorrentUrl);
    return reader;
}


RSSReader::RSSReader(QNetworkReply* reply): DownloadTask(reply), reader(reply) {
    connect(reply, &QNetworkReply::finished,
            this, &RSSReader::finish);
    connect(reply, &QNetworkReply::readyRead,
            this, &RSSReader::readyReadSlot);
    connect(reply, &QNetworkReply::downloadProgress, this, &RSSReader::readProgress);

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

RSSOptionsMenu::RSSOptionsMenu(RSSManager* manager): OptionsSubMenu(), m_manager(manager), m_list(new QListWidget(this)), m_lineedit(new QLineEdit(this)) {
    
    QVBoxLayout * layout = new QVBoxLayout(this);
    QWidget* listWidget = new QWidget(this);
    QHBoxLayout*  listLayout = new QHBoxLayout(listWidget);

    QWidget * addWidget = new QWidget(this);
    QHBoxLayout * addLayout = new QHBoxLayout(addWidget);
    layout->addWidget(addWidget);
    QPushButton* addButton = new QPushButton("Add", this);
    connect(addButton,&QPushButton::released,
            this, &RSSOptionsMenu::addUrl);
    QPushButton* remButton = new QPushButton("Delete", this);
    connect(remButton,&QPushButton::released,
            this, &RSSOptionsMenu::remove);

    this->setLayout(layout);
    layout->addWidget(listWidget);
    listWidget->setLayout(listLayout);
    listLayout->addWidget(m_list);
    addLayout->addWidget(m_lineedit);
    addLayout->addWidget(addButton);
    addLayout->addWidget(remButton);
    //listLayout->addWidget(remButton);
    addWidget->setLayout(addLayout);
    load();
}

void RSSOptionsMenu::addUrl() {
    m_list->addItem(m_lineedit->displayText());
}

void RSSOptionsMenu::remove() {
    int row = m_list->currentRow();
    delete m_list->takeItem(row);
}

void RSSOptionsMenu::load() {
    const QSet<QString>& urls = m_manager->urls();
    m_list->clear();
    for(auto&& url: urls) {
        m_list->addItem(url);
    }
    
}

void RSSOptionsMenu::save() {
    QSet<QString> urls;
    for(int i=0; i < m_list->count(); ++i) {
        urls.insert(m_list->item(i)->text());
    }
    m_manager->setUrls(std::move(urls));
}

void RSSOptionsMenu::reset() {load();}
