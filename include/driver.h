#ifndef DRIVER_H
#define DRIVER_H
#include <QVariant>
#include <QList>
typedef QList<QVariant> QVariantList;
typedef QMap<QString,QVariant> ColumnMapping;
class Driver {
    public:
        Driver() {}
        virtual ~Driver() {}
        virtual QVariantList getColumns(const QString & filename) = 0;
        virtual const QStringList & columnNames() = 0;
        ColumnMapping parseFilename(const QString & filename);
        

};

#endif
