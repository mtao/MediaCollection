#include "../include/scanner.h"
#include <QStringList>
#include <QtCore/QFileInfo>
#include <QDir>
#include "drivers/music/include/tag.h"
#include <QDebug>



Scanner::Scanner(const QString & root): Scanner(QStringList{root}) {}
Scanner::Scanner(const QStringList & rootlist)
    : m_watcher(new QFileSystemWatcher(this)) {
        connect(m_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
        connect(m_watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(dirChanged(const QString &)));

        for(auto&& path: rootlist) {
            recursiveAddPaths(path);
        }
    for(auto&& a: m_watcher->directories()) {
        qWarning() << a;
    }

    for(auto&& a: m_watcher->files()) {
        qWarning() << a;
    }
    }
void Scanner::recursiveAddPaths(const QString & path, unsigned int depth) {
    if(depth > m_max_depth) return;
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
    m_watcher->addPath(path);
        QDir dir(path);
        QStringList fileList = dir.entryList(
                QDir::AllEntries | QDir::NoDotAndDotDot
                | QDir::NoSymLinks | QDir::Hidden, QDir::Name
                | QDir::IgnoreCase);
        for (int i = 0; i < fileList.count(); ++i) {
            recursiveAddPaths(path + tr("/")+fileList.at(i),depth+1);
        }
    } else {
        /*
        Tag t(path);
        qWarning() 
            << t.title() << " " 
            << t.artist() << " " 
            << t.album() << " " 
            << t.genre() << " " 
            << t.comment() << " " 
            << t.track();
        */
    }
}

#include <QDebug>
void Scanner::fileChanged(const QString & filepath) {
    qWarning()<<"File: " << filepath;
}

//TODO: deal with stuff being deleted
void Scanner::dirChanged(const QString & path) {
    QDir dir(path);
    QStringList fileList = dir.entryList(
            QDir::AllEntries | QDir::NoDotAndDotDot
            | QDir::NoSymLinks | QDir::Hidden, QDir::Name
            | QDir::IgnoreCase);
    //Add all directories not in the list of paths
    for(const QString & name: fileList) {
        QFileInfo info(name);
        if(info.isDir()) {
            if(m_watcher->directories().indexOf(name)==-1) {//TODO: Scans whole list of directories, should make faster
                recursiveAddPaths(path, findDepth(path));
            }

        } else {
        }
    }
    /*
    for (int i = 0; i < fileList.count(); ++i) {
        QString&& str = fileList
        recursiveAddPaths(path + tr("/")+fileList.at(i),depth+1);
    }
    */
    qWarning() <<"Dir: " << path;
}

int Scanner::findDepth(const QString & path) {
    int min=m_max_depth + 1;
    for(QString & dirname: m_watcher->directories()) {
        QDir dir(dirname);
        int depth = dir.relativeFilePath(path).split(tr("/")).size();
        if(depth < min)
            min = depth;
    }
    return min;
}
