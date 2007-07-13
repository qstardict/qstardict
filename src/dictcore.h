#ifndef DICTCORE_H
#define DICTCORE_H

#include <QObject>

#include <string>
#include <vector>
#include <QFlags>
#include <QStringList>
class Libs;

class DictCore: public QObject
{
        Q_OBJECT

    public:
        enum TranslationFlag
        {
            None,
            Simple,
            Html,
            Reformat
        };
        Q_DECLARE_FLAGS(TranslationFlags, TranslationFlag)

        DictCore(QObject *parent = 0);
        ~DictCore();

        void setDictDirs(const QStringList &dictDirs);
        QStringList dictDirs() const;

        void setDicts(const QStringList &orderedDicts);
        QStringList orderedDicts() const;
        QStringList disabledDicts() const;
        QStringList avialableDicts() const;

        QStringList find(const QString &str);
        bool isTranslatable(const QString &str);
        QString translate(const QString &str, TranslationFlags flags = TranslationFlags(None) | Reformat);

        static QStringList findDicts(const QString &dir);

    private:
        class SearchResult
        {
            public:
                QString dictName;
                QString def;
                QString exp;

                SearchResult(const char *_dictName, const char *_def, const char *_exp)
                        : dictName(QString::fromUtf8(_dictName)),
                          def(QString::fromUtf8(_def)),
                          exp(QString::fromUtf8(_exp))
                { }
        };
        typedef std::vector<SearchResult> SearchResultList;

        void simpleLookup(const std::string &str, SearchResultList &resultList);
        void lookupWithFuzzy(const std::string &str, SearchResultList &resultList);
        void lookupWithRule(const std::string &str, SearchResultList &resultList);
        void lookupData(const std::string &str, SearchResultList &resultList);
        QString translation(const QString &str, const QString &dict);

        Libs *sdLibs;
        QStringList m_dictDirs;
        QStringList m_orderedDicts;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DictCore::TranslationFlags)

#endif // DICTCORE_H

