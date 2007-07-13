#ifndef DICTCORE_H
#define DICTCORE_H

#include <QObject>

#include <list>
#include <string>
#include <vector>
#include <QStringList>
class Libs;

class DictCore: public QObject
{
        Q_OBJECT

    public:
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
        QString translate(const QString &str, bool simple = false, bool useHtml = true);

        static QStringList findDicts(const QString &dir);

    private:
        struct SearchResult
        {
            std::string dictName;
            std::string def;
            std::string exp;
            SearchResult(const std::string &dictName_, const std::string &def_, const std::string &exp_)
                    : dictName(dictName_), def(def_), exp(exp_)
            { }
        }
        ;
        typedef std::vector<SearchResult> SearchResultList;

        void simpleLookup(const std::string &str, SearchResultList &resultList);
        void lookupWithFuzzy(const std::string &str, SearchResultList &resultList);
        void lookupWithRule(const std::string &str, SearchResultList &resultList);
        void lookupData(const std::string &str, SearchResultList &resultList);

        Libs *sdLibs;
        QStringList m_dictDirs;
        QStringList m_orderedDicts;

        static std::string parse_data(const char *data);
        static std::string xdxf2text(const char *p);
};

#endif // DICTCORE_H

