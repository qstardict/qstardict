/*****************************************************************************
 * application.cpp - QStarDict, a StarDict clone written using Qt            *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "application.h"

#ifdef QSTARDICT_WITH_TRANSLATIONS
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <QStringList>
#endif // QSTARDICT_WITH_TRANSLATIONS

#include <QKeySequence>
#include "qxt/qxtglobalshortcut.h"

#include "dictcore.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "speaker.h"
#include "trayicon.h"
#include "pluginmanager.h"
#ifdef QSTARDICT_WITH_DBUS
#include "dbusadaptor.h"
#endif // QSTARDICT_WITH_DBUS

namespace QStarDict
{

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setOrganizationName("qstardict");
    setApplicationName("qstardict");
    setQuitOnLastWindowClosed(false);

#ifdef QSTARDICT_WITH_TRANSLATIONS
    m_translator = new QTranslator;
#ifdef Q_WS_MAC
    QString binPath = QCoreApplication::applicationDirPath();
    // navigate through mac's bundle tree structore
    m_translator->load("qstardict-" + QLocale::system().name(), binPath + "/../i18n/");
#else
    m_translator->load("qstardict-" + QLocale::system().name(), QSTARDICT_TRANSLATIONS_DIR);
#endif
    installTranslator(m_translator);
    m_qtTranslator = new QTranslator;
    m_qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    installTranslator(m_qtTranslator);
#endif // QSTARDICT_WITH_TRANSLATIONS

    m_pluginManager = new PluginManager;
    m_pluginManager->loadPlugins();
    m_dictCore = new DictCore;
    m_popupWindow = new PopupWindow;
    m_popupWindow->setDict(m_dictCore);
    m_speaker = new Speaker;
    m_trayIcon = new TrayIcon;
    m_popupShortcut = new QxtGlobalShortcut;
    m_mainWindow = new MainWindow;
	m_trayIcon->setMainWindow(m_mainWindow);
	m_mainWindow->setDict(m_dictCore);
#ifdef QSTARDICT_WITH_DBUS
    m_dbusAdaptor = new DBusAdaptor(m_mainWindow);
#endif // QSTARDICT_WITH_DBUS

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    setAttribute(Qt::AA_UseHighDpiPixmaps);
}

Application::~Application()
{
    delete m_trayIcon;
    delete m_mainWindow;
    delete m_popupWindow;
    delete m_speaker;
    delete m_dictCore;
    delete m_popupShortcut;
#ifdef QSTARDICT_WITH_TRANSLATIONS
    removeTranslator(m_translator);
    delete m_translator;
    removeTranslator(m_qtTranslator);
    delete m_qtTranslator;
    delete m_pluginManager;
#endif // QSTARDICT_WITH_TRANSLATIONS
}

int Application::exec()
{
    QString text = commandLineText();
    if (text != QString::null)
        m_mainWindow->showTranslation(text);
    return QApplication::exec();
}

QString Application::commandLineText()
{
    QStringList args(arguments());
    for(int i = 1; i < args.count(); ++i)
    {
        if(! args.at(i).startsWith('-'))
            return args.at(i);
    }
    return QString::null;
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

