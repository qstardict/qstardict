#ifndef QSTARDICT_UTIL_H
#define QSTARDICT_UTIL_H

#include <QtGlobal>
#include <QString>

namespace QStarDict {

class Util
{
public:
    static inline QString htmlEscape(const QString &text) {
        return text.toHtmlEscaped();
    }
};

} // namespace QStarDict

#endif // QSTARDICT_UTIL_H
