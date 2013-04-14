#ifndef SCANNER_H
#define SCANNER_H
#include <QFileSystemWatcher>
#include <QWidget>

class Scanner: public QObject{
    Q_OBJECT
    public:
        Scanner(const QString & root);
        Scanner(const QStringList & rootpaths);
    private:
        void recursiveAddPaths(const QString & path, unsigned int depth=0);
        QFileSystemWatcher * m_watcher;
        unsigned int m_max_depth=5;
        int findDepth(const QString & path);
public slots:
        void fileChanged(const QString &);
        void dirChanged(const QString &);
};

#endif
