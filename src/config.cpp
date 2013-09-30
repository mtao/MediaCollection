#include "config.h"
namespace internal {
QString concatenateToPath(std::initializer_list<QString> list) {
    QString result(*list.begin());
    for(const QString * it = list.begin()+1; it != list.end(); ++it) {
        result = result.append(QDir::separator()).append(*it);
    }
    return result;
};
}
static const QString configDir(
        concatenateToPath(
            {QDir::home().path(),".config","MediaCollection"}
            )
        );
