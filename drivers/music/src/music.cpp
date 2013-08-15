#include "../include/music.h"

#include <QString>
QVariantList MusicDriver::getColumns(const QString & filename) {
    Tag t(filename);
    QVariantList result;
    result << t.title();
    result << t.artist();
    result << t.track();
    result << t.album();
    result << t.genre();
    return result;

}
const QStringList MusicDriver::m_columnNames(
        {
        QString("Title")
        ,QString("Artist")
        ,QString("Track")
        ,QString("Album")
        ,QString("Genre")
        }
        );
const QStringList & MusicDriver::columnNames(){
    return m_columnNames;
}


