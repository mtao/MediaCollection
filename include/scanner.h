#ifndef SCANNER_H
#define SCANNER_H
#include <QFileSystemWatcher>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QWidget>
#include <QDir>

class Scanner: public QObject{
    Q_OBJECT
    public:
        Scanner(const QString & root);
        Scanner(const QStringList & rootpaths);
        ~Scanner();
    private:
        void initializeWatcher();
        void initializeDatabase();



        void checkDir(int rootid, const QString & path, unsigned int depth=0);
        QFileSystemWatcher m_watcher;
        QSqlDatabase m_db;
        QDir m_dir;
        unsigned int m_max_depth=5;
        int findDepth(const QString & path);

public slots:
        void fileChanged(const QString &);
        void rootChanged(const QString &);
        void printRoots() const;
        void printFollowed() const;
        void addRootDir(const QString &);
signals:
        void newFile(const QString &);
};

#endif
