#ifndef MUSIC_DRIVER_H
#define MUSIC_DRIVER_H

#include <QString>
#include <QStringList>
#include "../../../include/driver.h"
#include "tag.h"
class MusicDriver: public Driver {
    public:
        QVariantList getColumns(const QString & filename);
        const QStringList & columnNames();
    private:
        const static QStringList m_columnNames;

};

#endif
