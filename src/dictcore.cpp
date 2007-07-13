#include "dictcore.h"

#include <map>
#include <utility>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <glib.h>
#include "lib/lib.h"
#include "lib/file.hpp"

DictCore::DictCore(QObject *parent)
    : QObject(parent)
{
    sdLibs = NULL;

#ifdef Q_OS_UNIX
    m_dictDirs << "/usr/share/stardict/dic";
#else
    m_dictDirs << QCoreApplication::applicationDirPath() + "/dic";
#endif // Q_OS_UNIX
    m_dictDirs << QDir::homePath() + "/.stardict/dic";
}

DictCore::~DictCore()
{
    delete sdLibs;
}

void DictCore::setDicts(const QStringList &orderedDicts)
{
    m_orderedDicts = orderedDicts;

    delete sdLibs; 
    sdLibs = new Libs;

    for (QStringList::ConstIterator dictName = m_orderedDicts.begin(); dictName != m_orderedDicts.end(); dictName++)
        for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); dictDir++)
            if (QFile::exists(*dictDir + "/" + *dictName + ".ifo"))
            {
                sdLibs->load_dict(QDir::toNativeSeparators(*dictDir + "/" + *dictName + ".ifo").toUtf8().data());
                break;
            }
}

QStringList DictCore::orderedDicts() const
{
    return m_orderedDicts;
}

QStringList DictCore::disabledDicts()
{
    QStringList avialable = avialableDicts();
    QStringList disabled;

    for (QStringList::ConstIterator dictName = avialable.begin(); dictName != avialable.end(); dictName++)
        if (! m_orderedDicts.contains(*dictName))
            disabled << *dictName;

    return disabled;
}

QStringList DictCore::avialableDicts()
{
    QStringList result;

    for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); dictDir++)
        result << QDir(*dictDir).entryList(QStringList("*.ifo"), QDir::Files);
    for (QStringList::Iterator dictName = result.begin(); dictName != result.end(); dictName++)
        dictName->remove(QRegExp("\\.ifo$"));

    return result;
}

QStringList DictCore::find(const QString &str)
{
    SearchResultList resultList;
    lookupWithFuzzy(str.toUtf8().data() + std::string("*"), resultList);
    QStringList result;
    for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); i++)
        if (! result.contains(QString::fromUtf8(i->def.c_str())))
            result << QString::fromUtf8(i->def.c_str());

    return result;
}

bool DictCore::isTranslatable(const QString &str)
{
    if (str.isEmpty())
        return false;
    long ind;
    for (int idict = 0; idict < sdLibs->ndicts(); idict++)
		if (sdLibs->SimpleLookupWord(str.toUtf8().data(), ind, idict))
            return true;
    return false;
}

QString DictCore::translate(const QString &str, bool simple, bool useHtml)
{
    SearchResultList resultList;
    std::string query;

    if (str.isEmpty())
        return tr("Not found!");

    if (simple)
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

    std::string result;
    if (useHtml)
        for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); i++)
            result += "<p><font color=blue><i>" + i->dictName + "</i></font><br/>"
                "<font size=5><b>" + i->def + "</b></font><br/>" +
                "" + i->exp + "</p>";
    else
        for (SearchResultList::const_iterator i = resultList.begin(); i != resultList.end(); i++)
            result += "<-- " + i->dictName + " -->\n" +
                "--> " + i->def + "\n" +
                i->exp + "\n\n";

    return QString::fromUtf8(result.c_str());
}

void DictCore::simpleLookup(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    glong ind;
	resultList.reserve(sdLibs->ndicts());
	for (int idict = 0; idict < sdLibs->ndicts(); idict++)
		if (sdLibs->SimpleLookupWord(str.c_str(), ind, idict))
			resultList.push_back(
				SearchResult(sdLibs->dict_name(idict), 
                    sdLibs->poGetWord(ind, idict),
                    parse_data(sdLibs->poGetWordData(ind, idict))));
}

void DictCore::lookupWithFuzzy(const std::string &str, SearchResultList &resultList) // taken from sdcv
{
    static const int MAXFUZZY=10;

	gchar *fuzzy_res[MAXFUZZY];
	if (! sdLibs->LookupWithFuzzy(str.c_str(), fuzzy_res, MAXFUZZY))
		return;
	
	for (gchar **p=fuzzy_res, **end=fuzzy_res+MAXFUZZY; 
	     p!=end && *p; ++p) {
		simpleLookup(*p, resultList);
		g_free(*p);
	}
}

void DictCore::lookupWithRule(const std::string &str, SearchResultList &resultList)
{
    std::vector<gchar *> match_res((MAX_MATCH_ITEM_PER_LIB) * sdLibs->ndicts());

	gint nfound=sdLibs->LookupWithRule(str.c_str(), &match_res[0]);
	if (!nfound)
		return;

	for (gint i=0; i<nfound; ++i) {
		simpleLookup(match_res[i], resultList);
		g_free(match_res[i]);
	}
}

void DictCore::lookupData(const std::string &str, SearchResultList &resultList)
{
    std::vector<gchar *> drl[sdLibs->ndicts()];
	if (! sdLibs->LookupData(str.c_str(), drl))
		return;
	for (int idict=0; idict<sdLibs->ndicts(); ++idict)
		for (std::vector<gchar *>::size_type j=0; j<drl[idict].size(); ++j) {
			simpleLookup(drl[idict][j], resultList);
			g_free(drl[idict][j]);
		}
}

std::string DictCore::xdxf2text(const char *p) // taken from sdcv
{
	std::string res;
	for (; *p; ++p) {
		if (*p!='<') {
			if (g_str_has_prefix(p, "&gt;")) {
				res+=">";
				p+=3;
			} else if (g_str_has_prefix(p, "&lt;")) {
				res+="<";
				p+=3;
			} else if (g_str_has_prefix(p, "&amp;")) {
				res+="&";
				p+=4;
			} else if (g_str_has_prefix(p, "&quot;")) {
				res+="\"";
				p+=5;
			} else
				res+=*p;
			continue;
		}

		const char *next=strchr(p, '>');
		if (!next)
			continue;

		std::string name(p+1, next-p-1);

		if (name=="abr")
			res+="";
		else if (name=="/abr")
			res+="";
		else if (name=="k") {
			const char *begin=next;
			if ((next=strstr(begin, "</k>"))!=NULL)
				next+=sizeof("</k>")-1-1;
			else
				next=begin;
		} else if (name=="b")
			res+="";
		else if (name=="/b")
			res+="";
		else if (name=="i")
			res+="";
		else if (name=="/i")
			res+="";
		else if (name=="tr")
			res+="[";
		else if (name=="/tr")
			res+="]";
		else if (name=="ex")
			res+="";
		else if (name=="/ex")
			res+="";
		else if (!name.empty() && name[0]=='c' && name!="co") {
			std::string::size_type pos=name.find("code");
			if (pos!=std::string::size_type(-1)) {
				pos+=sizeof("code=\"")-1;
				std::string::size_type end_pos=name.find("\"");
				std::string color(name, pos, end_pos-pos);
				res+="";
			} else {
				res+="";
			}
		} else if (name=="/c")
			res+="";

		p=next;
	}
	return res;
}


std::string DictCore::parse_data(const char *data) // taken from sdcv
{
    using std::string;

	if (!data)
		return "";

	string res;
	guint32 data_size, sec_size=0;
	gchar *m_str;
	const gchar *p=data;
	data_size=*((guint32 *)p);
	p+=sizeof(guint32);
	while (guint32(p - data)<data_size) {
		switch (*p++) {
		case 'm':
		case 'l': //need more work...
		case 'g':
			sec_size = strlen(p);
			if (sec_size) {
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += m_str;
				g_free(m_str);
			}
			sec_size++;
			break;
		case 'x':
			sec_size = strlen(p);
			if (sec_size) {
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += xdxf2text(m_str);
				g_free(m_str);
			}
			sec_size++;
			break;
		case 't':
			sec_size = strlen(p);
			if(sec_size){
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += "["+string(m_str)+"]";
				g_free(m_str);
			}
			sec_size++;
			break;
		case 'y':
			sec_size = strlen(p);
			sec_size++;				
			break;
		case 'W':
		case 'P':
			sec_size=*((guint32 *)p);
			sec_size+=sizeof(guint32);
			break;
		}
		p += sec_size;
	}

  
	return res;
}

