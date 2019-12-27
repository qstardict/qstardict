/*****************************************************************************
 * application.cpp - QStarDict, a quasi-star dictionary                      *
 * Copyright (C) 2008-2019 Alexander Rodin                                   *
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

#include <QCommandLineParser>

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
#include "pluginmanager.h"
#ifdef QSTARDICT_WITH_TRAY_ICON
#include "trayicon.h"
#endif
#ifdef QSTARDICT_WITH_DBUS
#include "dbusadaptor.h"
#endif // QSTARDICT_WITH_DBUS

#ifdef Q_OS_MAC
#include <QDebug>
#include <objc/objc.h>
#include <objc/message.h>
void setupDockClickHandler();
bool dockClickHandler(id self,SEL _cmd,...);
#endif

#ifdef Q_OS_MAC
void setupDockClickHandler() {
    Class cls = objc_getClass("NSApplication");
    typedef objc_object* (*SendType)(void*, SEL);
    SendType casted_objc_msgSend = (SendType)(objc_msgSend);
    objc_object *appInst = casted_objc_msgSend(cls, sel_registerName("sharedApplication"));

    if(appInst != NULL) {
        objc_object* delegate = casted_objc_msgSend(appInst, sel_registerName("delegate"));
        Class delClass = (Class)casted_objc_msgSend(delegate,  sel_registerName("class"));
        SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
        if (class_getInstanceMethod(delClass, shouldHandle)) {
            if (class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:"))
                qDebug() << "Registered dock click handler (replaced original method)";
            else
                qWarning() << "Failed to replace method for dock click handler";
        }
        else {
            if (class_addMethod(delClass, shouldHandle, (IMP)dockClickHandler,"B@:"))
                qDebug() << "Registered dock click handler";
            else
                qWarning() << "Failed to register dock click handler";
        }
    }
}

bool dockClickHandler(id self,SEL _cmd,...) {
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    // Do something fun here!
    ((QStarDict::Application*)qApp)->mainWindow()->show();
    // Return NO (false) to suppress the default OS X actions
    return false;
}
#endif

namespace QStarDict
{

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setOrganizationName("qstardict");
    setApplicationName("qstardict");
    setApplicationVersion(QSTARDICT_VERSION);
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

    QCommandLineParser parser;
    parser.setApplicationDescription(tr("A quasi-star international and intergalactical dictionary"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption minimizedOption({"m", "minimized"}, tr("Start with minimized main window."));
    parser.addOption(minimizedOption);
    parser.process(*this);

    m_pluginManager = new PluginManager;
    m_pluginManager->loadPlugins();
    m_dictCore = new DictCore;
    m_popupWindow = new PopupWindow;
    m_popupWindow->setDict(m_dictCore);
    m_speaker = new Speaker;
#ifdef QSTARDICT_WITH_TRAY_ICON
    m_trayIcon = new TrayIcon;
#endif
    m_popupShortcut = new QxtGlobalShortcut;
    m_mainWindow = new MainWindow(nullptr, parser.isSet(minimizedOption));
#ifdef QSTARDICT_WITH_TRAY_ICON
	m_trayIcon->setMainWindow(m_mainWindow);
#endif
	m_mainWindow->setDict(m_dictCore);
#ifdef QSTARDICT_WITH_DBUS
    m_dbusAdaptor = new DBusAdaptor(m_mainWindow);
#endif // QSTARDICT_WITH_DBUS

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    setAttribute(Qt::AA_UseHighDpiPixmaps);
#ifdef Q_OS_MAC
    setupDockClickHandler();
#endif
}

Application::~Application()
{
#ifdef QSTARDICT_WITH_TRAY_ICON
    delete m_trayIcon;
#endif
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

