#ifndef QSTARDICT_UTIL_H
#define QSTARDICT_UTIL_H

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
# include <QTextDocument>
#else
# include <QString>
#endif

namespace QStarDict {

class Util
{
public:
    static inline QString htmlEscape(const QString &text) {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        return Qt::escape(text);
#else
        return text.toHtmlEscaped();
#endif
    }
};

} // namespace QStarDict

#endif // QSTARDICT_UTIL_H
