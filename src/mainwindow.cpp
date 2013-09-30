#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include "mainwindow.h"
#include "optionsmenu.h"
#include "drivers/music/include/tag.h"
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) 
      ,m_qnam(std::make_shared<QNetworkAccessManager>())
     ,m_torrentmanager(new TorrentManager(m_qnam,this))
      ,m_scanner("/home/mtao/Music")
     ,m_rssmanager(new RSSManager(m_qnam,m_torrentmanager))
      , m_options(new OptionsMenu(this))
{
    auto&& arguments = QCoreApplication::arguments();
    for(auto&& a: arguments) {
        qWarning() << a;
    }
    connectSockets();
    createMenu();
    createOptionsDialog();
    m_options->setWindowFlags(Qt::Dialog);

    /*
    m_rssmanager->addFeed(
            "https://broadcasthe.net/feeds.php?feed=torrents_notify_10252_trgub5w8xlr6v357cyfat5fvetswoc1g&user=1485564&auth=36ebcd4c8a0e8887473dae1986be8366&passkey=trgub5w8xlr6v357cyfat5fvetswoc1g&authkey=62aec26375c8c2d85cdf31985d50f3d1&name=Series+notifications"
            );
            */
}

void MainWindow::getTorrentUrl(const QUrl& url) {
    emit gotTorrentUrl(url);
}

void MainWindow::connectSockets() {
    connect(this, &MainWindow::gotTorrentUrl, m_torrentmanager->fetcher(), &TorrentFetcher::addTorrentUrl);
    connect(this, &MainWindow::gotTorrentFile, m_torrentmanager->fetcher(), &TorrentFetcher::addTorrentFile);
}
void MainWindow::openFile() {
    openFile_(
    QFileDialog::getOpenFileName(this, tr("Choose a torrent file"),
            QDir::homePath(),tr("*.torrent"))
    );
}

void MainWindow::openFile_(const QString& filename) {
    emit gotTorrentFile(filename);
}

void MainWindow::createOptionsDialog() {
    m_options->addMenu("RSS Feeds", m_rssmanager->createOptions());
    m_options->addMenu("Torrents", m_torrentmanager->createOptions());
    //m_options->setMenu("RSS Feeds");
    //m_options->addMenu("Torrent Management", m_torrentmanager->createOptions());
    //m_options->addMenu("Scanner Management", m_scanner->createOptions());
}


void MainWindow::createMenu() {
    QMenuBar * menubar = new QMenuBar(this);
    setMenuBar(menubar);
    QMenu* fileMenu = menubar->addMenu(tr("&File"));
    QMenu* optionsMenu = menubar->addMenu(tr("&Options"));

    QAction *RSSOptionsAction = new QAction(tr("&RSS"),this);
    
    QAction *OptionsAction = new QAction(tr("&Options"),this);
    
    connect(OptionsAction, &QAction::triggered,
            m_options, &OptionsMenu::show);
    optionsMenu->addAction(OptionsAction);
    optionsMenu->addAction(RSSOptionsAction);

    QAction *quitAct = new QAction(tr("&Quit"), this);
    connect(quitAct, &QAction::triggered, this, &MainWindow::close);
    QAction *openAct = new QAction(tr("&Open"), this);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAct);
    fileMenu->addAction(quitAct);

    
}
