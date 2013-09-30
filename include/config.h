#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
namespace internal {
QString concatenateToPath(std::initializer_list<QString> list);
}
static const QString configDir;
#endif
