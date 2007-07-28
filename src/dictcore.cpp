/********************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written with using Qt             *
 * Copyright (C) 2007 Alexander Rodin                                           *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 ********************************************************************************/

#include "dictcore.h"

#include <map>
#include <utility>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStack>
#include <glib/gmem.h>
#include <glib/gstrfuncs.h>
#include "lib/lib.h"
#include "lib/file.hpp"

namespace
{
std::string xdxf2html(const char *p);
std::string parse_data(const char *data);
QString html2text(QString html);
const int MaxFuzzy = 24;
}

DictCore::DictCore(QObject *parent)
        : QObject(parent)
{
    m_sdLibs = 0;
#ifdef Q_OS_UNIX
    m_dictDirs << "/usr/share/stardict/dic";
#else
    m_dictDirs << QCoreApplication::applicationDirPath() + "/dic";
#endif // Q_OS_UNIX

    m_dictDirs << QDir::homePath() + "/.stardict/dic";
}

DictCore::~DictCore()
{
    delete m_sdLibs;
}

void DictCore::setDicts(const QStringList &orderedDicts)
{
    m_orderedDicts = orderedDicts;

    delete m_sdLibs;
    m_sdLibs = new Libs;

    for (QStringList::ConstIterator dictName = m_orderedDicts.begin(); dictName != m_orderedDicts.end(); ++dictName)
        for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); ++dictDir)
            if (QFile::exists(*dictDir + "/" + *dictName + ".ifo"))
            {
                m_sdLibs->load_dict(QDir::toNativeSeparators(*dictDir + "/" + *dictName + ".ifo").toUtf8().data());
                break;
            }
}

QStringList DictCore::disabledDicts() const
{
    QStringList avialable = avialableDicts();
    QStringList disabled;

    for (QStringList::ConstIterator dictName = avialable.begin(); dictName != avialable.end(); ++dictName)
        if (! m_orderedDicts.contains(*dictName))
            disabled << *dictName;

    return disabled;
}

QStringList DictCore::avialableDicts() const
{
    QStringList result;

    for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); ++dictDir)
        result << findDicts(*dictDir);

    return result;
}

QStringList DictCore::findDicts(const QString &dir)
{
    QStringList result = QDir(dir).entryList(QStringList("*.ifo"), QDir::Files);

    for (QStringList::Iterator dictName = result.begin(); dictName != result.end(); ++dictName)
        dictName->remove(QRegExp("\\.ifo$"));
    return result;
}

QStringList DictCore::find(const QString &str)
{
    SearchResultList resultList;
    lookupWithFuzzy(str.toUtf8().data() + std::string("*"), resultList);
    QStringList result;
    for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); ++i)
        if (! result.contains(i->def))
            result << i->def;

    return result;
}

bool DictCore::isTranslatable(const QString &str)
{
    if (str.isEmpty())
        return false;
    long ind;
    for (int idict = 0; idict < m_sdLibs->ndicts(); ++idict)
        if (m_sdLibs->SimpleLookupWord(str.toUtf8().data(), ind, idict))
            return true;
    return false;
}

QString DictCore::translate(const QString &str, TranslationFlags flags)
{
    SearchResultList resultList;
    std::string query;

    if (str.isEmpty())
        return tr("Not found!");

    if (flags.testFlag(Simple))
        switch (analyze_query(str.toUtf8().data(), query))
        {
        case qtFUZZY:
            lookupWithFuzzy(query, resultList);
            break;
        case qtREGEXP:
            lookupWithRule(query, resultList);
            break;
        case qtSIMPLE:
            simpleLookup(str.toUtf8().data(), resultList);
            break;
        case qtDATA:
            lookupData(query, resultList);
            break;
        default:
            ; // nothing
        }
    else
        simpleLookup(str.toUtf8().data(), resultList);

    if (resultList.empty())
        return tr("Not found!");
    
    QString result;
    if (flags.testFlag(Html))
        for (SearchResultList::iterator i = resultList.begin(); i != resultList.end(); ++i)
        {
            if (flags.testFlag(Reformat))
            {
                QRegExp regExp;
                regExp.setPattern("_[I|V|X|L|C|D|M]+\\s*");
                int pos = 0;
                if ((pos = regExp.indexIn(i->exp, pos)) != -1)
                {
                    
                    qWarning("%s\n", i->exp.mid(pos, regExp.matchedLength()).toUtf8().data());qWarning("%s\n", i->exp.mid(pos, regExp.matchedLength()).toUtf8().data());
                    resultList.insert(i, SearchResult(i->dictName, i->def, i->exp.mid(pos)));
                    i->exp.truncate(pos);
                }
                regExp.setMinimal(true);
                pos = 0;
                regExp.setPattern("\\d+[>\\.]");
                QStack<QString> openedLists;
                while ((pos = regExp.indexIn(i->exp, pos)) != -1)
                {
                    QString result = i->exp.mid(pos, regExp.matchedLength());
                    i->exp.remove(pos, regExp.matchedLength());
                    i->exp.insert(pos, "<li>");
                    QString num = result;
                    num.remove(QRegExp("[^\\d]"));
                    result.remove(QRegExp("\\d"));
                    if (openedLists.size() == 0 || openedLists.top() != result)
                    {
                        if (num == "1")
                        {
                            i->exp.insert(pos, "<ol>");
                            if (openedLists.contains(result))
                            {
                                i->exp.insert(pos, "</li></ol>");
                                while (openedLists.size() && openedLists.top() != result)
                                {
                                    i->exp.insert(pos, "</li></ol>");
                                    openedLists.pop();
                                }
                            }
                            openedLists.push(result);
                        }
                        else
                        {
                            while (openedLists.size() && openedLists.top() != result)
                            {
                                i->exp.insert(pos, "</li></ol>");
                                openedLists.pop();
                            }
                        }
                    }
                }
                while (openedLists.size())
                {
                    i->exp += "</li></ol>";
                    openedLists.pop();
                }
                if (! i->exp.contains("<ol>"))
                {
                    i->exp.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
                    i->exp.replace(QRegExp("\n{2,}"), "</p><p>");
                    i->exp.replace("\n", "<br>");
                    i->exp.remove(QRegExp("^(\\s*<br>\\s*)*"));
                }

                regExp.setPattern("^(\\s*\\[.*\\])(?!\\s*<ol>)");
                pos = regExp.indexIn(i->exp);
                if (pos != -1)
                    i->exp.insert(pos + regExp.matchedLength(), "<br>");

                regExp.setPattern("(?=\\[.*\\])");
                i->exp.replace(regExp, "<font class=\"transcription\">");
                i->exp.replace("]", "]</font>");
            }
            else
            {
                i->exp.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
                i->exp.replace("\n", "<br>");
            }
            if (flags.testFlag(ExpandAbbreviations) && i->exp.contains(QRegExp("_\\S*[\\.:]")))
            {
                QRegExp regExp("_\\S*[\\.:]");
                int pos = 0;
                while ((pos = regExp.indexIn(i->exp, pos)) != -1)
                {
                    QString result = translation(i->exp.mid(pos, regExp.matchedLength()), i->dictName);
                    QChar lastChar = i->exp[pos + regExp.matchedLength() - 1];
                    if (! result.isEmpty())
                    {
                        result.remove(QRegExp("^\\s*"));
                        result.remove("<br>");
                        if (lastChar == ':')
                            result.append(":");
                        i->exp.replace(pos, regExp.matchedLength(),
                                "<font class=\"explanation\">" + result + "</font>");
                    }
                }
            }
            result += "<p><font class=\"normal\"><font class=\"dict_name\">" + i->dictName + "</font><br>"
                      "<font class=\"title\">" + i->def + "</font>";
            if (! i->exp.contains(QRegExp("^\\s*<ol>")))
                result += "<br>";
            result += i->exp + "</font></p>";
        }
    else
        for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); ++i)
            result += "<-- " + i->dictName + " -->\n" +
                      "--> " + i->def + "\n" +
                      html2text(i->exp) + "\n\n";
    return result;
}

void DictCore::simpleLookup(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    glong ind;
    for (int idict = 0; idict < m_sdLibs->ndicts(); idict++)
        if (m_sdLibs->SimpleLookupWord(str.c_str(), ind, idict))
            resultList.push_back(
                SearchResult(m_sdLibs->dict_name(idict).c_str(),
                             m_sdLibs->poGetWord(ind, idict),
                             parse_data(m_sdLibs->poGetWordData(ind, idict)).c_str()));
}

QString DictCore::translation(const QString &str, const QString &dict)
{
    long ind;
    for (int idict = 0; idict < m_sdLibs->ndicts(); idict++)
        if (m_sdLibs->dict_name(idict) == dict.toUtf8().data() &&
            m_sdLibs->SimpleLookupWord(str.toUtf8().data(), ind, idict))
            return QString::fromUtf8(parse_data(m_sdLibs->poGetWordData(ind, idict)).c_str());
    return QString();
}

void DictCore::lookupWithFuzzy(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    gchar *fuzzy_res[MaxFuzzy];
    if (! m_sdLibs->LookupWithFuzzy(str.c_str(), fuzzy_res, MaxFuzzy))
        return ;

    for (gchar **p = fuzzy_res, **end = fuzzy_res + MaxFuzzy;
            p != end && *p; ++p)
    {
        simpleLookup(*p, resultList);
        g_free(*p);
    }
}

void DictCore::lookupWithRule(const std::string &str, SearchResultList &resultList)
{
    std::vector<gchar *> match_res((MAX_MATCH_ITEM_PER_LIB) * m_sdLibs->ndicts());

    gint nfound = m_sdLibs->LookupWithRule(str.c_str(), &match_res[0]);
    if (!nfound)
        return ;

    for (gint i = 0; i < nfound; ++i)
    {
        simpleLookup(match_res[i], resultList);
        g_free(match_res[i]);
    }
}

void DictCore::lookupData(const std::string &str, SearchResultList &resultList)
{
    std::vector<gchar *> drl[m_sdLibs->ndicts()];
    if (! m_sdLibs->LookupData(str.c_str(), drl))
        return ;
    for (int idict = 0; idict < m_sdLibs->ndicts(); ++idict)
        for (std::vector<gchar *>::size_type j = 0; j<drl[idict].size(); ++j)
        {
            simpleLookup(drl[idict][j], resultList)
            ;
            g_free(drl[idict][j]);
        }
}

namespace
{
std::string xdxf2html(const char *p) // taken from sdcv
{
    std::string res;
    for (; *p; ++p)
    {
        if (*p != '<')
        {
            if (*p == '\n')
                res += "<br>";
            else
                res += *p;
            continue;
        }

        const char *next = strchr(p, '>');
        if (!next)
            continue;

        std::string name(p + 1, next - p - 1);

        if (name == "abr")
            res += "<font class=\"abbreviature\">";
        else if (name == "/abr")
            res += "</font>";
        else if (name == "k")
        {
            const char *begin = next;
            if ((next = strstr(begin, "</k>")) != 0)
                next += sizeof("</k>") - 1 - 1;
            else
                next = begin;
        }
        else if (name == "tr")
            res += "<font class=\"transcription\">[";
        else if (name == "/tr")
            res += "]</font>";
        else if (name == "ex")
            res += "<font class=\"example\">";
        else if (name == "/ex")
            res += "</font>";
        else if (!name.empty() && name[0] == 'c' && name != "co")
        {
            std::string::size_type pos = name.find("code");
            if (pos != std::string::size_type( -1))
            {
                pos += sizeof("code=\"") - 1;
                std::string::size_type end_pos = name.find("\"");
                std::string color(name, pos, end_pos - pos);
                res += "";
            }
            else
            {
                res += "";
            }
        }
        else if (name == "/c")
            res += "";

        p = next;
    }
    return res;
}

std::string parse_data(const char *data) // taken from sdcv
{
    using std::string;

    if (!data)
        return "";

    string res;
    guint32 data_size, sec_size = 0;
    gchar *m_str;
    const gchar *p = data;
    data_size = *((guint32 *)p);
    p += sizeof(guint32);
    while (guint32(p - data) < data_size)
    {
        switch (*p++)
        {
        case 'm':
        case 'l':  //need more work...
        case 'g':
            sec_size = strlen(p);
            if (sec_size)
            {
                res += "\n";
                m_str = g_strndup(p, sec_size);
                res += m_str;
                g_free(m_str);
            }
            ++sec_size;
            break;
        case 'x':
            sec_size = strlen(p);
            if (sec_size)
            {
                res += "\n";
                m_str = g_strndup(p, sec_size);
                res += xdxf2html(m_str);
                g_free(m_str);
            }
            ++sec_size;
            break;
        case 't':
            sec_size = strlen(p);
            if (sec_size)
            {
                res += "\n";
                m_str = g_strndup(p, sec_size);
                res += "[" + string(m_str) + "]";
                g_free(m_str);
            }
            ++sec_size;
            break;
        case 'y':
            sec_size = strlen(p);
            ++sec_size;
            break;
        case 'W':
        case 'P':
            sec_size = *((guint32 *)p);
            sec_size += sizeof(guint32);
            break;
        }
        p += sec_size;
    }
    return res;
}

QString html2text(QString html)
{
    return html.replace("<br>", "\n").
                remove(QRegExp("<.*>")).
                replace("&lt;", "<").
                replace("&gt;", ">").
                replace("&amp;", "&").
                replace("&quot;", "\"");
}
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

