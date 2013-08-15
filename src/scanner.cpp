#include "include/scanner.h"
#include <QStringList>
#include <QtCore/QFileInfo>
#include <QDir>
#include "drivers/music/include/tag.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>

QString concatenateToPath(std::initializer_list<QString> list) {
    QString result(*list.begin());
    for(const QString * it = list.begin()+1; it != list.end(); ++it) {
        result = result.append(QDir::separator()).append(*it);
    }
    return result;
}
static const QString configDir(
        concatenateToPath(
            {QDir::home().path(),".config","MediaCollection"}
            )
        );

Scanner::Scanner(const QString & root): Scanner(QStringList{root}) {}

    Scanner::Scanner(const QStringList & rootlist)
: m_watcher()
    , m_db(QSqlDatabase::addDatabase("QSQLITE"))
    , m_dir(QDir::currentPath())
{
    connect(&m_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
    connect(&m_watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(rootChanged(const QString &)));
    connect(&m_watcher, SIGNAL(pathCreated(const QString &)), this, SLOT(fileChanged(const QString &)));
    connect(&m_watcher, SIGNAL(pathDeleted(const QString &)), this, SLOT(fileChanged(const QString &)));


    if(!m_dir.mkpath(configDir)) {
        qWarning() << "Could not create config dir!!!";
    }

    initializeDatabase();
    //First initialize old watches, then new ones
    initializeWatcher();
    for(auto&& root: rootlist) {
        addRootDir(root);
    }

}



void Scanner::addRootDir(const QString & path) { 
    QSqlQuery query;
    bool ret=query.exec(QString("insert into scanRoots values(NULL,'%1') ").arg(path));
    if(!ret) {
        qWarning() << "Failed: " << query.lastQuery();
        qWarning() << "Failed: " << query.lastError();
    }
}

void Scanner::printRoots() const {
    QSqlQuery query(QString("select * from scanRoots"));
    while(query.next()) {
        qWarning() 
            << query.value(0).toInt()
            << " " << query.value(1).toString()
            ;
    }
}

void Scanner::printFollowed() const {
    QSqlQuery query(QString("select * from scanFileCache"));
    while(query.next()) {
        qWarning() 
            << query.value(0).toInt()
            << " " << query.value(1).toInt()
            << " " << query.value(2).toString()
            << " " << query.value(2).toDateTime().toString(Qt::ISODate);
            ;
    }
}


void Scanner::initializeWatcher() {
    QSqlQuery query(QString("select * from scanRoots"));
    QStringList rootlist;
    while(query.next()) {
        int id = query.value(0).toInt();
        QString path = query.value(1).toString();

        m_watcher.addPath(path);

        QDir::setCurrent(path);
        checkDir(id,".");
        QDir::setCurrent(m_dir.absolutePath());
    }
    /*
    for(auto&& a: m_watcher.directories()) {
        qWarning() << a;
    }

    for(auto&& a: m_watcher.files()) {
        qWarning() << a;
    }
    */
}



void Scanner::initializeDatabase() {
    QString dbpath(QDir::toNativeSeparators(
                concatenateToPath({configDir,"scanner.db"})
                ));

    m_db.setDatabaseName(dbpath);
    if(!m_db.open()) 
    {
        qWarning() << "Could not open database!";
        return;//Dont expect anything after this to work...
    }

    QSqlQuery query;
    bool ret = query.exec(
            "create table if not exists scanRoots "
            "(id integer primary key,"
            "path varchar(128) not null unique"
            ")"
            );
    if(ret) {
        qWarning() << "Was able to create scanroots table or it already existed";
    } else {
        qWarning() << "Failed to create scanroots table";
    }
    bool ret2 = query.exec(
            "create table if not exists scanFileCache "
            "(id integer primary key,"
            "rootid integer,"// foreign key references scanRoots(id),"
            "path varchar(128) not null,"
            "lastmodified timestamp,"//To keep track of modifications since last launch
            "foreign key(rootid) references scanRoots(id)"// foreign key references scanRoots(id),"
            ")"
            );
    if(ret2) {
        qWarning() << "Was able to create scandirs table or it already existed";
    } else {
        qWarning() << "Failed to create scandirs table";
        qWarning() << query.lastError();
    }



}

void Scanner::checkDir(int rootid, const QString & path, unsigned int depth) {
    if(depth > m_max_depth) return;
    qWarning () << "Checking the dir " << path ;
    QFileInfo fileInfo(path);
    QDateTime lastMod = fileInfo.lastModified();
    //TODO: we're doing string comparison here, which is bad :(
    QSqlQuery query(QString("select id,lastmodified from scanFileCache where rootid = %1 and path = '%2'")
            .arg(rootid).arg(path));
    bool dirty = false;
    int row;//bad data unless dirty is true
    if(!query.next()) {//not in db yet so add
        bool ret = query.exec(QString("insert into scanFileCache values(NULL,%1,'%2','%3')")
                .arg(rootid).arg(path).arg(lastMod.toString(Qt::ISODate)));
        if(!ret) {
            qWarning() << "Was unable to execute the following sql query:" << query.lastQuery();
        }
        dirty = true;
    } else {
        row = query.value(0).toInt();
        dirty = query.value(1).toDateTime() < lastMod;
        qWarning() <<"Date comparison: " << dirty << " "<< query.value(1).toString() << " " << lastMod.toString();
    }
    if(dirty) {
        if (fileInfo.isDir()) {


            QDir dir(path);
            QStringList fileList = dir.entryList(
                    QDir::AllEntries | QDir::NoDotAndDotDot
                    | QDir::NoSymLinks | QDir::Hidden, QDir::Name
                    | QDir::IgnoreCase);
            for (int i = 0; i < fileList.count(); ++i) {
                checkDir(rootid, path + tr("/")+fileList.at(i),depth+1);
            }
        } else {
            //TODO: Update metadata...
        }
        //Update row for timestamp after we've updated this...
        bool ret = query.exec(QString("update scanFileCache set lastmodified='%1' where id=%2")
                .arg(lastMod.toString(Qt::ISODate)).arg(row));
        if(!ret) {
            qWarning() << "Was unable to execute the following sql query:" << query.lastQuery();
        }
    }
}

Scanner::~Scanner() {
    m_db.close();
}


//Note:
//This should never be called because we are assuming that scanning all files will waste all inotify :(
void Scanner::fileChanged(const QString & filepath) {
    qWarning()<<"File: " << filepath;
    static_assert(true, "Shouldn't scan for files themselves");
}

//Note:
//Scanner will detect files being added but collection has to notice files not existing

//TODO: deal with stuff being deleted
void Scanner::rootChanged(const QString & path) {
    QSqlQuery query(QString("select id from scanRoots where path='%1'").arg(path));
    if(query.next()) {
        QDir::setCurrent(path);
        checkDir(query.value(0).toInt(),".");
        QDir::setCurrent(m_dir.absolutePath());
    }
}


int Scanner::findDepth(const QString & path) {
    int min=m_max_depth + 1;
    for(QString & dirname: m_watcher.directories()) {
        QDir dir(dirname);
        int depth = dir.relativeFilePath(path).split(tr("/")).size();
        if(depth < min)
            min = depth;
    }
    return min;
}
