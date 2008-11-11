/*
 *   Copyright (C) 2008 Nick Shaforostoff <shaforostoff@kde.ru>
 *
 *   based on work by:
 *   Copyright (C) 2007 Thomas Georgiou <TAGeorgiou@gmail.com> and Jeff Cooper <weirdsox11@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dictengine.h"
#include "dictplugin.h"
#include "config.h"

#include <QPluginLoader>
#include <QPointer>
#include <QMap>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include <KDebug>
// #include <KLocale>

#include <Plasma/DataContainer>

#define MEMORYFREE_DELAY 60000


/**
 * This class represents a dictionary provided by a plugin
 */
class Dictionary
{
    public:
        Dictionary(const QString &plugin, const QString &name)
            : m_plugin(plugin)
            , m_name(name)
        {}
        Dictionary()
        {}

        const QString &plugin() const {return m_plugin;}
        const QString &name() const {return m_name;}
        void setPlugin(const QString &plugin) {m_plugin=plugin;}
        void setName(const QString &name) {m_name=name;}
        bool operator == (const Dictionary &dict) {return m_name==dict.m_name && m_plugin==dict.m_plugin;}

    private:
        QString m_plugin;
        QString m_name;
};


struct QStarDictEngine::Private
{
    QString currentWord;
    QTimer timer;

    QHash<QString, QPointer<QPluginLoader> > plugins; //name to pointer
//     QList<Dictionary> dicts;
//     QHash<QString, QString> dictToPlugin; //name to metastructure
    QHash<QString, Dictionary> dicts; //name to metastructure

};






QStarDictEngine::QStarDictEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
    , d(new Private)
{
    Q_UNUSED(args)
//     QTime a;a.start();
#ifdef Q_OS_WIN
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("*0.dll"),QDir::Files|QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        d->plugins[i->fileName().left(i->fileName().length()-5))]=0;
#else
    qWarning()<<QSTARDICT_PLUGINS_DIR;
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("lib*.so"),QDir::Files|QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        d->plugins[i->fileName().mid(3, i->fileName().length()-6)]=0;
#endif



    // use cache to not load redudant plugins? 16->10 ms
//     QSettings settings("qstardict","qstardict");
//     m_dictDirs = settings.value("Multitran/dictDirs", m_dictDirs).toStringList();

    d->timer.setInterval(MEMORYFREE_DELAY);
    d->timer.setSingleShot(true);
    connect(&d->timer, SIGNAL(timeout()), this, SLOT(unloadPlugins()));


    QList<QString> plugins=d->plugins.keys();
    for (QList<QString>::const_iterator i = plugins.constBegin(); i != plugins.constEnd(); ++i)
    {
//         QStringList dicts = settings.value(*i+"/dicts", QStringList()).toStringList();
//         if (!dicts.isEmpty())
//         {
            QStarDict::DictPlugin* plugin = dictPlugin(*i);
            if (!plugin)
                continue;
            QStringList dicts=plugin->availableDicts();
//             settings.setValue(*i+"/dicts", QVariant(dicts));
//            plugin->setdicts(dicts);
//         }
        for (QStringList::const_iterator j = dicts.constBegin(); j != dicts.constEnd(); ++j)
            d->dicts[*j]=Dictionary(*i, *j);
//             d->dicts<<Dictionary(*i, *j);
    }
//     qWarning()<<a.elapsed();

}

QStarDictEngine::~QStarDictEngine()
{
    unloadPlugins();
    delete d;
}



QStarDict::DictPlugin* QStarDictEngine::dictPlugin(const QString &name)
{
    if (! d->plugins.contains(name) )
        return 0;
    if (! d->plugins.value(name) )
    {
#ifdef Q_OS_WIN
        QString pluginFilename = QSTARDICT_PLUGINS_DIR "/" + name + "0.dll";
#else 
        QString pluginFilename = QSTARDICT_PLUGINS_DIR "/" "lib" + name + ".so";
#endif
        QPluginLoader* plugin = new QPluginLoader(pluginFilename);
        if (! plugin->load())
        {
            kWarning() << plugin->errorString();
            delete plugin;
            return 0;
        }
        d->plugins[name]=plugin;
    }

    return qobject_cast<QStarDict::DictPlugin*>(d->plugins.value(name)->instance());
}

void QStarDictEngine::unloadPlugins()
{
    for (QHash<QString, QPointer<QPluginLoader> >::iterator i = d->plugins.begin(); i != d->plugins.end(); ++i)
    {
        if (*i)
        {
            (*i)->instance()->deleteLater();
            (*i)->deleteLater();
        }
    }

}

bool QStarDictEngine::sourceRequestEvent(const QString &word)
{
    qWarning()<<"sourceRequestEvent"<<word;
    d->timer.start();//delay freeing resources / activate delayed resources freeing

    if (word=="list-dictionaries")
    {
        QStringList result;
        int i=d->plugins.keys().size();
        while (--i>=0)
        {
            QStarDict::DictPlugin* plugin = dictPlugin(d->plugins.keys().at(i));
            if (!plugin)
                continue;
            result << plugin->availableDicts();
        }
        setData("list-dictionaries", "dictionaries", result);
        qWarning()<<result;
        return true;
    }

    QString simplifiedWord;
    QStringList queriedDicts;
    int pos=word.indexOf(':');
    if (pos!=-1)
    {
        queriedDicts=word.left(pos).split(',');
        simplifiedWord=word.mid(pos+1).simplified();
    }
    else
        simplifiedWord=word.simplified();
    qWarning()<<"simplifiedWord"<<simplifiedWord;
    qWarning()<<"queriedDicts"<<queriedDicts;

    if (queriedDicts.isEmpty())
        for (QHash<QString,Dictionary>::const_iterator i = d->dicts.constBegin(); i != d->dicts.constEnd(); ++i)
            queriedDicts<<i->name();

    d->currentWord = word;
    QString result;

    if (simplifiedWord.length() == 0)
    {
        setData(d->currentWord, "text", QString());
        return true;
    }

    for (QStringList::const_iterator i = queriedDicts.constBegin(); i != queriedDicts.constEnd(); ++i)
    {
        if (!d->dicts.contains(*i))
            continue;
        QStarDict::DictPlugin* plugin = dictPlugin(d->dicts.value(*i).plugin());
        if (!plugin)
            continue;
        QStringList ld=plugin->loadedDicts();
        if (!ld.contains(*i))
            plugin->setLoadedDicts(ld<<*i);
        if ( !plugin->isTranslatable(*i, simplifiedWord))
            continue;
        QStarDict::DictPlugin::Translation translation = plugin->translate(*i, simplifiedWord);
        if (translation.translation().isEmpty())
            continue;
        result += "<p>\n"
            "<font class=\"dict_name\">" + translation.dictName() + "</font><br>\n"
            "<font class=\"title\">" + translation.title() + "</font><br>\n"
            + translation.translation() + "</p>\n";
    }

    setData(d->currentWord, "text", result);
    return true;
}





#include "dictengine.moc"
