#include "include/driver.h"
#include <QVariant>

ColumnMapping parseFilename(const QString & filename) {
    const QVariantList values = getColumns(filename);
    const QStringList & colnames = columnNames();

    ColumnMapping ret;
    auto&& b = values.constBegin();
    auto&& a = colnames.constBegin();
    for(;a != colnames.constEnd() && b != values.constEnd(); ++a, ++b) {

        ret[*a] = *b;
    }
}
